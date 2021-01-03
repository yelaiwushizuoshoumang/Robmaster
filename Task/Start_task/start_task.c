#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "FreeRTOSConfig.h"
#include "Chassis_task.h"
#include "Control_task.h"
#include "data_process_task.h"
#include "Monitor_Task.h"
#include "RemoteTask.h"

#define START_TASK_PRIO		1
#define START_STK_SIZE 		128  
static TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define Control_Thread_PRIO    2
#define Control_Thread_Size   512
static TaskHandle_t Control_Handler;//控制任务
 void Control_Task(void *pvParameters);
 
#define Monitor_Thread_PRIO    3
#define Monitor_Thread_Size   256
static TaskHandle_t Monitor_Handler;//监测任务

#define CHASSIS_TASK_PRIO 4
#define CHASSIS_TASK_SIZE 512
static TaskHandle_t CHASSISTask_Handler;//底盘任务
void chassis_task(void *pvParameters);

#define DATA_PROCESS_TASK_PRIO 5
#define DATA_PROCESS_TASK_SIZE 256
static TaskHandle_t DATA_PROCESS_TASK_Handler;//编码器数据处理

#define Remote_TASK_PRIO 6
#define Remote_TASK_SIZE 256
static TaskHandle_t Remote_TASK_Handler;
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	xTaskCreate((TaskFunction_t )Control_Task, 
			  (const char*    )"Control_Task", 
		       (uint16_t       )Control_Thread_Size, 
			  (void*          )NULL, 
			  (UBaseType_t    )Control_Thread_PRIO,  
	            (TaskHandle_t*  )&Control_Handler);
	xTaskCreate((TaskFunction_t )chassis_task, 
				(const char*    )"chassis_task", 
				(uint16_t       )CHASSIS_TASK_SIZE, 
				(void*          )NULL, 
				(UBaseType_t    )CHASSIS_TASK_PRIO,  
	             			(TaskHandle_t*  )&CHASSISTask_Handler);
	xTaskCreate((TaskFunction_t )Monitor_Task,
				(const char*    )"Monitor_Task", 
				(uint16_t       )Monitor_Thread_Size, 
				(void*          )NULL, 
				(UBaseType_t    )Monitor_Thread_PRIO,  
				(TaskHandle_t*  )&Monitor_Handler);			
	xTaskCreate((TaskFunction_t )Data_process_task, 
				(const char*    )"Data_process_task", 
				(uint16_t       )DATA_PROCESS_TASK_SIZE, 
				(void*          )NULL, 
				(UBaseType_t    )DATA_PROCESS_TASK_PRIO,  
				(TaskHandle_t*  )&DATA_PROCESS_TASK_Handler);
	xTaskCreate((TaskFunction_t )Remote_task, 
				(const char*    )"Remote_task", 
				(uint16_t       )Remote_TASK_SIZE, 
				(void*          )NULL, 
				(UBaseType_t    )Remote_TASK_PRIO,  
				(TaskHandle_t*  )&Remote_TASK_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

/*
*********************************************************************************************************
* StartTask_Handler
*********************************************************************************************************
*/
void startTask(void)   
{
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
