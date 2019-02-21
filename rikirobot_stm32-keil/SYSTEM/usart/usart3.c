
#include "sys.h"
#include "usart3.h"	  
#include "delay.h"

u8 USART3_RX_BUF[USART3_REC_LEN];
u16 USART3_RX_STA=0;       //接收状态标记	 	
	
void UART3_Init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//使能USART3时钟
	USART_DeInit(USART3);
	
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
  USART_Init(USART3, &USART_InitStructure); 					//初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    			//使能串口3

}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
		u8 Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				Res =USART_ReceiveData(USART3);	//读取接收到的数据
				if((USART3_RX_STA&0x8000)==0)//接收未完成
				{
						if(USART3_RX_STA&0x4000)//接收到了0x0d
						{
								if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
								else USART3_RX_STA|=0x8000;	//接收完成了 
						}
						else //还没收到0X0D
						{	
								if(Res==0x0d)USART3_RX_STA|=0x4000;
								else
									{  		if(USART3_RX_STA>1)USART3_RX_STA=0;//接收数据错误,重新开始接收	
											USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
											USART3_RX_STA++;
											if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	 
										
									}		 
						}
				} 
		
		} 
} 
void USART3_SendData(u8 data )        													//串口3发送
{
        USART_SendData(USART3, data);         											//发送数据  USART_SendData库函数
        while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);						//等待发送结束
        USART_ClearFlag(USART3,USART_FLAG_TXE);											//清标志
} 
u8 USART3_ReadData(void )                												//串口3接收
{
        u8 data;
        if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != RESET)  						//等待接收
        {
                data =USART_ReceiveData(USART3);										//(USART1->DR);   读取接收到的数据
        }
        USART_ClearFlag(USART3,USART_FLAG_RXNE);
        return data;
} 


u16 US100_GetVal(void )  																// 返回单位 毫米 mm 
{

      u16 data=0;
      USART3_SendData(0x55);                                                          //串口向超声波模块发送 激活数据0x55
	    data = (u16)(USART3_RX_BUF[0]<<8)+(u16)USART3_RX_BUF[1];
			return data;
}
