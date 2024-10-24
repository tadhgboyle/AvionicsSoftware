/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : IWDG.cpp
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
/* Includes ------------------------------------------------------------------*/
#include "SystemDefines.hpp"
#include "IWDGTask.hpp"
#include "GPIO.hpp"
extern IWDG_HandleTypeDef hiwdg;

//static void MX_IWDG_Init(void);

int refreshIntervalValue = 9000;
int* refreshInterval = &refreshIntervalValue;

void IWDGTask::InitTask()
{
	//MX_IWDG_Init(); // initialize watchdog
	GPIO::LED1::Off();
	GPIO::LED2::Off();
	GPIO::LED3::Off();

    // Start the task
    BaseType_t rtValue =
        xTaskCreate((TaskFunction_t)IWDGTask::RunTask,
            (const char*)"IWDGTask",
            (uint16_t)TASK_IWDG_STACK_DEPTH_WORDS,
			(void*)&refreshInterval,
            (UBaseType_t)TASK_IWDG_PRIORITY,
            (TaskHandle_t*)&rtTaskHandle);

    //Ensure creation succeeded
    SOAR_ASSERT(rtValue == pdPASS, "IWDGTask::InitTask() - xTaskCreate() failed");

    Run(NULL);
}

void IWDGTask::Run(void * pvParams)
{
    //IWDG Task loop
    while(1) {
    	if (HAL_IWDG_Init(&hiwdg) != HAL_OK){
    		GPIO::LED1::On();	//LED1 does not turn on
    		Error_Handler();
    	}
    	GPIO::LED2::On(); 		//LED2 turns on
    	//GPIO::LED3::Off();
    	HAL_Delay(1000);
    	GPIO::LED2::Off();		//LED2 does not turn off
    }
}

//static void MX_IWDG_Init(void)
//{
//  hiwdg.Instance = IWDG;
//  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
//  hiwdg.Init.Reload = 624;
//  if (HAL_IWDG_Init(&hiwdg) != HAL_OK){
//	  GPIO::LED1::Off();
//	  GPIO::LED2::Off();
//	  GPIO::LED3::Off();
//	  Error_Handler();
//  }
//  }
