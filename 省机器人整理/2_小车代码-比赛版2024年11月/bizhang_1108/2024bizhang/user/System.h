/*
 * System.h
 *
 *  Created on: 2023年9月1日
 *      Author: ASUS
 */
#include "zf_common_headfile.h"
#ifndef USER_SYSTEM_H_
#define USER_SYSTEM_H_

extern void System();



#define S_MOTOR_PIN   ATOM1_CH1_P33_9       //定义舵机引脚

#define SteerMiddle       857  // 850     // 舵机中值百分比     1.5/t t=1/300*1000ms  计算得占空比应为4500/10000 可能是没有机械校准好
#define SteerLeft         915       //
#define SteerRight        785     // 600               // 舵机最大右打角百分比
#define SteerSet(x)    pwm_set_duty(S_MOTOR_PIN,x);  // 修改舵机占空比

// 电机 宏定义
#define MOTOR1_A   P02_7    //定义电机PWM引脚
#define MOTOR1_B   ATOM1_CH6_P02_6
#define MOTOR2_A   P02_5
#define MOTOR2_B   ATOM1_CH4_P02_4


//#define ENCODER2_TIM    TIM2_ENCODER    //R
//#define ENCOEDER2_CH1   TIM2_ENCODER_CH1_P33_7
//#define ENCOEDER2_CH2   TIM2_ENCODER_CH2_P33_6



//
//
#define ENCODER1_TIM    TIM5_ENCODER    //R
#define ENCOEDER1_CH1   TIM5_ENCODER_CH1_P10_3
#define ENCOEDER1_CH2   TIM5_ENCODER_CH2_P10_1
//编码器宏定义
#define ENCODER2_TIM    TIM2_ENCODER    //R
#define ENCOEDER2_CH1   TIM2_ENCODER_CH1_P33_7
#define ENCOEDER2_CH2   TIM2_ENCODER_CH2_P33_6
#define MotorMax    8000                              //PWM最高精度设置为10000
#define MotorMid    0
#define MotorMin    -8000



//编码器宏定义
#define All_Line       1024.0             //编码器线数
#define Ratio          2.267                 //齿轮比（大齿轮比小齿轮）
#define Length         0.201                 //轮胎长度20.1cm
#define Time           0.005                 //定时周期5ms
#define Multiple       100                   //m/s转换成cm/s
#define SPEED(X)       (int32)(X/All_Line/Ratio*Length/Time*Multiple)
#define DISTANCE(X)    (double)((X)*0.0288)//0.0288

#endif
