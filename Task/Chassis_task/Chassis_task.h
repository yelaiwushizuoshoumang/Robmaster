/**
  ****************************(C) COPYRIGHT 2020 NCIST****************************
  * @file       chassis.c/h
  * @brief      完成底盘控制任务。
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 NCIST****************************
  */
#ifndef _CHASSIS_TASK_H
#define _CHASSIS_TASK_H
#include "pid.h"
#include "CAN_Receive.h"
#include "start_task.h"
#include "RemoteTask.h"
#include "data_process_task.h"
#include "Monitor_Task.h"
//底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002

//左右的遥控器通道号码
#define CHASSIS_X_CHANNEL 0
//前后的遥控器通道号码
#define CHASSIS_Y_CHANNEL 1
//向左右转动的遥控器通道号码
#define CHASSIS_W_CHANNEL 2
//仰俯转动的遥控器通道号码
#define CHASSIS_Z_CHANNEL 3
//在特殊模式下，可以通过遥控器控制旋转
#define CHASSIS_WZ_CHANNEL 4  //波轮控制
//遥控器波轮（max 660）转化成车体左右速度（m/s）的比例
#define CHASSIS_VZ_RC_SEN 0.05f
/* normalized remote controller proportion */
#define RC_RESOLUTION     660.0f  //遥控器的比列
/* remote mode chassis move speed limit *///底盘线速
/* back and forward speed (mm/s) *///向前向后的速度
#define CHASSIS_RC_MAX_SPEED_X  660.0f
#define CHASSIS_RC_MOVE_RATIO_X 1.0f
/* left and right speed (mm/s) *///向左向右的速度
#define CHASSIS_RC_MAX_SPEED_Y  660.0f
#define CHASSIS_RC_MOVE_RATIO_Y 1.0f
//========================================
#define CHASSIS_RC_MAX_SPEED_W  660.0f
#define CHASSIS_RC_MOVE_RATIO_W 1.0f
//========================================
#define CHASSIS_RC_MAX_SPEED_Z  660.0f
#define CHASSIS_RC_MOVE_RATIO_Z 1.0f
/* wheel track distance(mm) */ //左右轮距
#define WHEELTRACK             410
/* wheelbase distance(mm) *///前后轴距
#define WHEELBASE              340
/* math relevant */
/* radian coefficient */
#define RADIAN_COEF        57.3f
/*角度转化比例 */
#define ANGLE_TO_RAD   0.01745329251994329576923690768489f
/* chassis motor use 3508 */
/* the deceleration ratio of chassis motor */
#define CHASSIS_DECELE_RATIO  (1.0f/19.0f)
/* single 3508 motor maximum speed, unit is rpm */
#define MAX_WHEEL_RPM        6500//8500  //8347rpm = 3500mm/s
/* the perimeter of wheel(mm) */
#define PERIMETER              478
/* chassis maximum translation speed, unit is mm/s */
#define MAX_CHASSIS_VX_SPEED 4200  //8000rpm
#define MAX_CHASSIS_VY_SPEED 4200
/* chassis maximum rotation speed, unit is degree/s */
#define MAX_CHASSIS_VR_SPEED 2000   //5000rpm


//底盘电机速度环PID
#define M3505_MOTOR_SPEED_PID_KP 15.8656897f
#define M3505_MOTOR_SPEED_PID_KI 0.59958556f
#define M3505_MOTOR_SPEED_PID_KD 0.0f
#define M3505_MOTOR_SPEED_PID_MAX_OUT  5000.0f
#define M3505_MOTOR_SPEED_PID_MAX_IOUT 500.0f

//底盘旋转跟随PID
#define CHASSIS_FOLLOW_GIMBAL_PID_KP 3.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KI 0.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KD 0.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT  40.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT 10.0f

typedef struct
{
  const motor_measure_t *chassis_motor_measure;
  const	Encoder_process_t *chassis_encoder;	   //底盘电机编码器归整数据
  int16_t give_current;
} Chassis_Motor_t;


typedef enum
{
  CHASSIS_RELAX          	= 0,//底盘无力模式
  CHASSIS_INIT           	= 1,
  CHASSIS_SEPARATE_GIMBAL     = 2,//云台分离
  CHASSIS_FOLLOW_GIMBAL		= 3,//云台跟随
} Chassis_mode_e; //底盘运行模式 


typedef struct
{
//     const Gimbal_Motor_t *chassis_yaw_motor;   //底盘使用到yaw云台电机的相对角度来计算底盘的欧拉角
//     const Gimbal_Motor_t *chassis_pitch_motor; //底盘使用到pitch云台电机的相对角度来计算底盘的欧拉角
  Chassis_mode_e chassis_mode;               //底盘控制状态机
  Chassis_Motor_t motor_chassis[4];          //底盘电机数据
	PidTypeDef chassis_angle_pid;              //底盘跟随角度pid
	PidTypeDef motor_speed_pid[4];             //底盘电机速度pid
	const RC_ctrl_t *chassis_rc_ctrl;  
	float RC_X_ChassisSpeedRef;			//左右动态输入
  float RC_Y_ChassisSpeedRef;			//前后动态输入
  float RC_W_ChassisSpeedRef;			//转动动态输入
	float RC_Z_ChassisSpeedRef;			//仰俯动态输入
  float vx;
	float vy;
	float vz;
	int16_t         rotate_x_offset;
	int16_t         rotate_y_offset;
	int16_t         wheel_spd_ref[4];
	float		 				wheel_spd_fdb[4];
	float           chassis_relative_angle_set;
			
} chassis_move_t;
#define VAL_LIMIT(val, min, max) \
  do                             \
  {                              \
    if ((val) <= (min))          \
    {                            \
      (val) = (min);             \
    }                            \
    else if ((val) >= (max))     \
    {                            \
      (val) = (max);             \
    }                            \
  } while (0)


extern void chassis_task(void *pvParameters);
#endif
