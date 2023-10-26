/**********************************************************************************
 * ������  CAN BOX TOOl   
 * ��汾  ��ST_v3.5
**********************************************************************************/

#define TOOL_NAME  "CAN BOX TOOl"
#define VERSION    "Version:v1.0"
#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "12864.h"
#include "menu.h"
#include "can.h"


void printfCanRecv(u8 key,u8 *canbuf){
	//����log
	u8 t;
	printf("\r\nCan Recv:\r\nHEX:");
	for(t=0;t<key;t++)
	{									    
		printf(" %02X ",canbuf[t]);
	}
	printf("\r\nOCT:");
	for(t=0;t<key;t++)
	{									    
		printf("%03d ",canbuf[t]);
	}
	printf("\r\n");
}

/*******************************************************************************
* ������  : main
* ����    : ���������û������main������ʼ����
* ����    : ��
* ���    : ��
* ����ֵ  : int:����ֵΪһ��16λ������
* ˵��    : ��
*******************************************************************************/
int main(void)
{
	//���ڶ������
	u8 t;
	u8 len;	
	u16 times = 0; 
	u8 keyValue = 0;
	//can�������
	u8 key;
	//CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
	u8 mode = 0;          
	u8 canbuf[8];
	
	delay_init();       	//��ʼ��delay
	uart_init(9600);      //USART1��ʼ������
	led_Init();         	//LED IO����
	key_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	lcd_12864_Init();     //LCD12864��ʼ��
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_2tq,CAN_BS1_3tq,12,mode);//CAN��ʼ��,������500Kbps 
	initmenu();
	
	printf("\r\n %s \r\n %s \r\n",TOOL_NAME,VERSION);
	printf("\r\n init complete ��\r\n");
	displayListChar(1,0,TOOL_NAME);
	displayListChar(2,0,VERSION);
	
	while(1){
		
		//���ڴ�����Ϣ
		if ( USART_RX_STA & 0x8000){					   
			//�õ��˴ν��յ������ݳ���
			len = USART_RX_STA & 0x3fff;   
			printf("\r\n���ѷ�����ϢΪ:\r\n");
			for(t=0;t<len;t++){
				USART1->DR=USART_RX_BUF[t];
				//�ȴ����ͽ���
				while((USART1->SR&0X40)==0);
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
		}else{
			times++;
			if(times%5000==0){
				printf("\r\n ���Դ���log:5000\r\n");
			}
			if(times % 200 ==0){
				printf("��������,��ͨ�����ڷ���ָ��.\r\n");
			}
			if(times % 30 ==0){ 
				//��˸LED,��ʾϵͳ��������.
				LED=!LED;  
			}		
			delay_ms(10);   
		}
		
		//��������
		keyValue = keyScan(0);
			
		//��Ϣ�Լ���ʾ����
		if(keyValue != 0){
			printf("\r\nkey Value��%d",keyValue);
			menuHandleKey(keyValue);
		}
		
		key = Can_Receive_Msg(canbuf);
		//���յ�������
		if(key)
		{			
			printfCanRecv(key,canbuf);
			displayRecvCanSinnal(key,canbuf);
		}
	}
}
