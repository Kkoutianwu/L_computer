#ifndef _US_100_H_
#define _US_100_H_

#include "config.h"
#include "sys.h"

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收

class US_100 
{
	public:
		void USART3_Init(u32 bound);
		void USART3_IRQHandler(void);
		void USART3_SendData(u8 data );
		u8 	USART3_ReadData(void );
		u16 US100_GetVal(void ) ;

};

#endif //_LED_H_