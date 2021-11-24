#include "led.h"
#include "delay.h"
#include "sys.h"
//#include "usart.h"
#include "oled.h"
#include "pubdata.h"
#include "ds18b20.h"   
//ALIENTEK Mini STM32�����巶������23
//DS18B20�����¶ȴ�����ʵ��  
//ds18b20.c .h��Դ��csdn���أ�https://download.csdn.net/download/zhou0842/9699352������֪��ԭ���߻�ӭ��֪
//
////////////////////////////////////////////////
//
//			�޸��ߣ�berry 2021.11.23
//			ʹ��˵���� oled ���� PB13��SCL�� PB15��SDA��
//			ds18b20ʹ�õ�����ģʽ���������ǰ��ȡ�Լ��Ĵ������ĵ�ַ��debugģʽ���Կ�����
//			��ַ��ÿ�ε�����һ������������debug��鿴ID��ֵ�������ĵ�ַ���ϵ�������while1�Ϳ��ԣ�
//			���ۿ��ԽӺܶ�ܶഫ���������д���������PA0������
//
///////////////////////////////////////////////
//
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 int main(void)
 { 
	u8 t=0;		
	u8 flag;	 
	short temperature; 
	double temp;
	u8 ID[8], i;
	u8 ID1[8] = {0x28,0xFF,0XE2,0X93,0X67,0X18,0X01,0X63};//��������64λ��ַ
	u8 ID2[8] = {0x28,0xee,0x1b,0x1a,0x33,0x16,0x01,0x29};
	u8 ID3[8] = {0x28,0xff,0x7d,0x93,0x67,0x18,0x01,0xbb};
	u8 ID4[8] = {0x28,0xee,0x8c,0x49,0x33,0x16,0x01,0x9f};
	u8 temps[4];
	//u8 IDS[4] = {&ID1,&ID2,&ID3,&ID4};
	u8* IDS[4];//���4·�������ĵ�ַ
	u32 id;

	u8 ret = 0;	 
	delay_init();	    	 //��ʱ������ʼ��	  
	//uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	 OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	 	OLED_Refresh();
	 OLED_Clear();

	
	
	ret = DS18B20_GetID(ID);//ÿ�ε�����һ������������debug��鿴ID��ֵ�������ĵ�ַ
	
	if ( ret == 1 )
	{
		OLED_ShowString(0,0,"not find ds18b20",16);
	}
	else
	{

		
	OLED_ShowString(0,0,"ID1:",16);
	
	for ( i = 0; i < 8; i++ )
		{
			
			OLED_ShowNum(30+i*9,0,ID1[i],1,16);
		}
	
	}
	

	
	
	OLED_Refresh();//�����Դ浽OLED	
	
		
	
	IDS[0] = ID1;//�Ҳ��ǻ��ID1[]�������ʼ��ַ�Ž�ȥ
	IDS[1] = ID2;
	IDS[2] = ID3;
	IDS[3] = ID4;
	
	
	
	
	
	
	
while(1)
	{	    	   


	
	for ( i = 0; i < 4; i++ )//����4·������
		{
			

		ret = DS18B20_GetTemp(IDS[i], &temp);//����������յĺ�����һ���������ʼ��ַ

		if ( ret == 1 )
		{	
			//printf("û�з����豸!\r\n");
			OLED_ShowString(0,16,"no sd18b20!",16);
			OLED_Refresh();//�����Դ浽OLED	
			delay_ms(500);

		}
		else
		{
			//printf("��ǰ�����¶� : %0.4f ��\r\n", temp);
				temp=(int)temp;//����ת��
			OLED_ShowString(0,16,"temp",16);
			OLED_ShowNum(35,16,i+1,1,16);
			OLED_ShowString(45,16,":",16);
			OLED_ShowNum(55,16,temp,2,16);
			OLED_Refresh();//�����Դ浽OLED	
			temps[i] = temp;
		}

		OLED_Refresh();//�����Դ浽OLED	

	//	LED0=!LED0;
		delay_ms(20);
	}

	
	
	
	
	//��������/////////////////////////////////////////////////////////
	//
	//������һ·�¶ȳ���23��ʱ������oled��ʾ������ţ�λ�ã�
	//
	//����������for i����������ţ���Ҫ�Ķ���ע�����i����
	OLED_ShowString(0,35,"alarm:",16);
	OLED_ShowString(55,35,"     ",16);
	flag = 0;
	for ( i = 0; i < 4; i++ )
		{
			if(temps[i] > 23)
			{
				flag = 1;
				
				OLED_ShowNum(55+i*9,35,i+1,1,16);
				
			}	
			
		}
	OLED_Refresh();//�����Դ浽OLED	
	LED0 = !flag; //ȡ��flag
		
		//��������////////////////////////////////////////////////////////////////////
		
		
		
	}
}
