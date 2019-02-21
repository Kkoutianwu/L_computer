#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "motor.h"
#include "encoder.h"
#include "key.h"
#include "stm32f10x.h"
#include "PID.h"
#include "timer.h"
#include "UART3.h"

float setL,setR;
int  dis;  
int sl,sr;
char *str[7]={"0001","0002","0003","0004","0005","0006","0007"};//歌单
int Encoder_Left=0,Encoder_Right=0;             //左右编码器的脉冲计数
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

void stop(void)
{

	AIN1=0;
	AIN2=0;
	BIN1=0;
	BIN2=0;	
	PID_ParameterReckonL();
	PID_ParameterReckonR();
}

void left(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=0;
	AIN2=0;
	BIN1=0;
	BIN2=1;	

}
void right(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=0;	
	
}

void diaotou(int l,int r)
{
	PWMA=l;
	PWMB=r;
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;	
	
}

void linear(void)
{

		setL=3;setR=3;
		Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值	
		if(TrackerM==1)
		{setL=3;setR=3;}
		else
		{
			if(TrackerL==1)
			{
				setR+=1;
				if(setR>15)setR=15;
					
			}
			else if(TrackerR==1)
			{
				setL+=1;
				if(setL>15)setL=15;
			}
			else{setL=3;setR=3;}
		}
		sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
		sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
		printf("SL:%d     SR:%d\r\n",sl,sr);
		run(sl,sr);
		delay_ms(3);
}

void rightturn(void)
{
	int i;
		stop();
		for(i=0;i<50;i++)
		{
			Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致				
			sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
			printf("SL:%d     SR:%d\r\n",sl,sr);
			right(sl,0);
			delay_ms(10);
		}
		while(TrackerM==0)
		{
			Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
			Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值					
			sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
			sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
			printf("SL:%d     SR:%d\r\n",sl,sr);	
			right(sl,0);
			delay_ms(1);
		}
		for(i=0;i<0;i++)
		{
			Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致				
			sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
			printf("SL:%d     SR:%d\r\n",sl,sr);
			right(sl,0);
			delay_ms(10);
		}
		stop();
}

void leftturn(void)
{
		int i;
		stop();
		for(i=0;i<50;i++)
		{
		Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值					
		sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
		sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
		printf("SL:%d     SR:%d\r\n",sl,sr);
		left(sl,sr);
		 delay_ms(10);
		}
		while(TrackerM==0)
		{
		Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值					
		sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
		sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
		printf("SL:%d     SR:%d\r\n",sl,sr);	
		left(sl,sr);
		 delay_ms(10);
		}
		for(i=0;i<0;i++)
		{
		Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值					
		sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
		sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
		printf("SL:%d     SR:%d\r\n",sl,sr);
		left(sl,sr);
		 delay_ms(10);
		}
		stop();	
}

void turn_round(void)
{
	int i,count;
	 stop();
		for(i=0;i<170;i++)
		{		
        if(SENSOR==0)
				{
					 delay_ms(50);
					 if(SENSOR==0)
						{ 
							 delay_ms(50);
							 if(SENSOR==0)
								{ 
									 stop();
									while(SENSOR==0);
								}
						}
				}			
				setL=3;setR=3;
				Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
				Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值	
				if(TrackerM==1){setL=3;setR=3;}
				else
				{
					if(TrackerL==1)
					{
						setR+=1;
					}
					else if(TrackerR==1)
					{
						setL+=1;
					}
					else{setL=3;setR=3;}
				}

				sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
				sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
				printf("SL:%d     SR:%d\r\n",sl,sr);
				run(sl,sr);
				delay_ms(2);
		}
		
		
		setL=3;setR=3;
		for(i=0;i<100;i++)
		{
				Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
				Encoder_Right=-Read_Encoder(4);                                      //===读取编码器的值					
				sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
				sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
				printf("SL:%d     SR:%d\r\n",sl,sr);
				diaotou(sl,sr);
				delay_ms(1);
		}
		while(TrackerM==0)
		{
				Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
				Encoder_Right=-Read_Encoder(4);                                      //===读取编码器的值					
				sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
				sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
				printf("SL:%d     SR:%d\r\n",sl,sr);	
				diaotou(sl,sr);
				delay_ms(1);
		}

		PID_ParameterReckonL();
		PID_ParameterReckonR();
		while(TrackerM==0 || TrackerB==0)
		{	
				setL=3;setR=3;
				PID_ParameterReckonL();
				PID_ParameterReckonR();	
				count=0;							
				while(TrackerB==0 && TrackerL==0)
				{
						count++;
						Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致		
						sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));										
						printf("SL:%d     SR:%d\r\n",sl,sr);	
						right(sl,0);
						delay_ms(1);
					if(count>=100)break;
				}
				PID_ParameterReckonL();
				PID_ParameterReckonR();	
				count=0;									
				while(TrackerM==0 && TrackerR==0)
				{
						count++;
						Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值														
						sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
						printf("SL:%d     SR:%d\r\n",sl,sr);	
						left(0,sr);
						delay_ms(1);
						if(count>=100)break;
				}
		}
		stop();
		//	while(1);
}
void rest(int t)
{
	int i,j;
	LED=0;
	LED0=0;
	LED1=0;
	for(i=0;i<t;i++)
	{
			for(j=0;j<60;j++)
			{	
				delay_ms(1000);
			}
			delay_ms(10);
			UART3_Put_String("play,0001,$");
			delay_ms(10);
	}
	LED=1;
	LED0=1;
	LED1=1;
}

void ignore(void)
{
	int i;
			if(TrackerL==1 && TrackerR==1)
			{
				
							for(i=0;i<20;i++)
							{					
									setL=3;setR=3;
									Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
									Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值	
									if(TrackerM==1){setL=3;setR=3;}
									else
									{
										if(TrackerL==1)
										{
											setR+=1;
										}
										else if(TrackerR==1)
										{
											setL+=1;
										}
										else{setL=3;setR=3;}
									}

									sl=(int)(PID_ControlL((float)(setL-Encoder_Left)));
									sr=(int)(PID_ControlR((float)(setR-Encoder_Right)));
									printf("SL:%d     SR:%d\r\n",sl,sr);
									run(sl,sr);
									delay_ms(2);
							}
					
			}
}	
 int main(void)
 {	


  int i,zwcs=0,num=0,cs=0; 

 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	KEY_Init();   
	MiniBalance_PWM_Init(899,0);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 
	Encoder_Init_TIM2();            //=====编码器接口
  Encoder_Init_TIM4();            //=====初始化编码器2
	TIM3_Cap_Init(0XFFFF,72-1);	    //=====超声波初始化
	Initial_UART3(9600);
  PWMA=0;
	PWMB=0;
	AIN2=1;
	AIN1=0;
	BIN1=1;
	BIN2=0;	
	PID_ParameterReckonL();
	PID_ParameterReckonR();
	delay_ms(10);
	UART3_Put_String("play,0001,$");
	delay_ms(10);
	delay_ms(1000);
	setL=3;
	setR=3;
	rest(80);
	while(1)
	{

			if(SENSOR==0)
				{
					 delay_ms(50);
					 if(SENSOR==0)
						{ 
							 delay_ms(50);
							 if(SENSOR==0)
								{ 
									 stop();
									while(SENSOR==0);
								}
						}
				}

//				printf("L:%d     R:%d\r\n",Encoder_Left,Encoder_Right);  
//			printf("SL:%d     SR:%d\r\n",((int)PID_ControlL((float)(setL-Encoder_Left))),((int)PID_ControlR((float)(setR-Encoder_Right)))); 
        linear(); //直线走
				i++;
				if(i>=3)
				{
					i=0;
					dis=(int)(Read_Distane());
					if(dis==0)
					{
						dis=1500;
					}
					printf("D:%d\r\n",dis); 
				}
				if(dis<=120)
				{
					 delay_ms(10);
					dis=(int)(Read_Distane());
					printf("D:%d\r\n",dis); 
					 if(dis<=120)
						{ 
									 stop();
									while(dis<=120)
									{
										dis=(int)(Read_Distane());
										printf("D:%d\r\n",dis); 
									}
						
						}
				}
				
		 setL=3;setR=3;
		/********************转弯**************/
			if(TrackerL==1 && TrackerR==1 && dis<820 &&zwcs==0)
			{
							rightturn();
								zwcs++;

							delay_ms(10);
							UART3_Put_String("play,0002,$");
							delay_ms(10);
				      

			}

			if(TrackerL==1 && TrackerR==1 && dis<500 && zwcs==1)
			{
						  leftturn();
							zwcs++;
							delay_ms(10);
							UART3_Put_String("play,0003,$");
							delay_ms(10);
			}
			
			if(TrackerL==1 && TrackerR==1 && dis<700 && zwcs==2)
			{
							rightturn();
							zwcs++;
							delay_ms(10);
							UART3_Put_String("play,0004,$");
							delay_ms(10);
			}
			
			
			if(TrackerL==1 && TrackerR==1  && zwcs==3)
			{      
				      if(num<2)
							{
								ignore();
								num++;								
							}
							else
							{
								num=0;
								rightturn();
								zwcs++;
								delay_ms(10);
								UART3_Put_String("play,0005,$");
								delay_ms(10);
							}
			}
			
			if(TrackerL==1 && TrackerR==1 && (zwcs==4 || zwcs==6 || zwcs==11 || zwcs==14))
			{
							leftturn();
							zwcs++;
							delay_ms(10);
							UART3_Put_String("play,0006,$");
							delay_ms(10);
						
			}
			
			if(TrackerL==1 && TrackerR==1 &&  (zwcs==5 ||  zwcs==7 ||  zwcs==13))
			{
							rightturn();
							zwcs++;
							delay_ms(10);
							UART3_Put_String("play,0007,$");
							delay_ms(10);
			}	
		
/*************************************掉头******************************************/				
				if(TrackerL==1 && TrackerR==1  && dis<1200 && zwcs==8) //dis<1200
				{
						turn_round();
						zwcs++;
						delay_ms(10);
						UART3_Put_String("play,0008,$");
						delay_ms(10);
				}
				
				if(TrackerL==1 && TrackerR==1 && dis<800 && zwcs==9)
				{
					leftturn();
					zwcs++;
					delay_ms(10);
					UART3_Put_String("play,0001,$");
					delay_ms(10);
				}
				
				if(TrackerL==1 && TrackerR==1 && dis<420 && zwcs==10)
				{
 					rightturn();
					zwcs++;         
					delay_ms(10);
					UART3_Put_String("play,0002,$");
					delay_ms(10);

				}
				
			if(TrackerL==1 && TrackerR==1  && zwcs==12)
			{      
				      if(num<2)
							{
								ignore();
								num++;								
							}
							else
							{
								num=0;
								leftturn();
								zwcs++;
								delay_ms(10);
								UART3_Put_String("play,0001,$");
								delay_ms(10);
							}
			}
				
			if(TrackerL==1 && TrackerR==1 && dis<1200 && zwcs==15) //dis<1200
			{
					turn_round();
					zwcs=0;
					delay_ms(10);
					UART3_Put_String("play,0004,$");
					delay_ms(10);
				  rest(10);
					cs++;
					if(cs>=40)
					{
						stop();
						USART_Cmd(USART3, DISABLE);
						while(1);
					}
			}      				
/*******************************************/
				
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("翻转\r\n");
//			LED0=~LED0;
//			LED1=~LED1;
//			set+=5;
//			if(set>41) set=0;
//			LED=~LED;
//			USART_RX_STA=0;
//			}


//		  dis=(int)(Read_Distane());
//		  	printf("D:%d\r\n",dis); 
//				delay_ms(10);				
//			printf("play,0001,$"); 	
	}	 
}

