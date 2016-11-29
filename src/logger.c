/*
 * logger.c
 *
 *  Created on: 2016. nov. 25.
 *      Author: Bence
 */

#include <logger.h>

#include <hum_temp_sensor/Si7021_driver.h>
#include <gyroscope/FXAS21002C_driver.h>
#include <weight_measurement.h>
#include <power_management.h>

#include <GPRS_GSM_GPS/SIM808_device.h>
#include <state_machine.h>

static hive_data_t hive_log[LOG_PERIOD] = {0};
device_t device = {0};

static hive_data_t hive = {0};

void refresh_device_data()
{
  device.MCU_vcc            = calculate_MCU_vcc();
  device.MCU_temperature    = r_MCU_temp(device.MCU_vcc);

  device.vbat               = r_battery_voltage(device.MCU_vcc);
  device.current            = r_supply_current(device.MCU_vcc);

  device.charger            = r_charger_stat();

  device.SIM808_temperature = r_temperature_SIM808();
  device.SIM808_vcc         = r_vcc_SIM808();

  //device.GYRO_temperature   = r_temp_FXAS21002C();
}

void log_hive_data(uint8_t log_cycle)
{
  hive.mass = measure_mass(device.MCU_vcc);
  r_both_Si7021(&(hive.humidity), &(hive.temperature));

  hive_log[log_cycle - 1] = hive;
}
