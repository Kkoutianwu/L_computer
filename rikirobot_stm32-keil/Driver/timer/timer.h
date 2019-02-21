#ifdef __cplusplus
extern "C" {
#endif
	
#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
	
#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)
	
void MiniBalance_Motor_Init(void);	
void TIM3_PWM_Init(u16 arr,u16 psc);
void ESC_Init(u16 pwm_value);//电调中值初始化等待

														 //1124 1526 1928
void ESC1_PWM(u16 pwm_value);// 小   中   大
void ESC2_PWM(u16 pwm_value);// 小   中   大
void ESC3_PWM(u16 pwm_value);// 小   中   大
void ESC4_PWM(u16 pwm_value);// 小   中   大
#endif
#ifdef __cplusplus
}
#endif
