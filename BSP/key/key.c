#include "key.h"
#include "delay.h"
	   
//按键初始化函数 
void key_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	return;
} 

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//注意此函数有响应优先级,KEYUP>KEYDWN>WK_LEF!!

u8 keyScan(u8 mode)
{	 
	static u8 key_up = 1;//按键按松开标志
	
	if(mode){
		key_up=1;  //支持连按		 
	}		
	
	if( key_up && ( KEY_UP==0||KEY_DWN==0||KEY_LEF==0||KEY_RHT==0||KEY_MID==0||KEY_SET==0||KEY_RST==0)){
		
		delay_ms(10);//去抖动 
		key_up=0;
		
		if(KEY_UP==0)
			return KEY_UP_PRES;
		else if(KEY_DWN==0)
			return KEY_DWN_PRES;
		else if(KEY_LEF==0)
			return KEY_LEF_PRES; 
		else if(KEY_RHT==0)
			return KEY_RHT_PRES; 
		else if(KEY_MID==0)
			return KEY_MID_PRES; 
		else if(KEY_SET==0)
			return KEY_SET_PRES; 
		else if(KEY_RST==0)
			return KEY_RST_PRES;  
		
	}else if(KEY_UP==1&&KEY_DWN==1&&KEY_LEF==1&&KEY_RHT==1&&KEY_MID==1&&KEY_SET==1&&KEY_RST==1){
		key_up=1; 
	}
	
	return 0;// 无按键按下
	
}
