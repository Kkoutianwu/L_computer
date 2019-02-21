#ifdef __cplusplus
extern "C" {
#endif
#ifndef __KEY_H
#define __KEY_H	 
#include "config.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//读取按键0
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//读取按键1
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键2 
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) //读取按键3 
 
void KEY_Init(void);//IO初始化
int KEY_Scan(int);  	//按键扫描函数					    
#endif
#ifdef __cplusplus
}
#endif