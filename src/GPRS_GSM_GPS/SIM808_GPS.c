/*
 * SIM808_GPS.c
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#include <GPRS_GSM_GPS/SIM808_GPS.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>

#include <device_management.h>
#include "string.h"

static GPS_data_t gps_data = {0};
static boolean_t ready = False;
static boolean_t on = False;

static void parse_GPS_data(char* data);
static void skip_field(char** pointer);
static int8_t get_sign(char** pointer);
static void get_digits_num(uint8_t* digits, char delimiter, char* data);
static void parse_coordinate_fraction(float* coorindate, char** pointer);

boolean_t get_state_GPS()
{
  return on;
}

void enable_GPS(boolean_t enable)
{
  if(True == enable)
  {
    enable_GPS_antenna(True);
    put_s_SIM808("AT+CGNSPWR=1\r");
    HAL_Delay(100);
    put_s_SIM808("AT+CGNSSEQ=\"RMC\"\r");
    on = True;
  }
  else
  {
    put_s_SIM808("AT+CGNSPWR=0\r");
    enable_GPS_antenna(False);
    on = False;
  }
}

GPS_data_t get_data_GPS()
{
  put_s_SIM808("AT+CGNSINF\r");
  get_s_SIM808(4, parse_GPS_data);

  waitFor(&ready, 1000);

  return gps_data;
}

static void parse_GPS_data(char* data)
{
    boolean_t gps_status = False, fix_status = False;
    int8_t sign = 1;
    uint8_t digits = 0;

    if(strstr(data, "+CGNSINF") == NULL)
    {
        rx_error = False;
        ready = False;
    }
    else
    {
        /* remove header */
        while(data[0] != ':')
        {
            data++;
        }
        data+=2;

        /* parse GPS and fix status */
        gps_status = data[0] - 48;
        data+=2;
        fix_status = data[0] - 48;
        data+=2;

        if(gps_status == False || fix_status == False)
        {
            gps_data.valid = False;
            return;
        }
        else
        {
            gps_data.valid = True;
            gps_data.latitude = 0;
            gps_data.longitude = 0;
            gps_data.speed_over_ground = 0;
        }

        /* skip UTC date & time */
        skip_field(&data);

        /* parse latitude */
        sign = get_sign(&data);
        get_digits_num(&digits, '.', data);

        if(digits == 2)
            gps_data.latitude = (data[0] - 48) * 10 + (data[1] - 48);
        else if(digits == 1)
            gps_data.latitude = (data[0] - 48);
        else
        {
            gps_data.valid = False;
            return;
        }

        data += digits + 1;
        parse_coordinate_fraction(&gps_data.latitude, &data);
        gps_data.latitude *= sign;

        /* parse longitude */
        sign = get_sign(&data);
        get_digits_num(&digits, '.', data);

        if(digits == 3)
            gps_data.longitude = (data[0] - 48) * 100 + (data[1] - 48) * 10 + (data[2] - 48);
        else if(digits == 2)
            gps_data.longitude = (data[0] - 48) * 10 + (data[1] - 48);
        else if(digits == 1)
            gps_data.longitude = (data[0] - 48);
        else
        {
            gps_data.valid = False;
            return;
        }

        data += digits + 1;
        parse_coordinate_fraction(&gps_data.longitude, &data);
        gps_data.longitude *= sign;

        /* skip MSL Altitude */
        skip_field(&data);

        /* parse speed over ground */
        get_digits_num(&digits, '.', data);

        if(digits == 3)
            gps_data.speed_over_ground = (data[0] - 48) * 100 + (data[1] - 48) * 10 + (data[2] - 48);
        else if(digits == 2)
            gps_data.speed_over_ground = (data[0] - 48) * 10 + (data[1] - 48);
        else if(digits == 1)
            gps_data.speed_over_ground = (data[0] - 48);
        else
        {
            gps_data.valid = False;
            return;
        }

        data += digits + 1;
        get_digits_num(&digits, ',', data);

        if(digits == 2)
        {
            gps_data.speed_over_ground += (data[0] - 48)/10.0;
            data++;
            gps_data.speed_over_ground += (data[0] - 48)/100.0;
        }
        else if(digits == 1)
            gps_data.speed_over_ground += (data[0] - 48)/10.0;

        ready = True;
    }
}

static void skip_field(char** pointer)
{
    while((*pointer)[0] != ',')
    {
        (*pointer)++;
    }
    (*pointer)++;
}

static int8_t get_sign(char** pointer)
{
    if((*pointer)[0] == '-')
    {
        (*pointer)++;
        return -1;
    }
    else
    {
        return 1;
    }
}

static void get_digits_num(uint8_t* digits, char delimiter, char* data)
{
    *digits = 0;
    while(data[*digits] != delimiter)
    {
        (*digits)++;
    }
}

static void parse_coordinate_fraction(float* coorindate, char** pointer)
{
    (*coorindate) += ((*pointer)[0]- 48)/10.0;
    (*pointer)++;
    (*coorindate) += ((*pointer)[0] - 48)/100.0;
    (*pointer)++;
    (*coorindate) += ((*pointer)[0] - 48)/1000.0;
    (*pointer)++;
    (*coorindate) += ((*pointer)[0] - 48)/10000.0;
    (*pointer)++;
    (*coorindate) += ((*pointer)[0] - 48)/100000.0;
    (*pointer)++;
    (*coorindate) += ((*pointer)[0] - 48)/1000000.0;
    (*pointer)+=2;
}


