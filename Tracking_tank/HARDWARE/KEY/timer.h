#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void TIM3_Cap_Init(u16 arr,u16 psc);
u32 Read_Distane(void);
void TIM3_IRQHandler(void);
#endif