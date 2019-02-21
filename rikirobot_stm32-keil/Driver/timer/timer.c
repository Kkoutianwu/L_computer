#ifdef __cplusplus
extern "C" {
#endif
#include"oled.h"
#include "timer.h"
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

	
void MiniBalance_Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	MiniBalance_Motor_Init();	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH2
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
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
