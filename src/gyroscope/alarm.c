/*
 * alarm.c
 *
 *  Created on: 2016. nov. 26.
 *      Author: Bence
 */

#include <gyroscope/alarm.h>
#include <state_machine.h>
#include <gyroscope/FXAS21002C_driver.h>
#include <GPRS_GSM_GPS/SIM808_GPS.h>
#include <GPRS_GSM_GPS/SIM808_GSM.h>

static uint32_t alarm_triggered_moment = 0;

void configure_alarm(uint16_t threshold, uint16_t debounce_time)
{
  enter_mode_FXAS21002C(Standby);

  s_RT_count_FXAS21002C(calculate_RT_DCnt_value_FXAS21002C(debounce_time));
  s_RT_DCnt_mode_FXAS21002C(False);

  s_RT_threshold_FXAS21002C(threshold);

  s_interrupt_polarity_FXAS21002C(ACTIVE_HIGH);
  s_interrupt_pin_mode_FXAS21002C(PUSH_PULL);
}

void enable_alarm(boolean_t enable)
{
  enter_mode_FXAS21002C(Standby);

  enable_RT_event_FXAS21002C(Z_axis, enable);
  enable_RT_event_FXAS21002C(Y_axis, enable);
  enable_RT_event_FXAS21002C(X_axis, enable);

  enable_RT_intr_on_pin_FXAS21002C(enable, INT1);

  if(True == enable)
  {
    enter_mode_FXAS21002C(Active);
  }
}

void identify_alarm_event(uint16_t sec)
{
  if(alarm_triggered_moment == 0)
  {
    alarm_triggered_moment = HAL_GetTick();
  }

  if((HAL_GetTick() - alarm_triggered_moment) > (sec*1000) )
  {
    GPS_data_t data = get_data_GPS();

    if(data.valid == True)
    {
      state_SBH =  (data.speed_over_ground > 0.5) ? ALARM_THEFT : ALARM_DISPLACEMENT;
      alarm_triggered_moment = 0;
    }
  }
}

boolean_t send_theft_alarm_msg(char* number)
{
  char message[160] = {0};
  GPS_data_t data = get_data_GPS();

  if(data.valid == True)
  {
    snprintf(message, 159, "THEFT longitude: %f latitude: %f speed: %05f",
        data.longitude, data.latitude, data.speed_over_ground);
    return send_SMS(message, number);
  }
  else
  {
    return False;
  }
}

boolean_t send_displacement_alarm_msg(char* number)
{
  char message[160] = {0};
  GPS_data_t data = get_data_GPS();

  if(data.valid == True)
  {
    snprintf(message, 159, "DISPLACEMENT longitude: %f latitude: %f speed: %05f",
        data.longitude, data.latitude, data.speed_over_ground);
    return send_SMS(message, number);
  }
  else
  {
    return False;
  }
}
