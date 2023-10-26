#include "menu.h"
#include "stm32f10x.h"
#include "12864.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"
#include "can.h"

/*�˵��ṹ��*/
typedef struct menu
{
 u8 rangeFrom;//��ǰѡ����ʼ����
 u8 rangeTo; //��ǰѡ������������
 u8 itemCount;//��Ŀ����
 u8 itemSelected;//��ǰѡ����
 u8 *menuItems[17];//�˵���Ŀ
 struct menu **subMenus;//�Ӳ˵�
 struct menu *parent;//�ϼ��˵����������˵���Ϊnull
 void (**func)();//ѡ����Ӧ������ִ�еĺ���

}Menu; 

Menu *currentMenu;//��ǰ�˵�
static u8 hasJumpOutMenu = 1;
static u8 isRecvOpen = 0;
static int speed = 30;//��ʼ�ٶ�

Menu mainMenu = { 
 0,3,4,0,						//Ĭ����ʾ0-3��ܹ�4���ǰѡ��0�
  {
  "Send signal    \x10",//"\x10"��ʾһ��С��ͷ
  "Set can mode   \x10",
  "Automated test \x10",
  "Setting",
  }
}; 

Menu canMenu = { 
 0,3,15,0,						//Ĭ����ʾ0-3��ܹ�4���ǰѡ��0�
  {
  "P-GEAR         \x10",//"\x10"��ʾһ��С��ͷ
	"R-GEAR         \x10",
	"N-GEAR         \x10",
	"D-GEAR         \x10",
	"SPEED          \x10",
	"ACC ON         \x10",
	"ACC OFF        \x10",
  "VOLUM +        \x10",
  "VOLUM -        \x10",
	"PRE            \x10",
	"NEXT           \x10",
	"VOICE          \x10",
	"PAGE UP        \x10",
	"PAGE DOWN      \x10",
	"HANG UP        \x10",
  }
};

Menu disMenu = { 
 0,3,4,0,						//Ĭ����ʾ0-3��ܹ�4���ǰѡ��0�
  {
  "Normal         \x10",//"\x10"��ʾһ��С��ͷ
  "LoopBack       \x10",
  "OnlyRecv",
  "OnlySend",
  }
}; 

void printfCanSent(u8 key,u8 *canbuf){
	//����log
	u8 t;
	printf("\r\nCan Sent:\r\nHEX:");
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

void func(void){}
//P-GEAR
void funcSendCanP_Gear(void){
	u8 res;
	u8 canbuf[8]={0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x394,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent P-GEAR");
	printf("\r\nsend cmd1 \r\n");
	printfCanSent(8,canbuf);
}
//R-GEAR
void funcSendCanR_Gear(void){
  u8 res;
	u8 canbuf[8]={0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x394,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent R-GEAR");
	printf("\r\nsend cmd2 \r\n");
	printfCanSent(8,canbuf);
}
//N-GEAR
void funcSendCanN_Gear(void){
  u8 res;
	u8 canbuf[8]={0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x394,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent N-GEAR");
	printf("\r\nsend cmd3 \r\n");
	printfCanSent(8,canbuf);
}
//D-GEAR
void funcSendCanD_Gear(void){
  u8 res;
	u8 canbuf[8]={0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x394,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent D-GEAR");
	printf("\r\nsend cmd4 \r\n");
	printfCanSent(8,canbuf);
}
//SPEED++
void funcSendCanSpeed(void){
  u8 res;
	u8 canbuf[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	speed++;
	canbuf[3] = (speed * 100) & 0x00ff;
	canbuf[4] = ((speed * 100) & 0xff00)>>8; 
	res = DevCan_Send_Msg(0x3da,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent SPEED");
	printf("\r\nsend cmd5 \r\n");
	printfCanSent(8,canbuf);
}

//SPEED--
void funcSendCanSpeedSub(void){
  u8 res;
	u8 canbuf[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	speed--;
	canbuf[3] = (speed * 100) & 0x00ff;
	canbuf[4] = ((speed * 100) & 0xff00)>>8; 
	res = DevCan_Send_Msg(0x3da,canbuf,8);//����8���ֽ�
	displaySentCanSinnal(res,8,canbuf,"Sent SPEED");
	printf("\r\nsend cmd5 \r\n");
	printfCanSent(8,canbuf);
}

//ACC ON
void funcSendCanAccOn(void){
	u8 res;
	u8 canbuf[4]={0x00,0x00,0x01,0x00};
	u8 canbuf1[4]={0x00,0x00,0x03,0x00};
	res = DevCan_Send_Msg(0x3C0,canbuf,4);//����4���ֽ�
	delay_ms(1000);
	res = DevCan_Send_Msg(0x3C0,canbuf1,4);//����4���ֽ�
	//funcSendCanEgOff();
	displaySentCanSinnal(res,4,canbuf,"Sent ACC ON");
	printf("\r\nsend cmd6 \r\n");
	printfCanSent(4,canbuf);
}
//ACC OFF
void funcSendCanAccOff(void){
	u8 res;
	u8 canbuf[4]={0x00,0x00,0x01,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x3C0,canbuf,4);//����4���ֽ�
	delay_ms(3000);
	res = DevCan_Send_Msg(0x3C0,canbuf1,4);//����4���ֽ�
	displaySentCanSinnal(res,4,canbuf1,"Sent ACC OFF");
	printf("\r\nsend cmd7 \r\n");
	printfCanSent(4,canbuf1);
}

//VOLUM+
void funcSendCanVolumUp(void){
	u8 res;
	u8 canbuf[4]={0x10,0x00,0x00,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(500);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent VOLUM+");
	printf("\r\nsend cmd8 \r\n");
	printfCanSent(4,canbuf);
}
//VOLUM-
void funcSendCanVolumDown(void){
	u8 res;
	u8 canbuf[4]={0x11,0x00,0x00,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(500);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent VOLUM-");
	printf("\r\nsend cmd9 \r\n");
	printfCanSent(4,canbuf);
}
//PRE
void funcSendCanPre(void){
	u8 res;
	u8 canbuf[4]={0x0a,0x00,0x00,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(300);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent PRE");
	printf("\r\nsend cmd10 \r\n");
	printfCanSent(4,canbuf);
}
//NEXT
void funcSendCanNext(void){
	u8 res;
	u8 canbuf[4]={0x0b,0x00,0x00,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(300);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent NEXT");
	printf("\r\nsend cmd11 \r\n");
	printfCanSent(4,canbuf);
}

//VOICE
void funcSendVoice(void){
	u8 res;
	u8 canbuf[4]={0x19,0x00,0x00,0x00};
	u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(500);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent VOICE");
	printf("\r\nsend cmd12 \r\n");
	printfCanSent(4,canbuf);
}

//PAGE UP
void funcSendPageUp(void){
	u8 res;
	u8 canbuf[4]={0x16,0x00,0x00,0x00};
  u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(300);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent PAGE UP");
	printf("\r\nsend cmd10 \r\n");
	printfCanSent(4,canbuf);
}

//PAGE DOWN
void funcSendPageDown(void){
	u8 res;
  u8 canbuf[4]={0x15,0x00,0x00,0x00};
  u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(300);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent PAGE DOWN");
	printf("\r\nsend cmd11 \r\n");
	printfCanSent(4,canbuf);
}

//HANG UP
void funcSendHangUp(void){
	u8 res;
  u8 canbuf[4]={0x1C,0x00,0x00,0x00};
  u8 canbuf1[4]={0x00,0x00,0x00,0x00};
	res = DevCan_Send_Msg(0x5bf,canbuf,4);//����8���ֽ�
	delay_ms(300);
	res = DevCan_Send_Msg(0x5bf,canbuf1,4);//����8���ֽ�
	displaySentCanSinnal(res,4,canbuf,"Sent NEXT");
	printf("\r\nsend cmd11 \r\n");
	printfCanSent(4,canbuf);
}


void funcAutomatedTest(){
	u32 times;
	u32 t;
	u8 str[20]="0";
	times = 0;
	while(1){
	
	  times++;
	
		funcSendCanAccOn();
		clearAlldisplay();
		
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","�ѷ��Ϳ����źţ�");
		displayListChar(2,0,str);
		
		memset(str,0,sizeof(str));
		sprintf((char*)str,"���Դ���:%u",times);
		displayListChar(1,0,str);
		
		for(t= 0;t<140;t++){
			delay_ms(1000);
			memset(str,0,sizeof(str));
			sprintf((char*)str,"��ʱ 140s:%us",t);
		  displayListChar(3,0,str); 
		}
		
		funcSendCanAccOff();
		clearAlldisplay();
		
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","�ѷ��͹ػ��źţ�");
		displayListChar(2,0,str);
		
		memset(str,0,sizeof(str));
		sprintf((char*)str,"���Դ���:%u",times);
		displayListChar(1,0,str);
		
		for(t=0;t<40;t++){
			delay_ms(1000);
			memset(str,0,sizeof(str));
			sprintf((char*)str,"��ʱ 40s:%us",t);
		  displayListChar(3,0,str); 
		}
	
	}
}


void initmenu(void)
{
	currentMenu = &mainMenu;
	
	//������˵�
	mainMenu.subMenus = malloc(sizeof(&mainMenu)*4);
	mainMenu.subMenus[0] = &canMenu;
	mainMenu.subMenus[1] = &disMenu;
	mainMenu.subMenus[2] = NULL;
	mainMenu.subMenus[3] = NULL;
	
	mainMenu.func = malloc(sizeof(&func)*4);
	mainMenu.func[0] = NULL;
	mainMenu.func[1] = NULL;
	mainMenu.func[2] = funcAutomatedTest;
	mainMenu.func[3] = NULL;

	mainMenu.parent = NULL;//mainmenu is top menu
	
	//����һ���Ӳ˵� can�ź�
	canMenu.subMenus = malloc(sizeof(&canMenu)*15);
	canMenu.subMenus[0] = NULL;
	canMenu.subMenus[1] = NULL;
	canMenu.subMenus[2] = NULL;
	canMenu.subMenus[3] = NULL;
	canMenu.subMenus[4] = NULL;
	canMenu.subMenus[5] = NULL;
	canMenu.subMenus[6] = NULL;
	canMenu.subMenus[7] = NULL;
	canMenu.subMenus[8] = NULL;
	canMenu.subMenus[9] = NULL;
	canMenu.subMenus[10] = NULL;
	canMenu.subMenus[11] = NULL;
	canMenu.subMenus[12] = NULL;
	canMenu.subMenus[13] = NULL;
	canMenu.subMenus[14] = NULL;
	
	canMenu.func = malloc(sizeof(&func)*15);
	canMenu.func[0] = funcSendCanP_Gear;
	canMenu.func[1] = funcSendCanR_Gear;
	canMenu.func[2] = funcSendCanN_Gear;
	canMenu.func[3] = funcSendCanD_Gear;
	canMenu.func[4] = funcSendCanSpeed;
	canMenu.func[5] = funcSendCanAccOn;
	canMenu.func[6] = funcSendCanAccOff;
	canMenu.func[7] = funcSendCanVolumUp;
	canMenu.func[8] = funcSendCanVolumDown;
	canMenu.func[9] = funcSendCanPre;
	canMenu.func[10] = funcSendCanNext;
	canMenu.func[11] = funcSendVoice;
	canMenu.func[12] = funcSendPageUp;
	canMenu.func[13] = funcSendPageDown;
	canMenu.func[14] = funcSendHangUp;


	
	
	canMenu.parent = &mainMenu;//canMenu parent menu is mainmenu1
	
	//���ڶ����Ӳ˵� ����
	disMenu.subMenus =  malloc(sizeof(&canMenu)*4);
	disMenu.subMenus[0] = NULL;
	disMenu.subMenus[1] = NULL;
	disMenu.subMenus[2] = NULL;
	disMenu.subMenus[3] = NULL;
	
	disMenu.func = malloc(sizeof(&func)*4);
	disMenu.func[0] = NULL;
	disMenu.func[1] = NULL;
	disMenu.func[2] = NULL;
	disMenu.func[3] = NULL;
	
	disMenu.parent = &mainMenu;//disMenu parent menu is mainmenu1
	
}

/*���ڴ�����*/
void menuHandleKey(u8 ReadValue){
	
	if(hasJumpOutMenu > 0){
		 hasJumpOutMenu = 0;
		 displaymenu(currentMenu->itemSelected);
		 printf("\r\nCmd run out,return menu.");
		 return;
	}
	
	switch(ReadValue){
		
		case KEY_RHT_PRES://�����ң�ȷ�ϰ�����
			{
				if(currentMenu->subMenus[currentMenu->itemSelected] !=NULL)
				{
				   currentMenu = currentMenu->subMenus[currentMenu->itemSelected];
				   displaymenu(currentMenu->itemSelected);
				}else{
					if(currentMenu->func[currentMenu->itemSelected] !=NULL){
						currentMenu->func[currentMenu->itemSelected]();
					}
				}
				break;
			}
		case KEY_UP_PRES://�����ϣ���һ�в˵���
			{
				if(currentMenu->itemSelected == 0)
					break;
				else{
					currentMenu->itemSelected--;
					
					if(currentMenu->itemSelected < currentMenu->rangeFrom)
					{
						currentMenu->rangeFrom = currentMenu->itemSelected;
						currentMenu->rangeTo = currentMenu->rangeFrom+3;
					}
					displaymenu(currentMenu->itemSelected);

				}	
				break;
			}
		case KEY_DWN_PRES://�����£���һ�в˵� ��
			{
				if(currentMenu->itemSelected == currentMenu->itemCount-1)
					break;
				else
				{
					currentMenu->itemSelected++;
					if(currentMenu->itemSelected >currentMenu->rangeTo)
					{
						currentMenu->rangeTo = currentMenu->itemSelected;
						currentMenu->rangeFrom = currentMenu->rangeTo-3;
					}
					displaymenu(currentMenu->itemSelected);

				}
					break;
			}	
		case KEY_LEF_PRES://�����󣨷��أ�
			{
				if(currentMenu->parent!=NULL)
				{
					currentMenu = currentMenu->parent;
					displaymenu(currentMenu->itemSelected);
				}
				break;
			}
			
		case KEY_SET_PRES://�л�ģʽ����
			{
				if(isRecvOpen == 0){
					isRecvOpen = 1;
					printf("\r\nRecv has opened");
				}else{
					isRecvOpen = 0;
					printf("\r\nRecv has closed");
				}
				displayRecvMode();
				break;
			}
		case KEY_RST_PRES://��λ����
		{
		    NVIC_SystemReset();
				break;
		}
			
		default:
			displaymenu(currentMenu->itemSelected);
		   break;
	}
	return;
}

/*������ʾ�˵�*/
void displaymenu(u8 line)
{
	int i;
	clearAlldisplay();
	
	line = 3-(currentMenu->rangeTo - line);
	/*4��ѭ����ӡ�˵�*/
	for(i = 0;i<4;i++){
		displayListChar(i,0,currentMenu->menuItems[i+currentMenu->rangeFrom]);
	}
	con_disp(line,1);
}

void displayRecvMode(void){
	
		clearAlldisplay();
	  hasJumpOutMenu =1;
	
		displayListChar(0,0,"CanRecv Changed:");
		displayListChar(1,0,"Can RECV:       ");
		if(isRecvOpen > 0){
		displayListChar(2,0,"has opened!     ");
		}else{
		displayListChar(2,0,"has closed!     ");
		}
		
		displayListChar(3,0,"                ");
}

void displayRecvCanSinnal(u8 key,u8 *canbuf){
		u8 t;
		u8 str[20]="0";
		
		if(isRecvOpen == 0){
			printf("\r\n Recv Mode has Closed");
			return;
		}
	
		clearAlldisplay();
		hasJumpOutMenu =1;
		
		displayListChar(0,0,"RECV can signal:");
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","Hex1:");
		for(t=0;t<key/2;t++)
		{									    
				sprintf((char*)str+(3*t+5),"%02X  ",canbuf[t]);
		}
		displayListChar(2,0,str);
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","Hex2:");
		for(t=0;t<key/2;t++)
		{									    
				sprintf((char*)str+(3*t+5),"%02X  ",canbuf[t+key/2]);
		}
		displayListChar(3,0,str);
}

void displaySentCanSinnal(u8 res,u8 key,u8 *canbuf,u8 *cmdName){
		u8 t;
		u8 str[20]="0";
	
		clearAlldisplay();
	  hasJumpOutMenu =1;
	
		displayListChar(0,0,cmdName);
		if(res){
		 	displayListChar(1,0,"Failed:!        ");
		}else{
			displayListChar(1,0,"Success:!       ");
		}
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","Hex1:");
		for(t=0;t<key/2;t++)
		{									    
				sprintf((char*)str+(3*t+5),"%02X  ",canbuf[t]);
		}
		displayListChar(2,0,str);
		memset(str,0,sizeof(str));
		sprintf((char*)str,"%s","Hex2:");
		for(t=0;t<key/2;t++)
		{									    
				sprintf((char*)str+(3*t+5),"%02X  ",canbuf[t+key/2]);
		}
		displayListChar(3,0,str);
}
