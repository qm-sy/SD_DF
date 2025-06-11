/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
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
#define DR2_485_Pin GPIO_PIN_1
#define DR2_485_GPIO_Port GPIOA
#define TX2_485_Pin GPIO_PIN_2
#define TX2_485_GPIO_Port GPIOA
#define RX2_485_Pin GPIO_PIN_3
#define RX2_485_GPIO_Port GPIOA
#define IN_24V_Pin GPIO_PIN_4
#define IN_24V_GPIO_Port GPIOA
#define TFT_SCK_Pin GPIO_PIN_5
#define TFT_SCK_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_6
#define TFT_DC_GPIO_Port GPIOA
#define TFT_SDA_Pin GPIO_PIN_7
#define TFT_SDA_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_4
#define TFT_RESET_GPIO_Port GPIOC
#define OUT_24V_Pin GPIO_PIN_5
#define OUT_24V_GPIO_Port GPIOC
#define Analog1_Pin GPIO_PIN_0
#define Analog1_GPIO_Port GPIOB
#define Analog2_Pin GPIO_PIN_1
#define Analog2_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_2
#define Buzzer_GPIO_Port GPIOB
#define TX3_485_Pin GPIO_PIN_10
#define TX3_485_GPIO_Port GPIOB
#define RX3_485_Pin GPIO_PIN_11
#define RX3_485_GPIO_Port GPIOB
#define DR3_485_Pin GPIO_PIN_12
#define DR3_485_GPIO_Port GPIOB
#define led5_Pin GPIO_PIN_13
#define led5_GPIO_Port GPIOB
#define led4_Pin GPIO_PIN_14
#define led4_GPIO_Port GPIOB
#define led3_Pin GPIO_PIN_15
#define led3_GPIO_Port GPIOB
#define button5_Pin GPIO_PIN_6
#define button5_GPIO_Port GPIOC
#define button4_Pin GPIO_PIN_7
#define button4_GPIO_Port GPIOC
#define button3_Pin GPIO_PIN_8
#define button3_GPIO_Port GPIOC
#define button2_Pin GPIO_PIN_9
#define button2_GPIO_Port GPIOC
#define button1_Pin GPIO_PIN_8
#define button1_GPIO_Port GPIOA
#define TX1_485_Pin GPIO_PIN_9
#define TX1_485_GPIO_Port GPIOA
#define RX1_485_Pin GPIO_PIN_10
#define RX1_485_GPIO_Port GPIOA
#define DR1_485_Pin GPIO_PIN_11
#define DR1_485_GPIO_Port GPIOA
#define led2_Pin GPIO_PIN_12
#define led2_GPIO_Port GPIOA
#define led1_Pin GPIO_PIN_15
#define led1_GPIO_Port GPIOA
#define TX4_Pin GPIO_PIN_10
#define TX4_GPIO_Port GPIOC
#define RX4_Pin GPIO_PIN_11
#define RX4_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOC
#define AC220_1_Pin GPIO_PIN_2
#define AC220_1_GPIO_Port GPIOD
#define AC220_2_Pin GPIO_PIN_3
#define AC220_2_GPIO_Port GPIOB
#define AC220_3_Pin GPIO_PIN_4
#define AC220_3_GPIO_Port GPIOB
#define AC220_4_Pin GPIO_PIN_5
#define AC220_4_GPIO_Port GPIOB
#define ZERO_Pin GPIO_PIN_6
#define ZERO_GPIO_Port GPIOB
#define ZERO_EXTI_IRQn EXTI9_5_IRQn
#define PWM1_Pin GPIO_PIN_8
#define PWM1_GPIO_Port GPIOB
#define PWM2_Pin GPIO_PIN_9
#define PWM2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TX1_485 HAL_GPIO_WritePin(DR1_485_GPIO_Port,DR1_485_Pin,GPIO_PIN_SET)
#define RX1_485 HAL_GPIO_WritePin(DR1_485_GPIO_Port,DR1_485_Pin,GPIO_PIN_RESET)

#define TX2_485 HAL_GPIO_WritePin(DR2_485_GPIO_Port,DR3_485_Pin,GPIO_PIN_SET)
#define RX2_485 HAL_GPIO_WritePin(DR2_485_GPIO_Port,DR3_485_Pin,GPIO_PIN_RESET)

#define TX3_485 HAL_GPIO_WritePin(DR3_485_GPIO_Port,DR3_485_Pin,GPIO_PIN_SET)
#define RX3_485 HAL_GPIO_WritePin(DR3_485_GPIO_Port,DR3_485_Pin,GPIO_PIN_RESET)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
