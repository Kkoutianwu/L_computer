
#include "sys.h"
#include "usart3.h"	  
#include "delay.h"

u8 USART3_RX_BUF[USART3_REC_LEN];
u16 USART3_RX_STA=0;       //����״̬���	 	
	
void UART3_Init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//ʹ��USART3ʱ��
	USART_DeInit(USART3);
	
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART3_RX	  GPIOB.11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
  USART_Init(USART3, &USART_InitStructure); 					//��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    			//ʹ�ܴ���3

}

void USART3_IRQHandler(void)                	//����3�жϷ������
{
		u8 Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
				Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
				if((USART3_RX_STA&0x8000)==0)//����δ���
				{
						if(USART3_RX_STA&0x4000)//���յ���0x0d
						{
								if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
								else USART3_RX_STA|=0x8000;	//��������� 
						}
						else //��û�յ�0X0D
						{	
								if(Res==0x0d)USART3_RX_STA|=0x4000;
								else
									{  		if(USART3_RX_STA>1)USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	
											USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
											USART3_RX_STA++;
											if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
										
									}		 
						}
				} 
		
		} 
} 
void USART3_SendData(u8 data )        													//����3����
{
        USART_SendData(USART3, data);         											//��������  USART_SendData�⺯��
        while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);						//�ȴ����ͽ���
        USART_ClearFlag(USART3,USART_FLAG_TXE);											//���־
} 
u8 USART3_ReadData(void )                												//����3����
{
        u8 data;
        if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != RESET)  						//�ȴ�����
        {
                data =USART_ReceiveData(USART3);										//(USART1->DR);   ��ȡ���յ�������
        }
        USART_ClearFlag(USART3,USART_FLAG_RXNE);
        return data;
} 


u16 US100_GetVal(void )  																// ���ص�λ ���� mm 
{

      u16 data=0;
      USART3_SendData(0x55);                                                          //����������ģ�鷢�� ��������0x55
	    data = (u16)(USART3_RX_BUF[0]<<8)+(u16)USART3_RX_BUF[1];
			return data;
}
