/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
/* USER CODE BEGIN Includes */
#define EN_WATCHDOG   1
#define SPECIAL_TEMP  200
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define EEPROM_SCL_Pin GPIO_PIN_2
#define EEPROM_SCL_GPIO_Port GPIOC
#define EEPROM_SDA_Pin GPIO_PIN_3
#define EEPROM_SDA_GPIO_Port GPIOC
#define RS485_DE_Pin GPIO_PIN_1
#define RS485_DE_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_2
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_3
#define RS485_RX_GPIO_Port GPIOA
#define WIFI_STATE_Pin GPIO_PIN_6
#define WIFI_STATE_GPIO_Port GPIOA
#define WIFI_KEY_Pin GPIO_PIN_7
#define WIFI_KEY_GPIO_Port GPIOA
#define WIFI_TX_Pin GPIO_PIN_4
#define WIFI_TX_GPIO_Port GPIOC
#define WIFI_RX_Pin GPIO_PIN_5
#define WIFI_RX_GPIO_Port GPIOC
#define FAN_HIGH_Pin GPIO_PIN_0
#define FAN_HIGH_GPIO_Port GPIOB
#define FAN_MID_Pin GPIO_PIN_1
#define FAN_MID_GPIO_Port GPIOB
#define FAN_LOW_Pin GPIO_PIN_2
#define FAN_LOW_GPIO_Port GPIOB
#define MOTOR_AIR_Pin GPIO_PIN_10
#define MOTOR_AIR_GPIO_Port GPIOB
#define MOTOR_WATER_Pin GPIO_PIN_11
#define MOTOR_WATER_GPIO_Port GPIOB
#define K3_CS_Pin GPIO_PIN_12
#define K3_CS_GPIO_Port GPIOB
#define K3_SCK_Pin GPIO_PIN_13
#define K3_SCK_GPIO_Port GPIOB
#define K3_MISO_Pin GPIO_PIN_14
#define K3_MISO_GPIO_Port GPIOB
#define DWIN_TX_Pin GPIO_PIN_9
#define DWIN_TX_GPIO_Port GPIOA
#define DWIN_RX_Pin GPIO_PIN_10
#define DWIN_RX_GPIO_Port GPIOA
#define ALARM_HEAT_Pin GPIO_PIN_11
#define ALARM_HEAT_GPIO_Port GPIOA
#define ALARM_WATER_Pin GPIO_PIN_12
#define ALARM_WATER_GPIO_Port GPIOA
#define HEATERROD1_Pin GPIO_PIN_15
#define HEATERROD1_GPIO_Port GPIOA
#define HEATERROD2_Pin GPIO_PIN_10
#define HEATERROD2_GPIO_Port GPIOC
#define HEATERROD3_Pin GPIO_PIN_11
#define HEATERROD3_GPIO_Port GPIOC
#define K1_CS_Pin GPIO_PIN_2
#define K1_CS_GPIO_Port GPIOD
#define K1_SCK_Pin GPIO_PIN_3
#define K1_SCK_GPIO_Port GPIOB
#define K1_MISO_Pin GPIO_PIN_4
#define K1_MISO_GPIO_Port GPIOB
#define K2_CS_Pin GPIO_PIN_5
#define K2_CS_GPIO_Port GPIOB
#define K2_SCK_Pin GPIO_PIN_6
#define K2_SCK_GPIO_Port GPIOB
#define K2_MISO_Pin GPIO_PIN_7
#define K2_MISO_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
