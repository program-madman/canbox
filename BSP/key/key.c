#include "key.h"
#include "delay.h"
	   
//������ʼ������ 
void key_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTCʱ��

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	return;
} 

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//ע��˺�������Ӧ���ȼ�,KEYUP>KEYDWN>WK_LEF!!

u8 keyScan(u8 mode)
{	 
	static u8 key_up = 1;//�������ɿ���־
	
	if(mode){
		key_up=1;  //֧������		 
	}		
	
	if( key_up && ( KEY_UP==0||KEY_DWN==0||KEY_LEF==0||KEY_RHT==0||KEY_MID==0||KEY_SET==0||KEY_RST==0)){
		
		delay_ms(10);//ȥ���� 
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
	
	return 0;// �ް�������
	
}
