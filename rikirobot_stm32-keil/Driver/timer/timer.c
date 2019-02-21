#ifdef __cplusplus
extern "C" {
#endif
#include"oled.h"
#include "timer.h"
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

	
void MiniBalance_Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
}


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	MiniBalance_Motor_Init();	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH2
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}


void ESC_Init(u16 pwm_value)
{
	
  TIM_SetCompare1(TIM3,pwm_value);	//1526
	TIM_SetCompare2(TIM3,pwm_value);	//1526
	TIM_SetCompare3(TIM3,pwm_value);	//1526
	TIM_SetCompare4(TIM3,pwm_value);  //1526
}
void ESC1_PWM(u16 pwm_value)
{ 
	char pwm_buff[50];
	sprintf(pwm_buff,"m1:%d",pwm_value);
	OLED_ShowStr(0,0,pwm_buff, 2);
	if(pwm_value  <1000)
		pwm_value = 1526;
	if(pwm_value>1928)
		pwm_value = 1928;
	if(pwm_value<1124)
		pwm_value = 1124;

  TIM_SetCompare1(TIM3,pwm_value);

}
void ESC2_PWM(u16 pwm_value)
{  
	char pwm_buff[50];
	sprintf(pwm_buff,"m2:%d",pwm_value);
	OLED_ShowStr(70,0,pwm_buff, 2);
	if(pwm_value  <1000)
		pwm_value = 1526;
	if(pwm_value>1928)
		pwm_value = 1928;
	if(pwm_value<1124)
		pwm_value = 1124;

  TIM_SetCompare2(TIM3,pwm_value);	//1526

}
void ESC3_PWM(u16 pwm_value)
{ 
	char pwm_buff[50];
	sprintf(pwm_buff,"m3:%d",pwm_value);
	OLED_ShowStr(0,2,pwm_buff, 2);
	if(pwm_value  <1000)
		pwm_value = 1526;
	if(pwm_value>1928)
		pwm_value = 1928;
	if(pwm_value<1124)
		pwm_value = 1124;

  TIM_SetCompare3(TIM3,pwm_value);	//1526

}
void ESC4_PWM(u16 pwm_value)
{  
	char pwm_buff[50];
	sprintf(pwm_buff,"m4:%d",pwm_value);
	OLED_ShowStr(70,2,pwm_buff, 2);
	if(pwm_value <1000 )
		 pwm_value = 1526;
	if(pwm_value>1928)
		pwm_value = 1928;
	if(pwm_value<1124)
		pwm_value = 1124;
  TIM_SetCompare4(TIM3,pwm_value);	//1526

}
#ifdef __cplusplus
}
#endif
