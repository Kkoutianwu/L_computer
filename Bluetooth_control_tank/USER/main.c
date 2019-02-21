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
//	int Encoder_Left=0,Encoder_Right=0;             //���ұ��������������
 
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
	KEY_Init();   
	MiniBalance_PWM_Init(899,0);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� 
	//Encoder_Init_TIM2();            //=====�������ӿ�
  //Encoder_Init_TIM4();            //=====��ʼ��������2
  PWMA=0;
	PWMB=0;
	AIN2=1;
	AIN1=0;
	BIN1=1;
	BIN2=0;	
	delay_ms(1000);//�ȵȳ�ʼ�����
	 HC05_Init();
//  while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
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
		if(USART2_RX_STA&0X8000)			//���յ�һ��������
		{
	
 			reclen=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
		  	USART2_RX_BUF[reclen]=0;	 	//���������
			if(reclen>2) 		//����DS1���
			{
				if(strcmp((const char*)USART2_RX_BUF,"mod1")==0)//��LED1
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
				}//�ر�LED1
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
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("��ת\r\n");
//			LED0=~LED0;
//			LED1=~LED1;
//			run(900,670);
//			LED=~LED;
//			USART_RX_STA=0;
//			}else
//		{
//			//Encoder_Left=-Read_Encoder(2);                                      //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
//			//Encoder_Right=Read_Encoder(4);                                      //===��ȡ��������ֵ	
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


