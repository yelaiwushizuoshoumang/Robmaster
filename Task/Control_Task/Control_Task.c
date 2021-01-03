/**
  ************************************* Copyright ******************************
  *
  *
  * FileName   : Control_Task.c
  * Version    : v1.0
  * Author     : Ars Nova
  * Date       : 2020-12-24
  * Description:
  * Function List:
  	1. ....
  	   <version>:
  <modify staff>:
  		  <data>:
   <description>:
  	2. ...
  ******************************************************************************
 */

#include "Control_Task.h"
#include "BSP_GPIO_Init.h"
#include "data_process_task.h"
#include "CAN_Receive.h"
#include "stm32f4xx_can.h"
#include "start_task.h"
#include "stm32f4xx.h"                  // Device header
#define uchar unsigned char
	
Top_WorkState   Top_Work_State = ResetInit_State;//工作转态

static void Reset_State(void);
//static void Move_Choose_State(void);



//static void Capture_Task_Pro(void);//一级夹取



extern void Control_Task(void *pvParameters)
{
		D1_ON;
    vTaskDelay(10);
    Reset_State();
    while(1)
    {
 
		

	
	
    }

}
/*
====+=====================整体复位==============================
*/
static void Reset_State(void)
{
//    AC_TF_Channel_3_OFF; //旋转夹手..
//    AC_TF_Channel_4_OFF;//夹手开关..
//    AC_TF_Channel_5_OFF;//弹药箱复位
//    AC_TF_Channel_2_OFF;//夹手缩回..
//    vTaskDelay(10);
//    AC_TF_Channel_6_OFF; //整体下降
}
/*
=================================================================
*/
/*
=========================夹取任务================================
*/
/*===============一级夹取===============*/
//static void Capture_Task_Pro(void)
//{
//    AC_TF_Channel_6_ON;
//	
//    vTaskDelay(10);
//    AC_TF_Channel_3_ON;
//    AC_TF_Channel_4_ON;
//    AC_TF_Channel_3_OFF;
//    AC_TF_Channel_5_ON;
//    vTaskDelay(1);
//}

