#include "init.h"
#include "Driver_StateMachine.h"
WorkState_e workState = PREPARE_STATE; 			//初始工作状态为准备状态
//uint32_t time_tick_ms = 0;


//u16 Systeam_State=0;    //用于控制底盘状态输出
//u16 Systeam_State_1=0;  //用于控制底盘状态输出
//u16 Systeam_OutState=0;  //底盘执行反馈状态
//u16 Systeam_OutState_1;
//u8  Display_State=0;     //裁判系统显示状态

///**********************************************************
//*工作状态切换状态机
//只能在正常模式下进行模式切换，在停止模式下 只能到准备模式 
//**********************************************************/

//void WorkStateFSM(void)
//{
//	switch(workState)
//	{
//		case PREPARE_STATE:	     //上电后初始化状态 4s钟左右
//		{
//			if(GetInputMode() == STOP)
//			{
//				workState = STOP_STATE;   //停止运动状态
//			}
//			else if(time_tick_ms > PREPARE_TIME_TICK_MS)
//			{
//				workState = NORMAL_STATE; 	//无输入状态
//			}			 
//		}break;
//		case NORMAL_STATE:   	//无输入状态
//		{
//     				
//								
//		}break;
//		case BUFF_STATE:       //神符状态
//		{
//	
//			
//		}break ;
//		case ERR_STATE:      //错误状态
//		{	    
//			 if(SysErrorStatus==0)
//			 {
//				ControtLoopTaskInit();
//				RemoteTaskInit();

//			 }
//			
//		}break;
//		case STOP_STATE:     //停止运动状态
//		{

//			if(GetInputMode() != STOP)
//			{
//				ControtLoopTaskInit();
//				RemoteTaskInit();
//			}
//		}break;    	  
//		default:
//		{
//			
//		}
//	}	
//}
//设置工作状态
void SetWorkState(WorkState_e state)
{
    workState = state;
}
//获取工作状态
WorkState_e GetWorkState()
{
	return workState;
}


