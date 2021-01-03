#include "data_process_task.h"
extern motor_measure_t motor_friction[2],motor_chassis[4];
//编码器处理变量
Encoder_process_t Encoder_chassis[4],Encoder_friction[2];


void Data_process_task(void  *pvParameters)
{
	static uint8_t i;
	while(1)
	{
		EncoderProcess3508(&Encoder_friction[0] ,&motor_friction[0]);
		EncoderProcess3508(&Encoder_friction[1] ,&motor_friction[1]);
		for(i = 0; i < 4; i++)
		{
		EncoderProcess3508(&Encoder_chassis[i] ,&motor_chassis[i]);
		}
	vTaskDelay(1);
	}
}
/*
***********************************************************************************************
*Name          :EncoderProcess
*Input         :can message
*Return        :void
*Description   :to get the initiatial encoder of the chassis motor 201 202 203 204
***********************************************************************************************
*/
/*
  采用向量法进行编码器总和的值的计算
*/
void EncoderProcess3508(Encoder_process_t* v ,motor_measure_t *motor)
{
	int32_t temp_sum = 0; 
		v->diff=motor->ecd-motor->last_ecd;
		if(v->diff < -6500)    //两次编码器的反馈值差别太大，表示圈数发生了改变
		{
			v->round_cnt++;
			v->ecd_raw_rate = v->diff + 8192;
		}
		else if(v->diff > 6500)
		{
			v->round_cnt--;
			v->ecd_raw_rate = v->diff- 8192;
		}
		else
		{
			v->ecd_raw_rate = v->diff;
		}
		//计算得到连续的编码器输出值
	     v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//计算得到角度值，范围正负无穷大
	     v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192 + v->round_cnt * 360;
		v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
		if(v->buf_count == RATE_BUF_SIZE)
		{
			v->buf_count = 0;
		}
		//计算速度平均值
		for(uint8_t i = 0;i < RATE_BUF_SIZE; i++)
		{
			temp_sum += v->rate_buf[i];
		}

		v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);		
}


//返回底盘电机编码器变量地址，通过指针方式获取原始数据
const Encoder_process_t *get_Chassis_Encoder_Measure_Point(uint8_t i)
{
    return &Encoder_chassis[(i & 0x03)];
}

