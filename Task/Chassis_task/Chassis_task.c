#include "main.h"
#include "Chassis_task.h"
#include "BSP_GPIO_Init.h"
//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 357
//拨杆 S1 S2
#define ModeChannel_R 	0
#define ModeChannel_L 	1
//底盘运动数据
chassis_move_t chassis_move;
PidTypeDef PID;
static void chassis_init(chassis_move_t *chassis_move_init);//底盘初始化
static void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode);//底盘模式设置
static void chassis_feedback_update(chassis_move_t *chassis_move_mode);//反馈数据的更新
static void mecanum_calc(float vx, float vy, float vz, int16_t speed[]);//麦克纳姆轮的计算
static void chassis_give_current(chassis_move_t *chassis_current);//电流值
static void chassis_Behaviour_update(chassis_move_t *chassis_behaviour_update);//行为更新
extern void chassis_task(void *pvParameters)
{
    //空闲一段时间
   vTaskDelay(CHASSIS_TASK_INIT_TIME);
	//底盘初始化
   chassis_init(&chassis_move);
	 D2_ON;
	 chassis_move.chassis_mode = CHASSIS_SEPARATE_GIMBAL ;
   while (1)
   {
   chassis_behaviour_mode_set(&chassis_move);
   chassis_feedback_update(&chassis_move);
   chassis_Behaviour_update(&chassis_move);
	   
	if(chassis_move.chassis_mode==CHASSIS_RELAX)
	{
		CAN_CMD_CHASSIS(0, 0, 0, 0);
	}
	else
	{
		CAN_CMD_CHASSIS(chassis_move.motor_chassis[0].give_current, chassis_move.motor_chassis[1].give_current,
		chassis_move.motor_chassis[2].give_current, chassis_move.motor_chassis[3].give_current);
	}
   vTaskDelay(10);  //系统延时
   }
 }

static void chassis_init(chassis_move_t *chassis_move_init)
{    //底盘速度环pid值
    const static fp32 motor_speed_pid[3] = {M3505_MOTOR_SPEED_PID_KP, M3505_MOTOR_SPEED_PID_KI, M3505_MOTOR_SPEED_PID_KD};
	 //底盘旋转环pid值
    const static fp32 chassis_yaw_pid[3] = {CHASSIS_FOLLOW_GIMBAL_PID_KP, CHASSIS_FOLLOW_GIMBAL_PID_KI, CHASSIS_FOLLOW_GIMBAL_PID_KD};
	if (chassis_move_init == NULL)
    {
        return;
    }
	uint8_t i;
   
	//底盘开机状态为停止
    chassis_move_init->chassis_mode = CHASSIS_RELAX;
	//获取遥控器指针
    chassis_move_init->chassis_rc_ctrl = get_remote_control_point();
	//获取电机和编码器数据
    for (i = 0; i < 4; i++)
    {
     chassis_move_init->motor_chassis[i].chassis_motor_measure = get_Chassis_Motor_Measure_Point(i);
	   chassis_move_init->motor_chassis[i].chassis_encoder = get_Chassis_Encoder_Measure_Point(i);
	   PID_Init(&chassis_move_init->motor_speed_pid[i], PID_POSITION, motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT, M3505_MOTOR_SPEED_PID_MAX_IOUT);
    }
    //初始化旋转PID
    PID_Init(&chassis_move_init->chassis_angle_pid, PID_POSITION, chassis_yaw_pid, CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT, CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT);
}
static void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
	
    if (chassis_move_mode == NULL)
    {
        return;
    }
	//双底                  
	if(switch_is_down(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_R])&&switch_is_down(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_L]))
	{
	chassis_move_mode->chassis_mode=CHASSIS_INIT;
	}
	//双顶
	else if (switch_is_up(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_R])&&switch_is_up(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_L]))
	{
	chassis_move_mode->chassis_mode=CHASSIS_RELAX;
	}
	//双中
	else if (switch_is_mid(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_R])&&switch_is_mid(chassis_move_mode->chassis_rc_ctrl->rc.s[ModeChannel_L]))
	{
	chassis_move_mode->chassis_mode=CHASSIS_SEPARATE_GIMBAL;
	}
//	else
//	{
//	chassis_move_mode->chassis_mode=CHASSIS_RELAX;
//	}

}
static void chassis_feedback_update(chassis_move_t *chassis_move_update)
{
	if (chassis_move_update == NULL)
    {
        return;
    }
	//速度反馈
	chassis_move_update->wheel_spd_fdb[0]=chassis_move_update->motor_chassis[0].chassis_encoder->filter_rate;
	chassis_move_update->wheel_spd_fdb[1]=chassis_move_update->motor_chassis[1].chassis_encoder->filter_rate;
	chassis_move_update->wheel_spd_fdb[2]=chassis_move_update->motor_chassis[2].chassis_encoder->filter_rate;
	chassis_move_update->wheel_spd_fdb[3]=chassis_move_update->motor_chassis[3].chassis_encoder->filter_rate;
	//遥控器数据更新
	chassis_move_update->RC_X_ChassisSpeedRef = chassis_move_update->chassis_rc_ctrl->rc.ch[CHASSIS_X_CHANNEL]/RC_RESOLUTION*CHASSIS_RC_MAX_SPEED_X;//左右
	chassis_move_update->RC_Y_ChassisSpeedRef = chassis_move_update->chassis_rc_ctrl->rc.ch[CHASSIS_Y_CHANNEL]/RC_RESOLUTION*CHASSIS_RC_MAX_SPEED_Y;//前后
	chassis_move_update->RC_W_ChassisSpeedRef = chassis_move_update->chassis_rc_ctrl->rc.ch[CHASSIS_W_CHANNEL]/RC_RESOLUTION*CHASSIS_RC_MAX_SPEED_W;//控制旋转
	chassis_move_update->RC_Z_ChassisSpeedRef = chassis_move_update->chassis_rc_ctrl->rc.ch[CHASSIS_Z_CHANNEL]/RC_RESOLUTION*CHASSIS_RC_MAX_SPEED_Z;//

}
static void chassis_Behaviour_update(chassis_move_t *chassis_behaviour_update)
{
	uint8_t i=0;
//	fp32 sin_yaw = 0.0f, cos_yaw = 0.0f;

	if (chassis_behaviour_update == NULL)
    {
        return;
    }
	if(chassis_behaviour_update->chassis_mode==CHASSIS_RELAX){//底盘停止
	for(i = 0; i < 4; i++)
	{	
	chassis_move.motor_chassis[i].give_current=0;}
	}
	else if(chassis_behaviour_update->chassis_mode==CHASSIS_INIT)//底盘初始化
	{
	chassis_behaviour_update->vx=0;
	chassis_behaviour_update->vy=0;
	chassis_behaviour_update->vz=0;
     mecanum_calc(chassis_behaviour_update->vx,chassis_behaviour_update->vy,chassis_behaviour_update->vz,chassis_behaviour_update->wheel_spd_ref);
	chassis_give_current(chassis_behaviour_update);
	}
	else if(chassis_behaviour_update->chassis_mode==CHASSIS_FOLLOW_GIMBAL)//底盘跟随云台
	{
	chassis_behaviour_update->vx = chassis_behaviour_update->RC_X_ChassisSpeedRef;
	chassis_behaviour_update->vy = chassis_behaviour_update->RC_Y_ChassisSpeedRef;
	chassis_behaviour_update->vz = chassis_behaviour_update->RC_W_ChassisSpeedRef;
	mecanum_calc(chassis_behaviour_update->vx,chassis_behaviour_update->vy,chassis_behaviour_update->vz,chassis_behaviour_update->wheel_spd_ref);
	chassis_give_current(chassis_behaviour_update);
	}
	else{
	for(i = 0; i < 4; i++)
	{	
	chassis_move.motor_chassis[i].give_current=0;}
	}
	for (i = 0; i < 4; i++)
    {
	PID_Calc(&chassis_behaviour_update->motor_speed_pid[i], chassis_behaviour_update->wheel_spd_fdb[i], chassis_behaviour_update->wheel_spd_ref[i]);
	}
} 
 
static void chassis_give_current(chassis_move_t *chassis_current)    //赋值电流值,电流值控制电机
{
   chassis_current->motor_chassis[0].give_current = (int16_t)(chassis_current->motor_speed_pid[0].out);
	 chassis_current->motor_chassis[1].give_current = (int16_t)(chassis_current->motor_speed_pid[1].out);
	 chassis_current->motor_chassis[3].give_current = (int16_t)(chassis_current->motor_speed_pid[3].out);
	 chassis_current->motor_chassis[2].give_current = (int16_t)(chassis_current->motor_speed_pid[2].out);
    
}
/*==========================================================================================*/
/*
麦克纳姆的解算,利用全向算法
*/
static void mecanum_calc(float vx, float vy, float vz, int16_t speed[])
{
static float rotate_ratio_fr;//前右
static float rotate_ratio_fl;//前左
static float rotate_ratio_bl;//后左
static float rotate_ratio_br;//后右
static float wheel_rpm_ratio;

  rotate_ratio_fr = ((WHEELBASE+WHEELTRACK)/2.0f \
                      - chassis_move.rotate_x_offset + chassis_move.rotate_y_offset)/RADIAN_COEF;//(340+410)/2/57.3 = 6.54450274
  rotate_ratio_fl = ((WHEELBASE+WHEELTRACK)/2.0f \
                      - chassis_move.rotate_x_offset - chassis_move.rotate_y_offset)/RADIAN_COEF;
  rotate_ratio_bl = ((WHEELBASE+WHEELTRACK)/2.0f \
                      + chassis_move.rotate_x_offset - chassis_move.rotate_y_offset)/RADIAN_COEF;
  rotate_ratio_br = ((WHEELBASE+WHEELTRACK)/2.0f \
                      + chassis_move.rotate_x_offset + chassis_move.rotate_y_offset)/RADIAN_COEF;

  wheel_rpm_ratio = 60.0f/(PERIMETER*CHASSIS_DECELE_RATIO); //	60/周长*减数比 = 2.38493729
  
  
//==============限幅函数=========================  
  VAL_LIMIT(vx, -MAX_CHASSIS_VX_SPEED, MAX_CHASSIS_VX_SPEED); //mm/s
  VAL_LIMIT(vy, -MAX_CHASSIS_VY_SPEED, MAX_CHASSIS_VY_SPEED); //mm/s
  VAL_LIMIT(vz, -MAX_CHASSIS_VR_SPEED, MAX_CHASSIS_VR_SPEED); //deg/s
 //==============================================
  int16_t wheel_rpm[4];
  float   max = 0;
  //全向算法
  wheel_rpm[0] = (+vx + vy - vz * rotate_ratio_fr) * wheel_rpm_ratio;
  wheel_rpm[1] = (+vx - vy - vz * rotate_ratio_fl) * wheel_rpm_ratio;
  wheel_rpm[2] = (-vx - vy - vz * rotate_ratio_bl) * wheel_rpm_ratio;
  wheel_rpm[3] = (-vx + vy - vz * rotate_ratio_br) * wheel_rpm_ratio;


  //find max item
  for (uint8_t i = 0; i < 4; i++)
  {
    if (abs(wheel_rpm[i]) > max)
      max = abs(wheel_rpm[i]);
  }
  //equal proportion
  if (max > MAX_WHEEL_RPM)
  {
    float rate = MAX_WHEEL_RPM / max;
    for (uint8_t i = 0; i < 4; i++)
      wheel_rpm[i] *= rate;
  }
  memcpy(speed, wheel_rpm, 4*sizeof(int16_t));
}
//编码器参数整定滤波

