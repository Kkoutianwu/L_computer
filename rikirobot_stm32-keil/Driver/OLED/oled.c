#ifdef __cplusplus
extern "C" {
#endif
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h" 
#include "millisecondtimer.h"	
#include "ASCII_CODE_8X16_5X8_VERTICAL.h"
#include "Chinese_And_Graphic.h"
//--------------------------------------------------------
//IIC接口初始化GPIO
//--------------------------------------------------------
void IIC_Configuation(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟
	
	//因为用到PB3，与JTAG-DP冲突，所以要SWJ->CFG设置为010使PB3可用
	RCC->APB2ENR |= 1<<0; 				//使能复用功能调制配置				
	AFIO->MAPR   &= ~(0x7<<24);		//将AFIO->MAPR 的24~26位清零
	AFIO->MAPR   |= (0x2<<24);		//设置AFIO->MAPR 的24~26位为010   使PB3可作为普通IO口使用
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
}
//--------------------------------------------------------
//设置数据线为输出
//--------------------------------------------------------
void SDA_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_3;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructer);
}
//-------------------------------------------------------
//设置数据线为输入
//-------------------------------------------------------
void SDA_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_3;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructer);

}
void oled_delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=4;  //自己定义
      while(i--) ;    
   }
}
//------------------------------------------------------
//启动IIC总线
//------------------------------------------------------
void IIC_Start()
{
  SDA_Out();
	SDA_H;
	SCL_H;
  oled_delay_us(2);
	SDA_L;
	oled_delay_us(2);
	SCL_L;
	oled_delay_us(2);
}

//------------------------------------------------------
//释放IIC总线
//------------------------------------------------------
void IIC_Stop()
{
  //IIC_Start();
	SCL_H;
	SDA_L;
	oled_delay_us(2);
	SDA_H;
	oled_delay_us(2);
}
//------------------------------------------------------
//读应答
//------------------------------------------------------
unsigned char Read_Ask(void)
{
	unsigned char ReAsk;
	SDA_In();
	SCL_H;
	oled_delay_us(2);
	ReAsk=(unsigned char)SDA_READ;
	SCL_L;
	oled_delay_us(2);
	return ReAsk;
}
//-----------------------------------------------------
//写入一个字节
//-----------------------------------------------------
void Write_OneByte(unsigned char Dat)
{
     unsigned char i;
			SDA_Out();
			for(i=0;i<8;i++)
			{  
				 SCL_L;
				 oled_delay_us(2);
				 if(Dat&0x80)
				 {
					SDA_H;
				 }
					else
					{
					 SDA_L;
					}
				Dat<<=1;
				SCL_H;
				oled_delay_us(2);
				SCL_L;
				oled_delay_us(2);
			 // Dat<<=1;
			}
			Read_Ask();
}	
//-------------------------------------------------------------
u8 IIC_ReadByte(void)
{
    u8 data,i;
    SDA_H;
   	oled_delay_us(2);
    for(i=0;i<8;i++)
    {
        data<<=1;
        SCL_L;
        oled_delay_us(2);
        SCL_H;
        oled_delay_us(2);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
            data=data | 0x01;
        else 
            data=data & 0xFE;

    }
    SCL_L;
		delay(2);
    return data;
}
//------------------------------------------------------
//写命令
//------------------------------------------------------
void WriteCmd(unsigned char IIC_Cmd)
{
  IIC_Start();
  Write_OneByte(0x78);
	//Read_Ask();
	Write_OneByte(0x00);
	//Read_Ask();
	Write_OneByte(IIC_Cmd);
	//Read_Ask();
	IIC_Stop();

}
//------------------------------------------------------
//写数据
//------------------------------------------------------
void WriteDat(unsigned char IIC_Dat)
{
  IIC_Start();
  Write_OneByte(0x78);
	//Read_Ask();
	Write_OneByte(0x40);
	//Read_Ask();
	Write_OneByte(IIC_Dat);
	//Read_Ask();
	IIC_Stop();
}
//-----------------------------------------------------
//OLED初始化
//-----------------------------------------------------
void OLED_Init(void)
{ 
	delay(10);
	WriteCmd(0xae);
	WriteCmd(0x00);
	WriteCmd(0x10);
	WriteCmd(0x40);
	WriteCmd(0x81);
	WriteCmd(0xcf);
	WriteCmd(0xa1);
	WriteCmd(0xc8);
	WriteCmd(0xa6);
	WriteCmd(0xa8);
	WriteCmd(0x3f);
	WriteCmd(0xd3);
	WriteCmd(0x00);
	WriteCmd(0xd5);
	WriteCmd(0x80);
	WriteCmd(0xd9);
	WriteCmd(0xf1);
	WriteCmd(0xda);
	WriteCmd(0x12);
	WriteCmd(0xdb);
	WriteCmd(0x40);
	WriteCmd(0x20);
	WriteCmd(0x02);
	WriteCmd(0x8d);
	WriteCmd(0x14);
	WriteCmd(0xa4);
	WriteCmd(0xa6);
	WriteCmd(0xaf);
}
//----------------------------------------------------------------------
//设置坐标
//----------------------------------------------------------------------
void OLED_SetPos(unsigned char x,unsigned char y)
{
 WriteCmd(0xb0+y);
 WriteCmd(((x&0xf0)>>4)|0x10);
 WriteCmd((x&0x0f)|0x01);
}
//-----------------------------------------------------------------------
//OLED全屏填充
//----------------------------------------------------------------------
void OLED_Fill(unsigned char fill_Data)
{
 unsigned char m,n;
	for(m=0;m<8;m++)
	{
	 WriteCmd(0xb0+m);
	 WriteCmd(0x00);
	 WriteCmd(0x10);
	 for(n=0;n<128;n++)
		{
		WriteDat(fill_Data);
		}
	}
}
//--------------------------------------------------------------
void OLED_CLS(void)
{
 OLED_Fill(0x00);

}
//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none  
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
 WriteCmd(0x8d);
 WriteCmd(0x10);
 WriteCmd(0xae);
}
//-------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 
//------------------------------------------------------------
void OLED_OFF(void)
{
WriteCmd(0x8d);
WriteCmd(0x10);
WriteCmd(0xae);
}
//-------------------------------------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//------------------------------------------------------------------------------------------------
void OLED_ShowStr(unsigned char x,unsigned char y,const char ch[], unsigned char TextSize) //字符串显示函数
{
	unsigned char c=0,i=0,j=0;
	  switch(TextSize)
		{	case 1:
			{
			 while(ch[j]!='\0')
			 {
			  c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			  }
			 	break;
			}	
	   	case 2:
			{		
				 while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(x > 120)
					{
						x = 0;
						y++;
					}
					OLED_SetPos(x,y);
					for(i=0;i<8;i++)
						WriteDat(F8X16[c*16+i]);
					OLED_SetPos(x,y+1);
					for(i=0;i<8;i++)
						WriteDat(F8X16[c*16+i+8]);
					x += 8;
					j++;			
			  }
	   break;
		}
	}			
}
/*****************************************************************************/
//全屏清屏
void clear_screen(void)
{
	unsigned char i,j;
 	for(j=0;j<8;j++)
	{
		lcd_address(1+j,1);
		for(i=0;i<128;i++)
		{
			WriteDat(0x00);
		}
	}
}

void lcd_address(u8 page,u8 column)
{
	column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	WriteCmd(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	WriteCmd(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	WriteCmd(column&0x0f);				//设置列地址的低4位
}
/*//显示128x64点阵图像
void display_128x64(u8 *dp)
{
	u16 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,1);
		for (i=0;i<128;i++)
		{	
			WriteDat(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}
//显示128x16点阵图像
void display_128x16(u8 page,u8 column,u8 *dp)
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			WriteDat(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}
//显示32x32点阵图像、汉字、生僻字或32x32点阵的其他图标
void display_graphic_32x32(u8 page,u8 column,const u8 *dp)
{
	u8 i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<32;i++)
		{	
			WriteDat(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}
//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_16x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			WriteDat(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}
//显示8x16点阵图像、汉字、生僻字或8x16点阵的其他图标
void display_graphic_16x8(u8 page,u8 column,u16 *dp)
{
	u8 i,j;
	for(j=0;j<1;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			WriteDat(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
		dp++;
		}
	}
}
//显示128x64点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_128x64(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<128;i++)
		{	
			WriteDat(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}
//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
void display_graphic_8x16(u8 page,u8 column,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{	
			WriteDat(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16(u16 page,u16 column,u8 *text)
{
	u16 i=0,j,k,n;
	if(column>123)
	{
		column=1;
		page+=2;
	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{					
					WriteDat(ascii_table_8x16[j][k+8*n]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}

//显示5x8的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_5x8(u16 page,u16 column,u8 reverse,const char *text)
{
	u16 i=0,j,k,disp_data;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)
			{
				if(reverse==1)
				{
					disp_data=~ascii_table_5x8[j][k];
				}
				else
				{
					disp_data=ascii_table_5x8[j][k];
				}
				
				WriteDat(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
			}
			if(reverse==1)	transfer_data(0xff);	//写入一列空白列，使得5x8的字符与字符之间有一列间隔，更美观
			else	transfer_data(0x00);			//写入一列空白列，使得5x8的字符与字符之间有一列间隔，更美观
			i++;
			column+=6;
			if(column>123)
			{
				column=1;
				page++;
			}
		}
		else
		i++;
	}
}


//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(u8 page,u8 column,u8 *text)
{
    u8 i,j,k;
    u16 address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f即说明不是ASCII码字符
        {
            if(Chinese_text_16x16[i] == text[j])
            {
                if(Chinese_text_16x16[i + 1] == text[j + 1])
                {
                    address = i * 16;
                    break;
                }
            }
            i += 2;            
        }
        
        if(column > 113)
        {
            column = 0;
            page += 2;
        }
        
        if(address != 1)// 显示汉字                   
        {
            
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                WriteDat(Chinese_code_16x16[address]);   
	                address++;
	            }
   			}
            j += 2;        
        }
        else              //显示空白字符            
        {
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                WriteDat(0x00);   
	            }
   			}
            
            j++;
        }
        
        column+=16;
    }
}


//显示16x16点阵的汉字或者ASCII码8x16点阵的字符混合字符串
//括号里的参数：(页，列，字符串）
void disp_string_8x16_16x16(u8 page,u8 column,const u8 *text)
{
    u8 temp[3];
    u8 i = 0;    
    
    while(text[i] != '\0')
    {
        if(text[i] > 0x7e)
        {
            temp[0] = text[i];
            temp[1] = text[i + 1];
            temp[2] = '\0';          //汉字为两个字节
            display_string_16x16(page,column,temp);  //显示汉字
            column += 16;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //字母占一个字节
            display_string_8x16(page, column, temp);  //显示字母
            column += 8;
            i++;
        }
    }
}
//显示数字串
void display_number_16x8(u8 page,u8 column,u16 number)
{
    unsigned char i=0,n1=0,k=0;
	u8 n[4]={0};
    n[3]= number%10;
    n[2]=(number/10)%10;
    n[1]=(number/100)%10;
    n[0]=(number/1000)%10;
	for(i=0;i<4;i++) 
	{
		n[i]=n[i]+16;
		for(n1=0;n1<2;n1++)
			{			
			lcd_address(page,column+8*i);
			for(k=0;k<8;k++)
				{					
					WriteDat(ascii_table_8x16[n[i]][k]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
	    for(n1=0;n1<2;n1++)
			{			
			lcd_address(page+1,column+8*i);
			for(k=8;k<16;k++)
				{					
					WriteDat(ascii_table_8x16[n[i]][k]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}

	}
		
}*/
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
