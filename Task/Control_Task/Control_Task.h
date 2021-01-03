#ifndef _CONTROL_TASK_H
#define _CONTROL_TASK_H
#include "stm32f4xx.h"                  // Device header
typedef enum
{
    Stop_STATE=0,
    Left_STATE,
    Middle_STATE,
    Right_STATE,
} Top_MotorState;

typedef  enum
{
    ResetInit_State = 0,
    Once_Capture  ,
    Once_Capture_Pro ,
    Triple_Capture  ,
    Triple_Capture_Pro  ,
} Top_WorkState;


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



extern void Control_Task(void *pvParameters);

#endif
