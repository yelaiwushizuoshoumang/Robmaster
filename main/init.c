#include "init.h"                  // Device header
#include "arm_math.h"
#include "USART_INIT.h"


TaskHandle_t CAN1RxTask_Handler;
TaskHandle_t CAN1TxTask_Handler;
TaskHandle_t CAN2RxTask_Handler;
TaskHandle_t CAN2TxTask_Handler;

void Init_SystemAndTask(void)
{
    {
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
        CAN_mode_init();
        led_configuration();
				GPIO_Init_Configuration();
        Device_Usart4_ENABLE_Init(115200);
//			Device_Usart1_ENABLE_Init(100000);
				remote_control_init();

	{
				startTask();
	    	vTaskStartScheduler();
        osKernelStart();  //Start the RTOS Kernel
      }

    }

}

