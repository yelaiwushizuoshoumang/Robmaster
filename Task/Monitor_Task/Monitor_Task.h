#ifndef _MONITOR_TASK_H_
#define _MONITOR_TASK_H_

#include "stm32f4xx.h"                  // Device header
typedef struct SystemMonitor{
  u16 DBUSFrameCounter;  //遥控信号帧
	u16 PitchFrameCounter;  //云台电机帧
  u16 YawFrameCounter;
	u16 ChassisFrameCounter_1;  //底盘电机帧
  u16 ChassisFrameCounter_2;
	u16 ChassisFrameCounter_3;
	u16 ChassisFrameCounter_4;

//  u16 IMUFrameCounter;        //姿态数据帧
}SystemMonitor;


void Monitor_Task(void const *argument);
extern SystemMonitor Real_Monitor;
extern uint16_t SysErrorStatus ;

#endif
