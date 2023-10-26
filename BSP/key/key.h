#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键0
#define KEY_DWN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键0
#define KEY_LEF GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0
#define KEY_RHT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取按键0
#define KEY_MID GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键0
#define KEY_SET GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键0
#define KEY_RST GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读取按键0

#define KEY_UP_PRES  1 //读取按键0
#define KEY_DWN_PRES 2 //读取按键0
#define KEY_LEF_PRES 3 //读取按键0
#define KEY_RHT_PRES 4 //读取按键0
#define KEY_MID_PRES 5 //读取按键0
#define KEY_SET_PRES 6 //读取按键0
#define KEY_RST_PRES 7 //读取按键0

void key_Init(void);//IO初始化
u8 keyScan(u8 mode);  	//按键扫描函数					    
#endif
