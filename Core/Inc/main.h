/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define SLEEP_TIMEOUT 60000

typedef enum
{
	ePASSIVE,
	eLOCKING,
	eUNLOCKING,
	eSETTING_PASSCODE
} actionState;

typedef enum
{
	eLOCKED,
	eUNLOCKED
} lockStatus;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOF
#define ROW_1_Pin GPIO_PIN_1
#define ROW_1_GPIO_Port GPIOF
#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOA
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOA
#define LED_4_Pin GPIO_PIN_4
#define LED_4_GPIO_Port GPIOA
#define DIR_Pin GPIO_PIN_5
#define DIR_GPIO_Port GPIOA
#define STEP_Pin GPIO_PIN_6
#define STEP_GPIO_Port GPIOA
#define ROW_3_Pin GPIO_PIN_0
#define ROW_3_GPIO_Port GPIOB
#define ROW_2_Pin GPIO_PIN_1
#define ROW_2_GPIO_Port GPIOB
#define COL_2_Pin GPIO_PIN_11
#define COL_2_GPIO_Port GPIOA
#define ROW_4_Pin GPIO_PIN_12
#define ROW_4_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define COL_1_Pin GPIO_PIN_3
#define COL_1_GPIO_Port GPIOB
#define COL_3_Pin GPIO_PIN_5
#define COL_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define PASSCODE_SIZE       4
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
