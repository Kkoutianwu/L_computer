#ifdef __cplusplus
extern "C" {
#endif

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

#define SCL_L         GPIOB->BRR = GPIO_Pin_4
#define SCL_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR = GPIO_Pin_3
#define SDA_H         GPIOB->BSRR = GPIO_Pin_3
#define SDA_READ      (GPIOB->IDR&GPIO_Pin_3) >>3
void IIC_Configuation(void);
void SDA_Out(void);
void SDA_In(void);
void IIC_Start(void);
void oled_delay_us(u16 time);
void IIC_Stop(void);
unsigned char Read_Ask(void);
void Write_OneByte(unsigned char Dat);

void WriteCmd(unsigned char IIC_Cmd);
void WriteDat(unsigned char IIC_Dat);
void OLED_Init(void);
void OLED_SetPos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x,unsigned char y,const char ch[],unsigned char TextSize);
void lcd_address(u8 page,u8 column);
void clear_screen(void);
/*void display_128x64(u8 *dp);
void display_128x16(u8 page,u8 column,u8 *dp);
void display_graphic_32x32(u8 page,u8 column,const u8 *dp);
void display_graphic_16x16(u8 page,u8 column,u8 *dp);
void display_graphic_16x8(u8 page,u8 column,u16 *dp);
void display_graphic_128x64(u8 page,u8 column,u8 *dp);
void display_graphic_8x16(u8 page,u8 column,u8 *dp);
void display_string_8x16(u16 page,u16 column,u8 *text);
void display_string_5x8(u16 page,u16 column,u8 reverse,const char *text);
void display_string_16x16(u8 page,u8 column,u8 *text);
void disp_string_8x16_16x16(u8 page,u8 column,const u8 *text);
void display_number_16x8(u8 page,u8 column,u16 number);*/
#endif  
#ifdef __cplusplus
}
#endif
	 







 

