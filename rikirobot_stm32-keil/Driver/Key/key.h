#ifdef __cplusplus
extern "C" {
#endif
#ifndef __KEY_H
#define __KEY_H	 
#include "config.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//��ȡ����0
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//��ȡ����1
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ȡ����2 
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) //��ȡ����3 
 
void KEY_Init(void);//IO��ʼ��
int KEY_Scan(int);  	//����ɨ�躯��					    
#endif
#ifdef __cplusplus
}
#endif