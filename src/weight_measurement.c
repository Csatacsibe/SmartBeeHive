/*
 * weight_measurement.c
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#include <device_management.h>
#include <weight_measurement.h>
#include <power_management.h>
#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>

#define WINDOW_SIZE   48
#define SAMPLE_NUMBER (uint32_t) 300

static float scale_slope = 2.6; // 2.6 mV / 100 g

static uint16_t unloaded = 0;
static uint16_t filter_buffer[WINDOW_SIZE];
static uint32_t mass = 0;
static uint32_t input_buffer[SAMPLE_NUMBER];
static uint16_t vcc = 2800;

uint16_t scale_raw;
float output;

boolean_t scale_sampling_done = False;
scale_callback_t scale_callback = NULL;

//static float average(uint16_t* buf, uint16_t size);
static void push(uint16_t* buffer, uint16_t size, uint16_t element);
static void find_min_max(uint16_t* buf, uint16_t* max, uint16_t* min, uint16_t size);
static float averaging_filter(uint16_t input);
static uint16_t r_wheatstone_bridge(uint16_t mcu_vcc);
static uint16_t process_bridge_output(uint16_t value);
static uint32_t convert_to_gram(float input);

void scale_init()
{
  uint16_t i;
  uint16_t mcu_vcc;
  uint32_t sum = 0;

  mcu_vcc = calculate_MCU_vcc();

  for(i = 0; i < 500; i++)
  {
    sum += r_wheatstone_bridge(mcu_vcc);  // set null point
  }

  unloaded = sum/500;

  for(i = 0; i < WINDOW_SIZE; i++)
  {
    filter_buffer[i] = 65535;
  }
}

uint32_t measure_mass(uint16_t mcu_vcc)
{
  //uint16_t scale_raw;
  float scale_averaged;

  scale_raw = process_bridge_output(r_wheatstone_bridge(mcu_vcc));
  scale_averaged = averaging_filter(scale_raw);

  return convert_to_gram(scale_averaged);
}

uint32_t r_hive_mass()
{
  return mass;
}

void start_scale_sampling(uint16_t mcu_vcc)
{
  vcc = mcu_vcc;
  scale_sampling_done = False;
  scale_callback = &process_scale_samples;
  config_ext_channel_ADC(STRAIN_GAUGE, True);
  HAL_ADCEx_Calibration_Start(&hadc);
  HAL_ADC_Start_DMA(&hadc, input_buffer, SAMPLE_NUMBER);
}

void process_scale_samples(void)
{
  config_ext_channel_ADC(STRAIN_GAUGE, False);
  scale_sampling_done = True;

  uint16_t i, voltage, raw;
  float averaged;


  for(i = 0; i < SAMPLE_NUMBER; i++)
  {
    voltage = (vcc/4095.0) * input_buffer[i];
    raw = process_bridge_output(voltage);
    averaged = averaging_filter(raw);

    mass = convert_to_gram(averaged);
  }
}

static uint32_t convert_to_gram(float input)
{
  if(input == -1)
  {
    return 0;
  }

  uint32_t mass;

  uint16_t integer = input;
  uint8_t fraction = (input - integer) * 100;
  float temp = integer + fraction/100.0;

  temp = temp/scale_slope;
  mass = temp;

  return mass * 100;
}

/*static float average(uint16_t* buf, uint16_t size)
{
  uint16_t i;
  float avg = 0;

  for(i = 0; i < size; i++)
  {
    avg += buf[i];
  }

  return avg/size;
}*/

static uint16_t r_wheatstone_bridge(uint16_t mcu_vcc)
{
  //float output;
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(STRAIN_GAUGE);
  output = (mcu_vcc/4095.0) * digital_val;

  return output;
}

static uint16_t process_bridge_output(uint16_t value)
{
  int16_t temp;
  temp = value - unloaded;

  if(temp < 0)
  {
    temp = 0;
  }

  return temp;
}

static float averaging_filter(uint16_t input)
{
  uint16_t i, invalid = 0;
  uint16_t min, max;
  float sum = 0;

  push(filter_buffer, WINDOW_SIZE, input);

  for(i = 0; i < WINDOW_SIZE; i++)
  {
    if(filter_buffer[i] == 65535)
    {
      invalid = 1;
    }

    sum += filter_buffer[i];
  }

  if(invalid)
  {
    return -1;
  }
  else
  {
    find_min_max(filter_buffer, &max, &min, WINDOW_SIZE);
    sum = sum - min - max;
    sum = sum/(WINDOW_SIZE-2);
    return sum;
  }
}

static void push(uint16_t* buffer, uint16_t size, uint16_t element)
{
  uint16_t i;

  for(i = size-1; i > 0; i--)
  {
    buffer[i] = buffer[i-1];
  }

  buffer[0] = element;
}

static void find_min_max(uint16_t* buf, uint16_t* max, uint16_t* min, uint16_t size)
{
  uint16_t i = 0;

  *max = 0;
  *min = 10000;

  for(i = 0; i < size; i++)
  {
    if(buf[i] > *max) *max = buf[i];
    if(buf[i] < *min) *min = buf[i];
  }
}


