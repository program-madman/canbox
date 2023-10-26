#ifndef __12864_H
#define __12864_H

#include "stm32f10x.h"

/*
*LCD12864串行三线控制引脚定义
*CS  ->RS  (PIN5)
*SID ->R/W (PIN6)
*SCLK->E   (PIN7)
*NOTE:移植只需要修改这三个引脚对应的IO口即可，并修改IO初始化
*/
#define CS_H GPIO_SetBits(GPIOB,GPIO_Pin_5)       //RS=1   
#define CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_5)     //RS=0

#define SID_H GPIO_SetBits(GPIOB,GPIO_Pin_6)      //RW=1    
#define SID_L GPIO_ResetBits(GPIOB,GPIO_Pin_6);   //RW = 0

#define SCLK_H GPIO_SetBits(GPIOB,GPIO_Pin_7);    //EN = 1   
#define SCLK_L GPIO_ResetBits(GPIOB,GPIO_Pin_7);  //EN = 0

//函数声明
void lcd_12864_Init(void);
void Send_Byte(u8 zdata);
u8 Receive_Byte(void);
void Check_Busy(void);
void Write_LCD_Command(u8 cmdcode);
void Write_LCD_Data(u8 Dispdata);
u8 Read_Data(void);
void LCD_Clear_TXT( void );
void LCD_Clear_BMP( void );
void Display_LCD_Pos(u8 x,u8 y) ;
void Draw_dots(u8 x,u8 y);
void Disp_HZ(u8 X,const u8 * pt,u8 num);
void displayLcdString(u8 x,u8 *p,u8 rtime);	 
void PutBMP(u8 *puts);
void Draw_Dots(u8 x, u8 y, u8 color);
void Draw_Update(void);
void Draw_Clear(void);
void Draw_Char4x5(u8 x, u8 y, u8 value);
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len);

void displayOneChar(unsigned char Y, unsigned char X, unsigned char DData);
void displayListChar(unsigned char Y, unsigned char X, unsigned char *DData);
void displayImage (unsigned char *DData);

void clearAlldisplay(void);
void con_disp (unsigned int line,unsigned int enable);
void con_disp8(unsigned int x,unsigned int y,unsigned int direction);

#endif
