#ifndef _INIT_H_
#define _INIT_H_
/*=================OS=====================*/
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "cmsis_os.h"
/*================BSP====================*/
#include "BSP_CAN1_Init.h"
#include "BSP_CAN2_Init.h"
#include "BSP_GPIO_Init.h"
#include "USART_INIT.h"
#include "CAN.h"
#include "rc.h"
#include "led.h"
/*================TASK=================*/
#include "CAN_Receive.h"
#include "Chassis_task.h"
#include "data_process_task.h"
#include "Control_Task.h"
#include "Monitor_Task.h"
#include "RemoteTask.h"
/*===============MATH====================*/
#include "pid.h"

/*=======================================*/
void delay_ms(unsigned int t);
void delay_us(unsigned int t);
void Init_SystemAndTask(void);
#endif


