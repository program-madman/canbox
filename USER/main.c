/**********************************************************************************
 * 工程名  ：eboCan     
 * 库版本  ：ST_v3.5
**********************************************************************************/

#define TOOL_NAME  "EBO_CAN TOOl"
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
	//串口log
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
* 函数名  : main
* 描述    : 主函数，用户程序从main函数开始运行
* 输入    : 无
* 输出    : 无
* 返回值  : int:返回值为一个16位整形数
* 说明    : 无
*******************************************************************************/
int main(void)
{
	//串口定义变量
	u8 t;
	u8 len;	
	u16 times = 0; 
	u8 keyValue = 0;
	//can定义变量
	u8 key;
	//CAN工作模式;0,普通模式;1,环回模式
	u8 mode = 0;          
	u8 canbuf[8];
	
	delay_init();       	//初始化delay
	uart_init(9600);      //USART1初始化配置
	led_Init();         	//LED IO配置
	key_Init();          	//初始化与按键连接的硬件接口
	lcd_12864_Init();     //LCD12864初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_2tq,CAN_BS1_3tq,12,mode);//CAN初始化,波特率500Kbps 
	initmenu();
	
	printf("\r\n %s \r\n %s \r\n",TOOL_NAME,VERSION);
	printf("\r\n init complete ！\r\n");
	displayListChar(1,0,TOOL_NAME);
	displayListChar(2,0,VERSION);
	
	while(1){
		
		//串口处理消息
		if ( USART_RX_STA & 0x8000){					   
			//得到此次接收到的数据长度
			len = USART_RX_STA & 0x3fff;   
			printf("\r\n您已发送消息为:\r\n");
			for(t=0;t<len;t++){
				USART1->DR=USART_RX_BUF[t];
				//等待发送结束
				while((USART1->SR&0X40)==0);
			}
			printf("\r\n\r\n");
			USART_RX_STA=0;
		}else{
			times++;
			if(times%5000==0){
				printf("\r\n 调试串口log:5000\r\n");
			}
			if(times % 200 ==0){
				printf("正在运行,可通过串口发送指令.\r\n");
			}
			if(times % 30 ==0){ 
				//闪烁LED,提示系统正在运行.
				LED=!LED;  
			}		
			delay_ms(10);   
		}
		
		//按键处理
		keyValue = keyScan(0);
			
		//消息以及显示处理
		if(keyValue != 0){
			printf("\r\nkey Value：%d",keyValue);
			menuHandleKey(keyValue);
		}
		
		key = Can_Receive_Msg(canbuf);
		//接收到有数据
		if(key)
		{			
			printfCanRecv(key,canbuf);
			displayRecvCanSinnal(key,canbuf);
		}
	}
}
