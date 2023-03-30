/**
 ******************************************************************************
 * File Name          : TimerTransitions.cpp
 * Description        : Primary Watchdog task, default task for the system.
 ******************************************************************************
*/
#ifndef SOAR_TIMERTRANSITIONS_HPP_
#define SOAR_TIMERTRANSITIONS_HPP_
#include "Task.hpp"
#include "SystemDefines.hpp"
#include "RocketSM.hpp"
#include "Timer.hpp"

/* Macros/Enums ------------------------------------------------------------*/
constexpr uint32_t IGINITION_TIMER_PERIOD = 10000;
constexpr uint32_t IR_IGINITION_TIMER_PERIOD = 8000;
enum TIMERTRANSITION_CONTROLS  {
    CONFIRM_LAUNCH_2 = 0,
};

class TimerTransitions
{
public:
	static TimerTransitions& Inst() {
		static TimerTransitions inst;
		return inst;
	}
	TimerTransitions();
	void IgnitionSequence();
	void IRSequence();
	bool ignitionConfirmation = false;

protected:
	static void IngnitionToLaunchCallback(TimerHandle_t rtTimerHandle);

private:
	Timer* ignitionCountdown;
};

#endif    // SOAR_TIMERTRANSITIONS_HPP_
