
#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h"
	
#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收
//extern u16 USART3_RX_STA;         		//接收状态标记	
//extern char RxCounter3,RxBuffer3[USART_REC_LEN];
//如果想串口中断接收，请不要注释以下宏定义
void UART3_Init(u32 bound);
void USART3_SendData(u8 data ); 
u8 USART3_ReadData(void );
u16 US100_GetVal(void );  
#endif

