#include "Monitor_Task.h"
#include "FreeRTOS.h"
#include "Driver_StateMachine.h"
#include "stm32f4xx.h"  
#include "BSP_GPIO_Init.h"
#include <string.h>
#include "task.h"
// Device header

SystemMonitor Real_Monitor;  //计数帧  
SystemMonitor Rate_Monitor;  //速率帧  
/**
  * @brief  监视器任务
  * @param  unused
  * @retval voif
  */

 uint16_t SysErrorStatus = 0;   
void Monitor_Task(void const *argument)
{
//系统状态，每一位对应一个错误，0正常，1错误
/****************************************************************
    15  |   14  |   13  |   12  |   11  |   10 |   9   |   8     |
        |       |       |       |  拨弹 |  IMU | PC视觉 | 裁判系统|
-----------------------------------------------------------------
    7   |   6   |   5   |   4      |   3      |   2   |   1   |   0   |
底盘电流|右后底盘|左后底盘|左前底盘| 右前底盘 |  Yaw  | Pitch |  DBUS |
****************************************************************/
	

	while(1)
	{
		
		memcpy(&Rate_Monitor,&Real_Monitor,sizeof(SystemMonitor));
		memset(&Real_Monitor,0,sizeof(SystemMonitor));   //将结构体内数据清零
	
		//DBUS帧率过低
		if(Rate_Monitor.DBUSFrameCounter<12)
		{
		  SysErrorStatus|=(1<<0);
		}
		else
		{
		  SysErrorStatus&=~(1<<0);
		}
		 //Pitch云台帧率过低
		if(Rate_Monitor.PitchFrameCounter<30)
		{
		  SysErrorStatus|=(1<<1);
		}
		else
		{
		  SysErrorStatus&=~(1<<1);
		}
		//Yaw云台帧率过低
		if(Rate_Monitor.YawFrameCounter<30)
		{
		  SysErrorStatus|=(1<<2);
		}
		else
		{
		  SysErrorStatus&=~(1<<2);
		}
		//1号电机 
		if(Rate_Monitor.ChassisFrameCounter_1<30)
		{
		  SysErrorStatus|=(1<<3);
		}
		else
		{
		  SysErrorStatus&=~(1<<3);
		}
		//2号电机 
		if(Rate_Monitor.ChassisFrameCounter_2<30)
		{
		  SysErrorStatus|=(1<<4);
		}
		else
		{
		  SysErrorStatus&=~(1<<4);
		}
		//3号电机 
		if(Rate_Monitor.ChassisFrameCounter_3<30)
		{
		  SysErrorStatus|=(1<<5);
		}
		else
		{
		  SysErrorStatus&=~(1<<5);
		}
		//4号电机 
		if(Rate_Monitor.ChassisFrameCounter_4<30)
		{
		  SysErrorStatus|=(1<<6);
		}
		else
		{
		  SysErrorStatus&=~(1<<6);
		}


//		//IMU输出帧
//		if(Rate_Monitor.IMUFrameCounter<12)
//		{
//		  SysErrorStatus|=(1<<10);
//		}
//	  else
//		{
//		  SysErrorStatus&=~(1<<10);
//		}
	
		if(SysErrorStatus)  //有错误帧数据出现
		{	
			LED2_ON;
 			SetWorkState(ERR_STATE);	//设置工作模式
		}
		else
		{
		  LED3_OFF;
		}
	 vTaskDelay(10);
	}

}	
	
	
	
	



