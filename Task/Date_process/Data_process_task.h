#ifndef _DATA_PROCESS_TASK_H_
#define _DATA_PROCESS_TASK_H_
#include "CAN_Receive.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define  RATE_BUF_SIZE 6
typedef struct
{
	int32_t raw_value;   									//编码器不经处理的原始值
	int32_t last_raw_value;								//上一次的编码器原始值
	int32_t ecd_value;    
	int32_t diff;	
	int32_t ecd_bias;											//初始编码器值	
	int32_t ecd_raw_rate;		
	int32_t round_cnt;
	int32_t rate_buf[RATE_BUF_SIZE]; 	//buf，for filter
	uint8_t buf_count;					//滤波更新buf用
	int32_t filter_rate;				//速度
	float ecd_angle;											//角度

} Encoder_process_t;




void Data_process_task(void  *pvParameters);
/*
  采用向量法进行编码器总和的值的计算
*/
extern void EncoderProcess3508(Encoder_process_t* v ,motor_measure_t *motor);
//返回底盘电机编码器变量地址，通过指针方式获取原始数据
extern const Encoder_process_t *get_Chassis_Encoder_Measure_Point(uint8_t i);
//返回摩擦电机编码器变量地址，通过指针方式获取原始数据
//extern const Encoder_process_t *get_Friction_Encoder_Measure_Point(uint8_t i);
//返回顶部电机数据
extern const Encoder_process_t *get_Top_Motor_Encoder_Measure_Point(void);
#endif /*_DATA_PROCESS_TASK_H_*/


