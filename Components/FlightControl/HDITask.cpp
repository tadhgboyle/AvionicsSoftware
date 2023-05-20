/**
 ******************************************************************************
 * File Name          : HDITask.cpp
 * Description        : Primary flight task, default task for the system.
 ******************************************************************************
*/
#include "HDITask.hpp"
#include "GPIO.hpp"
#include "SystemDefines.hpp"
#include "RocketSM.hpp"
#include "FlightTask.hpp"


/**
 * @brief Constructor for FlightTask
 */

TIM_HandleTypeDef htim2;

HDITask::HDITask():Task(HDI_TASK_QUEUE_DEPTH_OBJS)
{
}

/**
 * @brief Initialize the FlightTask
 */
void HDITask::InitTask()
{
    // Make sure the task is not already initialized
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize flight task twice");

    BaseType_t rtValue =
        xTaskCreate((TaskFunction_t)HDITask::RunTask,
            (const char*)"HDITask",
            (uint16_t)HDI_TASK_STACK_DEPTH_WORDS,
            (void*)this,
            (UBaseType_t)HDI_TASK_RTOS_PRIORITY,
            (TaskHandle_t*)&rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "HDITask::InitTask() - xTaskCreate() failed");
}

/**
 * @brief Instance Run loop for the Flight Task, runs on scheduler start as long as the task is initialized.
 * @param pvParams RTOS Passed void parameters, contains a pointer to the object instance, should not be used
 */
void HDITask::Run(void * pvParams)
{
    uint32_t tempSecondCounter = 0; // TODO: Temporary counter, would normally be in HeartBeat task or HID Task, unless FlightTask is the HeartBeat task

    htim2.Instance = TIM2;
	htim2.Init.Prescaler = 952-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 210-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    uint8_t value = 0;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    GPIO::LED1::Off();

    while (1) {
    	//Every cycle, print something out (for testing)
    	SOAR_PRINT("FlightTask::Run() - [%d] Seconds\n", tempSecondCounter++);



    	RocketState currentHDIState = FlightTask::Inst().GetCurrentState();

        switch(currentHDIState){
        case RS_PRELAUNCH:
        	//GPIO::LED1::On();

        	while(value < 210){
        			  htim2.Instance -> CCR1 = value;
        			  value += 20;
        			  HAL_Delay (500);
        		  }
        	value = 0;
			HAL_GPIO_WritePin(GPIOC, HDI_PWM_BUZZER_Pin, GPIO_PIN_SET);


			osDelay(500);
			GPIO::LED1::Off();

			osDelay(500);
			break;

		case RS_ABORT:
			GPIO::LED2::On();
			//HAL_GPIO_WritePin(GPIOC, HDI_PWM_BUZZER, GPIO_PIN_RESET);

			osDelay(500);
			GPIO::LED2::Off();

			osDelay(500);
			break;

//		case RS_OPEN_VENT:
//			GPIO::LED3::On();
//
//			osDelay(500);
//			GPIO::LED3::Off();
//
//			osDelay(500);
//			break;
//
//		case RS_CLOSE_VENT:
//			GPIO::LED2::On();
//
//			osDelay(500);
//			GPIO::LED2::Off();
//
//			osDelay(500);
//			break;
//
//		case RS_OPEN_DRAIN:
//			GPIO::LED2::On();
//
//			osDelay(500);
//			GPIO::LED2::Off();
//
//			osDelay(500);
//			break;
//
//		case RS_CLOSE_DRAIN:
//			GPIO::LED2::On();
//
//			osDelay(500);
//			GPIO::LED2::Off();
//
//			osDelay(500);
//			break;

		default:

			GPIO::LED1::Off();
			GPIO::LED2::Off();
			GPIO::LED3::Off();

			HAL_GPIO_WritePin(GPIOC, HDI_PWM_BUZZER_Pin, GPIO_PIN_RESET);


		break;
	}


    }
}
