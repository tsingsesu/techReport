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
#define SteerLeft         925       //
#define SteerRight        785     // 600               // 舵机最大右打角百分比
#define SteerSet(x)    pwm_set_duty(S_MOTOR_PIN,x);  // 修改舵机占空比

// 电机 宏定义
// ★V3.1实测：电机改到「有刷电机1」座 P6（P7 座 DIR 反复不生效）
//   右轮 PWM=21.2 DIR=21.3   左轮 PWM=21.4 DIR=21.5
#define MOTOR1_A   P21_3                 // 右轮 DIR
#define MOTOR1_B   ATOM1_CH0_P21_2       // 右轮 PWM
#define MOTOR2_A   P21_5                 // 左轮 DIR
#define MOTOR2_B   ATOM1_CH2_P21_4       // 左轮 PWM


//#define ENCODER2_TIM    TIM2_ENCODER    //R
//#define ENCOEDER2_CH1   TIM2_ENCODER_CH1_P33_7
//#define ENCOEDER2_CH2   TIM2_ENCODER_CH2_P33_6



//
//
// ★V3.1：编码器2座(P10_3/P10_1) 接触不良，右轮改用编码器4座 TIM6
#define ENCODER1_TIM    TIM6_ENCODER    //R
#define ENCOEDER1_CH1   TIM6_ENCODER_CH1_P20_3
#define ENCOEDER1_CH2   TIM6_ENCODER_CH2_P20_0
//编码器宏定义
#define ENCODER2_TIM    TIM2_ENCODER    //R
#define ENCOEDER2_CH1   TIM2_ENCODER_CH1_P33_7
#define ENCOEDER2_CH2   TIM2_ENCODER_CH2_P33_6
#define MotorMax    3000                              //PWM最高精度设置为10000
#define MotorMid    0
#define MotorMin    -3000

// ★全局降速：目标速度 = 原值 x 12%（210 -> 25）
#define SPEED_PERCENT               (12)

// ★软启动步长：每 40 个 5ms 目标速度 +1（约 5 秒爬到 25）
#define MOTOR_RAMP_TICKS            (40)



//编码器宏定义
#define All_Line       1024.0             //编码器线数
#define Ratio          2.267                 //齿轮比（大齿轮比小齿轮）
#define Length         0.201                 //轮胎长度20.1cm
#define Time           0.005                 //定时周期5ms
#define Multiple       100                   //m/s转换成cm/s
#define SPEED(X)       (int32)(X/All_Line/Ratio*Length/Time*Multiple)
#define DISTANCE(X)    (double)((X)*0.0288)//0.0288

#endif
