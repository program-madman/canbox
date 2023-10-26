#include "12864.h"
#include "delay.h"        //��Ҫʹ��delay_ms()����

/*********************************************************
* Function Name  : lcd_12864_Init
* Description    : 12864��ʼ��
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void lcd_12864_Init(void)
{
	/*����12864��ʼ��GPIO�ڹ���*/ 
  GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
/*--------------------LCD����ָ��-----------------------*/
    delay_ms(2);
    Write_LCD_Command(0x30);  //30--����ָ���
    delay_ms(5);
    Write_LCD_Command(0x0c);  //��ʾ�򿪣����أ����׹�
    delay_ms(5);
    Write_LCD_Command(0x01);  //����
    delay_ms(10);             //����ʱ��ϳ�
    Write_LCD_Command(0x06);  //��������ģʽ         
    delay_ms(5);
	  Write_LCD_Command(0x80);  //����DDRAM��ַ
    delay_ms(5);

	  Draw_Clear();					// ����
}

/*********************************************************
* Function Name  : Send_Byte
* Description    : д���ݵ�LCD
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void Send_Byte(u8 zdata)
{
  u16 i;
  for(i=0; i<8; i++)
  {
  
	  if((zdata << i) & 0x80) 
			SID_H;
	  else   
	    SID_L;
	    SCLK_H;
		  SCLK_L;
	}
}
/*********************************************************
* Function Name  : Receive_Byte
* Description    : ��LCD����
* Input          : None
* Output         : None
* Return         : temp1+temp2
*********************************************************/
u8 Receive_Byte(void)
{
	u8 i,temp1,temp2,value;
	temp1=0;
	temp2=0;
	for(i=0;i<8;i++)
	{
		temp1=temp1<<1;
		SCLK_L;
		SCLK_H;
		SCLK_L;
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
		{
			temp1++;
		}
	}
	for(i=0;i<8;i++)
	{
		temp2=temp2<<1;
		SCLK_L;
		SCLK_H;
		SCLK_L;
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
		{
			temp2++;
		}
	}
	temp1=0xf0&temp1;
	temp2=0x0f&temp2;
	value=temp1+temp2;
	return   value ;
}
/*********************************************************
* Function Name  : Check_Busy
* Description    : LCDæ���
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void Check_Busy(void)
{
	do
		Send_Byte(0xfc);		  //11111,RW(1),RS(0),0
	while(0x80&Receive_Byte());
}
/*********************************************************
* Function Name  : Write_LCD_Command
* Description    : д���LCD
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void Write_LCD_Command(u8 cmdcode)
{ 
	CS_H;
	Check_Busy();
  Send_Byte(0xf8);
  Send_Byte(cmdcode & 0xf0);
  Send_Byte((cmdcode << 4) & 0xf0);
	delay_us(2);
  CS_L;
}
/*********************************************************
* Function Name  : Write_LCD_Data
* Description    : д��ʾ���ݵ�LCD
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void Write_LCD_Data(u8 Dispdata)
{  
  CS_H;
  Check_Busy();
  Send_Byte(0xfa);	  //11111,RW(0),RS(1),0
  Send_Byte(Dispdata & 0xf0);
  Send_Byte((Dispdata << 4) & 0xf0);
  delay_us(2);
  CS_L;
}
/*********************************************************
* Function Name  : Read_LCD_Data
* Description    : ��ȡLCD��ʾ����
* Input          : None
* Output         : None
* Return         : LCD�յ�������
*********************************************************/
u8 Read_Data(void)
{
	Check_Busy();
	Send_Byte(0xfe);    //11111��RW(1),RS(1),0 LCD->MCU
	return  Receive_Byte();
}
/*********************************************************
* Function Name  : LCD_Clear_Txt
* Description    : �ı������
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void LCD_Clear_TXT( void )
{
	u8 i;
	Write_LCD_Command(0x30);      //8BitMCU,����ָ���
	Write_LCD_Command(0x80);      //AC����ʼλ
	for(i=0;i<64;i++)
	{
		Write_LCD_Data(0x20);
	}
}
/*********************************************************
* Function Name  : LCD_Clear_BMP
* Description    : ͼƬ�����
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void LCD_Clear_BMP( void )
{
	u8 i,j;
	Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
	Write_LCD_Command(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
	for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
	{
		Write_LCD_Command(0x80|i);      //��λ��
		Write_LCD_Command(0x80);      //��λ��
		for(j=0;j<32;j++)            //256/8=32 byte
			Write_LCD_Data(0);
	}
}
/*********************************************************
* Function Name  : Display_LCD_Pos
* Description    : ������ʾλ��
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void Display_LCD_Pos(u8 x,u8 y) 
{
	u8 pos;
	switch(x)
	{
		case 0: x=0x80;break;
		case 1: x=0x90;break;
		case 2: x=0x88;break;
		case 3: x=0x98;break;
	}
	pos=x+y;
	Write_LCD_Command(pos);
}
/******************************************
* Function Name  : Disp_HZ
* Description    : ����Һ����ʾ����
								 *addr--��ʾλ�õ��׵�ַ
								 *pt--ָ����ʾ���ݵ�ָ��
								 *num--��ʾ�ַ�����
* Input          : None
* Output         : None
* Return         : None
********************************************/
void Disp_HZ(u8 X,const u8 * pt,u8 num)
{
	u8 i,addr;
		
	if (X==0) {addr=0x80;}
	else if (X==1) {addr=0x90;}
	else if (X==2) {addr=0x88;}
	else if (X==3) {addr=0x98;}
	Write_LCD_Command(addr); 
	for(i = 0;i < (num*2);i++) 
	Write_LCD_Data(*(pt++)); 
} 

/*********************************************************
* Function Name  : displayOneChar
* Description    : ��ָ��λ����ʾһ���ַ�
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void displayOneChar(unsigned char Y, unsigned char X, unsigned char strData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //����X���ܴ���16��Y���ܴ���1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 Write_LCD_Command(X); //���ﲻ���æ�źţ����͵�ַ��
 Write_LCD_Data(strData);
}

/*********************************************************
* Function Name  : displayLcdString
* Description    : ��ʾ�ַ���(û�г��Ȱ�ȫ���)
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void displayLcdString(u8 x,u8 *p,u8 rtime)
{
	u8 i,addr,num;
	num = sizeof p;
	switch(x)
	{
		case 0: addr=0x80;break;
		case 1: addr=0x90;break;
		case 2: addr=0x88;break;
		case 3: addr=0x98;break;
	}
	Write_LCD_Command(addr);
	for(i=0;i<(num*4);i++)
	{
		Write_LCD_Data(*(p++));
		delay_us(rtime);		 
	}
}

/*********************************************************
* Function Name  : displayListChar
* Description    : ��ָ��λ����ʾһ���ַ�(���Ȱ�ȫ���)
* Input          : Y��0-3֮�ڴ�����
* Output         : None
* Return         : None
*********************************************************/
void displayListChar(unsigned char Y, unsigned char X, unsigned char *strData)
{
  unsigned char listLength,addr,maxLength;
  listLength = 0;
	//����X���ܴ���16��Y��0-3֮��
	X &= 0x0F;
	maxLength = 0x0F - X;
	addr = X;
  if( Y > 3 ){
	  Y = 3;
	}
  
  switch( Y ){
	  case 0: addr |= 0X80; break;	//����������ѡ����Ӧ��ַ
	  case 1: addr |= 0X90; break;
	  case 2: addr |= 0X88; break;
	  case 3: addr |= 0X98; break;
	}
	
 Write_LCD_Command(addr); //���͵�ַ��
  while (strData[listLength]!='\0') //�������ִ�β���˳�
  {
   if (listLength <= maxLength) //X����ӦС��0xF
    {
      Write_LCD_Data(strData[listLength]); //
      listLength++;
			delay_us(5);
    }else{
			break;
		}
  }
}

/*********************************************************
* Function Name  : displayImage
* Description    : ͼ����ʾ122*32
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
void displayImage (unsigned char *strData){
	unsigned char x,y,i;
	unsigned int tmp=0;
	for(i=0;i<9;){		//���������ϰ������°�������Ϊ��ʼ��ַ��ͬ����Ҫ�ֿ�
	for(x=0;x<32;x++){				//32��
		Write_LCD_Command(0x34);
		Write_LCD_Command((0x80+x));//�е�ַ
		Write_LCD_Command((0x80+i));	//�е�ַ���°������������е�ַ0X88
		Write_LCD_Command(0x30);		
		for(y=0;y<16;y++)	
			Write_LCD_Data(strData[tmp+y]);//��ȡ����д��LCD
		tmp+=16;		
	}
	i+=8;
	}
	Write_LCD_Command(0x36);	//���书���趨
	Write_LCD_Command(0x30);
}



/*************************************************************
* Function Name  : PutBMP
* Description    : 
* Input          : None
* Output         : None
* Return         : None
**************************************************************/
void PutBMP(u8 *puts)
{
	u16 x=0;
	u8 i,j;
	
	Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
	Write_LCD_Command(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
	for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
	{
		Write_LCD_Command(0x80|i);      //��λ��
		Write_LCD_Command(0x80);      //��λ��
		for(j=0;j<16;j++)      //256/8=32 byte
		{            //��λ��ÿ���Զ�����
			Write_LCD_Data(puts[x]);
			x++;
		}
	}
	for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
	{
		Write_LCD_Command(0x80|i);      //��λ��
		Write_LCD_Command(0x88);      //��λ��
		for(j=0;j<16;j++)      //256/8=32 byte
		{            //��λ��ÿ���Զ�����
			Write_LCD_Data(puts[x]);
			x++;
		}
	}
}

/*-------------------------------------------------
 LCD��ʾ����
--------------------------------------------------*/
u16 LCD12864_Buffer[64][16] = {0};

const u8 m_ch4x5[][6] = 
{
	{0x06,0x09,0x09,0x09,0x06,5},		// 0���ַ����5����
	{0x02,0x06,0x02,0x02,0x07,5},		// 1
	{0x06,0x09,0x02,0x04,0x0F,5},		// 2
	{0x06,0x09,0x02,0x09,0x06,5},		// 3
	{0x02,0x06,0x0A,0x0F,0x02,5},		// 4
	{0x0E,0x08,0x0E,0x01,0x0E,5},		// 5
	{0x06,0x08,0x0E,0x09,0x06,5},		// 6
	{0x0F,0x01,0x02,0x04,0x04,5},		// 7
	{0x06,0x09,0x06,0x09,0x06,5},		// 8
	{0x06,0x09,0x07,0x01,0x06,5},		// 9
	{0x00,0x00,0x00,0x00,0x01,2},		// .
	{0x00,0x01,0x00,0x01,0x00,2},		// :
	{0x00,0x00,0x00,0x00,0x00,5}, 		// �ո�
};
/*------------------------------------------------------------------------------
* Function Name  : Draw_Dots
* Description    : ����
								 *x��������
								 *y��������
								 *color����ɫ��0����ʾ��1��ʾ
* Input          : None
* Output         : None
* Return         : None
------------------------------------------------------------------------------*/
const u16 DrawDotsTable[] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
void Draw_Dots(u8 x, u8 y, u8 color)
{
	u8 i0, y0;
	u16 value;

	// �������
	if ( y >= 32 )
	{
		y0 = y-32;
		i0 = x/16+8;
	}
	else
	{
		y0 = y;
		i0 = x/16;
	}
	value = DrawDotsTable[x%16];
	if (color == 0)
	{
		LCD12864_Buffer[y0][i0] &= ~value;
	}
	else
	{
		LCD12864_Buffer[y0][i0] |= value;
	}
}

/*------------------------------------------------------------------------------
* Function Name  : Draw_Update
* Description    : ˢ����ʾ
* Input          : None
* Output         : None
* Return         : None
------------------------------------------------------------------------------*/
void Draw_Update(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		Write_LCD_Command(0x80|y);
		Write_LCD_Command(0x80);
		for (x=0; x<16; x++)
		{
			Write_LCD_Data(LCD12864_Buffer[y][x]>>8);
			Write_LCD_Data(LCD12864_Buffer[y][x]);
		}
	}
}

/*------------------------------------------------------------------------------
* Function Name  : Draw_Clear
* Description    : ��ʾ����
* Input          : None
* Output         : None
* Return         : None
------------------------------------------------------------------------------*/
void Draw_Clear(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		for (x=0; x<16; x++)
		{
			LCD12864_Buffer[y][x] = 0;
		}
	}
}

/*------------------------------------------------------------------------------
* Function Name  : Draw_Char4X5
* Description    : ��ʾ4*5�ַ�
								 *x��д��������
							 	 *y��д��������
								 *value���ַ�
* Input          : None
* Output         : None
* Return         : None
------------------------------------------------------------------------------*/
void Draw_Char4x5(u8 x, u8 y, u8 value)
{
	u8 i, j, p, z, a;
	
	a = m_ch4x5[value][5];	
	for (i=0; i<5; i++)
	{
		p = 0x10>>(5-a);
		for (j=0; j<a; j ++)
		{
			z = m_ch4x5[value][i];
			if ( z & p )
			{
				Draw_Dots(j+x, i+y, 1);
			}
			p >>= 1;
		}
	}
}

/*--------------------------------------------------------
* Function Name  : Draw_Char4x5Line
* Description    : ��ʾһ��4*5�ַ�
								 *x��д��������
								 *y��д��������
								 *value���ַ���
								 *len������
* Input          : None
* Output         : None
* Return         : None
---------------------------------------------------------*/
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len)
{
	u8 i;

	for (i=0; i<len; i++)
	{
		Draw_Char4x5(x, y, value[i]);
		x += m_ch4x5[value[i]][5];
	}
}
/*--------------------------------------------------------
* Function Name  : clearAlldisplay
* Description    : ���ÿһ��
* Input          : None
* Output         : None
* Return         : None
---------------------------------------------------------*/
void clearAlldisplay(void)
{
	uint8_t i;
	Write_LCD_Command(0x80);
	for(i=0;i<16;i++)         //�����һ��
	{
		Write_LCD_Data(0x20);  
	}
	Write_LCD_Command(0x90);
	for(i=0;i<16;i++)         //����ڶ���
	{
		Write_LCD_Data(0x20);  
	}
	Write_LCD_Command(0x88);
	for(i=0;i<16;i++)         //���������
	{
		Write_LCD_Data(0x20);  
	}  
	Write_LCD_Command(0x98);
	for(i=0;i<16;i++)         //���������
	{
		Write_LCD_Data(0x20);  
	} 
}
/*lcd_Con_X_Y�������������з��׺�����ָ�����*/
u8 const Lcd_Con_X_Y[4][2]={{0x80,0x80},{0x80,0x90},{0x88,0x80},{0x88,0x90}};
/*--------------------------------------------------------
* Function Name  : con_disp
* Description    : ���з���
								 *line�����׵�������ȡֵ0-3
								 *enable���������1Ϊ���ף�0Ϊȡ������
* Input          : None
* Output         : None
* Return         : None
	---------------------------------------------------------*/
void con_disp(unsigned int line,unsigned int enable)
{
  u8 i,j;
	LCD_Clear_BMP();	//�������ͼƬ��RAMҪ���Է���
  for(j=0;j<16;j++)                                      
  {
    for(i=0;i<8;i++)                                  
    {
			Write_LCD_Command(0x34);      
			Write_LCD_Command(0x36);   
						
			Write_LCD_Command(Lcd_Con_X_Y[line][1]+j);
      Write_LCD_Command(Lcd_Con_X_Y[line][0]+i);
      Write_LCD_Command(0x30);
			
      if(enable==1)                                 
      {
         Write_LCD_Data(0xff);
         Write_LCD_Data(0xff);
				
      }
      else                                                 
      {
					Write_LCD_Data(0x00);
          Write_LCD_Data(0x00);
      }
    }
  }
	
	Write_LCD_Command(0x34);      
  Write_LCD_Command(0x36);
  Write_LCD_Command(0x30); 
		
}

/*--------------------------------------------------------
* Function Name  : con_disp8
* Description    : ���������ֿ�ȷ���/ȡ�����׺���
								 *	x�������꣬ȡֵΪ����һ�к͵ڶ���0x80~0x87�������е�����Ϊ0x88~0x8f��
								 *	y�������꣬ȡֵΪ����һ�к͵����У�0x80���ڶ��к͵�����0x8f��
								 *	direction:=1Ϊ16*16���Ұ�����=0Ϊ�����
								 *	eg:xȷ���У�yȷ���У�directionȷ���ַ������Ұ��
* Input          : None
* Output         : None
* Return         : None
	---------------------------------------------------------*/

void con_disp8(unsigned int x,unsigned int y,unsigned int direction)
{
	u8 j;
	LCD_Clear_BMP();								//�������ͼƬ��RAMҪ���Է���
	for(j=0;j<16;j++)                                       
	{
		
		Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
		Write_LCD_Command(0x36);       //��ͼON,����ָ�����36H���ܿ���ͼ 		
		Write_LCD_Command(y+j);
		Write_LCD_Command(x);
		Write_LCD_Command(0x30);				//30--����ָ���
		if(direction==1)                                 
		{ 
			
			Write_LCD_Data(0x00);
			Write_LCD_Data(0xFF);
		}
		else                                                         
		{                       
			Write_LCD_Data(0xFF);
			Write_LCD_Data(0x00);
		}
  }
	Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
  Write_LCD_Command(0x36);
  Write_LCD_Command(0x30);
}

