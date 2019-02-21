#include "led.h"
#include "stdlib.h"
#include "delay.h"
#include "sys.h"
#include <string.h>
#include "usart.h"
#include "usart2.h"
#include "hc05.h" 
#include "motor.h"
#include "key.h"
#include "stm32f10x.h"
short angle[6];
char s[6][4];
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
	u8 len=0;
	int i,j; 	 
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
	LED0=1;	
	delay_ms(1000);	
	while(1)
	{
		
//		u2_printf("#000P%sT1000!",s1);
//		delay_ms(1000);	
//		delay_ms(1000);						
//		u2_printf("#000P%sT1000!",s2);	
//		delay_ms(1000);		
//			delay_ms(1000);
//			for(i=0;i<6;i++)
//				{
//					angle[i]=20;
		
//					angle[i]=(int)(angle[i]*1000/135.0+1500);
//					sprintf(s[i],"%d",angle[i]);
//				}
//				u2_printf("{#000P%sT1000!#001P%sT1000!#002P%sT1000!#003P%sT1000!#004P%sT1000!#005P%sT1000!#006P%sT1000!}");
	//	printf("OKK");	
		//delay_ms(100);	
		if(USART_RX_STA&0X8000)			//接收到一次数据了
		{
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			USART_RX_BUF[len]=0;	 	//加入结束符
			if(len>3) 		//控制DS1检测
			{
 
				u2_printf("%s",USART_RX_BUF);
				if(strcmp((const char*)USART_RX_BUF,"rest\r\n")==0)u2_printf("{#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!#006P1500T1000!}");
				delay_ms(10);
				printf("OK\r\n");
				delay_ms(10);				
			}	
			USART_RX_STA=0;	   		
		}	
		

		delay_ms(50);	
	}	 
}


