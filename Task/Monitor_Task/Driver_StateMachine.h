#ifndef __DRIVER_STATUSMACHINE_H
#define __DRIVER_STATUSMACHINE_H

#include "stm32f4xx.h"

#define PREPARE_TIME_TICK_MS 1000      //prapare time in ms
#define YAW_PITCH_PREPARE_TIME_TICK_MS 500      //prapare time in ms
#define STATE_SWITCH_DELAY_TICK 100000   //mode change delay count in ms

typedef enum
{
    PREPARE_STATE=0,     		//上电后初始化状态 4s钟左右
    NORMAL_STATE,					//无输入状态
	  BUFF_STATE ,           //神符状态
	  ERR_STATE,            //错误状态
    STOP_STATE,        		//停止运动状态
}WorkState_e;

extern u16 Systeam_State;
extern u16 Systeam_OutState;
extern u16 Systeam_OutState_1;
extern u8  Display_State;
extern u16 Systeam_State_1;
void WorkStateFSM(void);
void SetWorkState(WorkState_e state);
WorkState_e GetWorkState(void);

#endif
