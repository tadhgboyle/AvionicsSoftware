/**
  ******************************************************************************
  * File Name          : WDGTask.cpp
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Data.h"
#include "DebugTask.hpp"
#include "Task.hpp"
#include <time.h>
#include "WDGTask.hpp"
#include "GPIO.hpp"

extern IWDG_HandleTypeDef hiwdg;

/* Macros --------------------------------------------------------------------*/

/* Structs -------------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Prototypes ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/**
 * @brief Default constructor, sets and sets up storage for member variables
 */
WDGTask::WDGTask() : Task(TASK_IWDG_QUEUE_DEPTH_OBJS)
{
}

/**
 * @brief Creates a task for the FreeRTOS Scheduler
 */
void WDGTask::InitTask()
{
    // Make sure the task is not already initialized
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize PT task twice");

    // Start the task
    BaseType_t rtValue =
        xTaskCreate((TaskFunction_t)WDGTask::RunTask,
            (const char*)"WDGTask",
            (uint16_t)TASK_IWDG_STACK_DEPTH_WORDS,
            (void*)this,
            (UBaseType_t)TASK_IWDG_PRIORITY,
            (TaskHandle_t*)&rtTaskHandle);

    //Ensure creation succeeded
    SOAR_ASSERT(rtValue == pdPASS, "WDGTask::InitTask() - xTaskCreate() failed");
}

/**
 * @brief 
 * @param pvParams 
 */
void WDGTask::Run(void * pvParams)
{
	while(1) {
		HAL_IWDG_Refresh(&hiwdg);
	}
}

/**
 * @brief Handles a command
 * @param cm Command reference to handle
 */
void WDGTask::HandleCommand(Command& cm)
{
    //TODO: Since this task will stall for a few milliseconds, we may need a way to eat the whole queue (combine similar eg. REQUEST commands and eat to WDG command etc)
    //TODO: Maybe a HandleEvtQueue instead that takes in the whole queue and eats the whole thing in order of non-blocking to blocking

    //Switch for the GLOBAL_COMMAND
    switch (cm.GetCommand()) {
    case REQUEST_COMMAND: {
        HandleRequestCommand(cm.GetTaskCommand());
    }
    case TASK_SPECIFIC_COMMAND: {
        break;
    }
    default:
        SOAR_PRINT("WDGTask - Received Unsupported Command {%d}\n", cm.GetCommand());
        break;
    }

    //No matter what we happens, we must reset allocated data
    cm.Reset();
}

/**
 * @brief Handles a Request Command
 * @param taskCommand The command to handle
 */
void WDGTask::HandleRequestCommand(uint16_t taskCommand)
{
    //Switch for task specific command within DATA_COMMAND
    switch (taskCommand) {
    default:
        SOAR_PRINT("UARTTask - Received Unsupported REQUEST_COMMAND {%d}\n", taskCommand);
        break;
    }
}
