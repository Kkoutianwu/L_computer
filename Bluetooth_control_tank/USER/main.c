#include "led.h"
#include "delay.h"
#include "sys.h"
#include <string.h>
#include "usart.h"
#include "usart2.h"
#include "hc05.h" 
#include "motor.h"
#include "key.h"
#include "stm32f10x.h"

void run(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=1;	
}
void back(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=0;
	AIN2=1;
	BIN1=1;
	BIN2=0;	
}
void right(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;	
	//delay_ms(900);
}
void left(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=0;
	AIN2=1;
	BIN1=0;
	BIN2=1;	
	//delay_ms(900);
}
void stop()
{

	AIN1=0;
	AIN2=0;
	BIN1=0;
	BIN2=0;	

}
 int main(void)
 {	

  u8 key,kb;
	u8 reclen=0;  	 
//	int Encoder_Left=0,Encoder_Right=0;             //左右编码器的脉冲计数
 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	KEY_Init();   
	MiniBalance_PWM_Init(899,0);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 
	//Encoder_Init_TIM2();            //=====编码器接口
  //Encoder_Init_TIM4();            //=====初始化编码器2
  PWMA=0;
	PWMB=0;
	AIN2=1;
	AIN1=0;
	BIN1=1;
	BIN2=0;	
	delay_ms(1000);//等等初始化完成
	 HC05_Init();
//  while(HC05_Init()) 		//初始化ATK-HC05模块  
//	{
//		
//		LED0=0;
//	
//		
//	}	
		LED0=1;	
	delay_ms(1000);
	
	
	while(1)
	{
		
//		u2_printf("#000P2500T1000!");
//		delay_ms(1000);
//		delay_ms(1000);
//		u2_printf("#000P0500T1000!");
//		delay_ms(1000);
//		delay_ms(1000);
		if(USART2_RX_STA&0X8000)			//接收到一次数据了
		{
	
 			reclen=USART2_RX_STA&0X7FFF;	//得到数据长度
		  	USART2_RX_BUF[reclen]=0;	 	//加入结束符
			if(reclen>2) 		//控制DS1检测
			{
				if(strcmp((const char*)USART2_RX_BUF,"mod1")==0)//打开LED1
				{  
					USART2_RX_STA=0;
						while(1)
						{
						
								if(SENSOR==0)
								{
									 delay_ms(50);
									 if(SENSOR==0)
									{  
											back(900,650);		
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
											delay_ms(100);
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
										
											delay_ms(100);
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
										
											delay_ms(100);
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
										
											delay_ms(100);
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
											
											delay_ms(100);
											LED0=~LED0;
											LED1=~LED1;
											LED=~LED;
											delay_ms(100);
								
											left(900,650);
										  delay_ms(900);
									 }
											
								}
								else
								{
									LED=0;
									run(900,670);
									
								}	
						 				
	               if(USART2_RX_STA&0X8000)	break;								
						}					
				}
				if(strcmp((const char*)USART2_RX_BUF,"led")==0)
				{
								LED0=~LED0;
          			LED1=~LED1;
				       	LED=~LED;
				}//关闭LED1
			  if(strcmp((const char*)USART2_RX_BUF,"run")==0)run(900,670);
				if(strcmp((const char*)USART2_RX_BUF,"back")==0)back(900,770);
				if(strcmp((const char*)USART2_RX_BUF,"left")==0)left(600,500);	
				if(strcmp((const char*)USART2_RX_BUF,"right")==0)right(600,500);
				if(strcmp((const char*)USART2_RX_BUF,"stop")==0)stop();
				
				
			}
 			USART2_RX_STA=0;	 
		}	 				
		
		
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("翻转\r\n");
//			LED0=~LED0;
//			LED1=~LED1;
//			run(900,670);
//			LED=~LED;
//			USART_RX_STA=0;
//			}else
//		{
//			//Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
//			//Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值	
//			// printf("L:%d     R:%d\r\n",Encoder_Left,Encoder_Right);  
//			 
//		//	printf("L:%d     R:%dr\n",400,500);  
//			 delay_ms(50);

//		}
		
//	if(SENSOR==0)
//	{
//		 delay_ms(50);
//		 if(SENSOR==0)
//		{  
//				back(900,650);		
//	      LED=1;
//		    delay_ms(100);
//	      LED=0;
//		    delay_ms(100);
//	      LED=1;
//		    delay_ms(100);
//	      LED=0;
//		    delay_ms(100);
//	      LED=1;
//		    delay_ms(100);
//	      LED=0;
//		    delay_ms(100);
	



//				left(900,650);
//		 }
//				
//	}
//	else
//	{
//		    LED=0;
//		run(900,670);
//		
//	}	
	
	}	 
}


