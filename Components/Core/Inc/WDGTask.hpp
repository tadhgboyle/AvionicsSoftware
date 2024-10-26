/**
 ******************************************************************************
 * File Name          : WDGTask.hpp
 * Description        :
 ******************************************************************************
*/
#ifndef WDG_TASK_HPP_
#define WDG_TASK_HPP_

/* Includes ------------------------------------------------------------------*/
#include "Task.hpp"
#include "Data.h"
#include "SystemDefines.hpp"


/* Macros/Enums ------------------------------------------------------------*/

/* Class ------------------------------------------------------------------*/
class WDGTask : public Task
{
public:
    static WDGTask& Inst() {
        static WDGTask inst;
        return inst;
    }

    void InitTask();

protected:
    static void RunTask(void* pvParams) { WDGTask::Inst().Run(pvParams); } // Static Task Interface, passes control to the instance Run();

    void Run(void* pvParams);    // Main run code

    void HandleCommand(Command& cm);
    void HandleRequestCommand(uint16_t taskCommand);

private:
    WDGTask();                                        // Private constructor
    WDGTask(const WDGTask&);                    // Prevent copy-construction
    WDGTask& operator=(const WDGTask&);            // Prevent assignment
};

#endif    // WDG_TASK_HPP_
