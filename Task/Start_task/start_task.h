#ifndef _START_TASK_H_
#define _START_TASK_H_

void startTask(void);

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

//电机码盘值最大以及中值
#define Half_ecd_range 4096
#define ecd_range 8191

#define PI					3.14159265358979f


#endif




