/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

volatile uint32_t ir_on = 0;
volatile float t_got_0 = 0;
volatile float t_got_1 = 0;

volatile uint32_t state = 0;
volatile uint32_t data = 0;
volatile uint32_t bit_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  // WARNING: THIS IS THE NEC PROTOCOL BUT INVERTED!
  // -> I DONT KNOW WHY BUT IT IS, THE LINE IS HIGH IN IDLE ...

  // UP:    0xff00ef00
  // DOWN:  0xfe01ef00
  // OFF:   0xfd02ef00
  // ON:    0xfc03ef00
  // RED:   0xfb04ef00
  // GREEN: 0xfa05ef00
  // BLUE:  0xf906ef00
  // WHITE: 0xf807ef00
  // LILA:  0xf10eef00
  // PINK:  0xe916ef00
  // FADE:  0xec13ef00

  if(__HAL_TIM_GET_IT_SOURCE(&htim1, TIM_IT_CC1) == SET) {

	  if (htim1.Instance->CCER == 1u << TIM_CHANNEL_1) {
		  t_got_1 = htim1.Instance->CCR1 / 1000.0f;
		  // if rising

		  if (htim1.Instance->CCR1 > 8500) {
			  state = 1;
		  } else if (state == 2) {
			  if (bit_counter == 32) {
				  state = 0;
			  } else {
				  state = 3;
			  }
		  }

		  htim1.Instance->CNT = 0;
		  htim1.Instance->CCER = 3u << TIM_CHANNEL_1;
	  } else {
		  t_got_0 = htim1.Instance->CCR1 / 1000.0f;
		  // if falling

		  if (state == 1 && htim1.Instance->CCR1 > 4000) {
			  state = 2;
			  bit_counter = 0;
			  data = 0;
		  } else if (state == 3) {

			 // read 1
			  if (htim1.Instance->CCR1 > 1000) {
				  data = data | (1u << bit_counter);
			  }

			  state = 2;
			  bit_counter++;
		  }

		  htim1.Instance->CNT = 0;
		  htim1.Instance->CCER = 1u << TIM_CHANNEL_1;
	  }
  }

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
