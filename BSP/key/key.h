#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����0
#define KEY_DWN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����0
#define KEY_LEF GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0
#define KEY_RHT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����0
#define KEY_MID GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����0
#define KEY_SET GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����0
#define KEY_RST GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����0

#define KEY_UP_PRES  1 //��ȡ����0
#define KEY_DWN_PRES 2 //��ȡ����0
#define KEY_LEF_PRES 3 //��ȡ����0
#define KEY_RHT_PRES 4 //��ȡ����0
#define KEY_MID_PRES 5 //��ȡ����0
#define KEY_SET_PRES 6 //��ȡ����0
#define KEY_RST_PRES 7 //��ȡ����0

void key_Init(void);//IO��ʼ��
u8 keyScan(u8 mode);  	//����ɨ�躯��					    
#endif
