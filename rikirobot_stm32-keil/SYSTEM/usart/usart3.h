
#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h"
	
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������3����
//extern u16 USART3_RX_STA;         		//����״̬���	
//extern char RxCounter3,RxBuffer3[USART_REC_LEN];
//����봮���жϽ��գ��벻Ҫע�����º궨��
void UART3_Init(u32 bound);
void USART3_SendData(u8 data ); 
u8 USART3_ReadData(void );
u16 US100_GetVal(void );  
#endif

