/*
 * constants.h
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/* PIN and PORT labels */

#define SIM_RST_Pin           GPIO_PIN_0
#define SIM_RST_GPIO_Port     GPIOF
#define SIM_PWR_KEY_Pin       GPIO_PIN_1
#define SIM_PWR_KEY_GPIO_Port GPIOF
#define GSM_RI_Pin            GPIO_PIN_0
#define GSM_RI_GPIO_Port      GPIOA
#define GPS_ANT_EN_Pin        GPIO_PIN_1
#define GPS_ANT_EN_GPIO_Port  GPIOA
#define GAUGE_OUT_Pin         GPIO_PIN_4
#define GAUGE_OUT_GPIO_Port   GPIOA
#define I_SUPP_Pin            GPIO_PIN_5
#define I_SUPP_GPIO_Port      GPIOA
#define V_BAT_Pin             GPIO_PIN_6
#define V_BAT_GPIO_Port       GPIOA
#define CHG_STAT_Pin          GPIO_PIN_0
#define CHG_STAT_GPIO_Port    GPIOB
#define CHG_EN_Pin            GPIO_PIN_1
#define CHG_EN_GPIO_Port      GPIOB
#define _4_2V_EN_Pin          GPIO_PIN_8
#define _4_2V_EN_GPIO_Port    GPIOA
#define GY_RST_Pin            GPIO_PIN_3
#define GY_RST_GPIO_Port      GPIOB
#define GY_INT1_Pin           GPIO_PIN_4
#define GY_INT1_GPIO_Port     GPIOB
#define GY_INT2_Pin           GPIO_PIN_5
#define GY_INT2_GPIO_Port     GPIOB
#define TACT_BUTTON           GPIO_PIN_6
#define TACT_BUTTON_GPIO_Port GPIOB
#define LED_Pin               GPIO_PIN_7
#define LED_GPIO_Port         GPIOB

#endif
