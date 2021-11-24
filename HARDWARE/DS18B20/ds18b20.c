
#include "pubdata.h"

/* �ܽ����� */
void DS18B20_DQ_High(void)
{
	GPIO_SetBits(GPIO_DS18B20, DS18B20_Pin);
}

/* �ܽ����� */
void DS18B20_DQ_Low(void)
{
	GPIO_ResetBits(GPIO_DS18B20, DS18B20_Pin);
}

/* ����Ϊ��������ģʽ */
void DS18B20_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin  = DS18B20_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  

	GPIO_Init(GPIO_DS18B20, &GPIO_InitStruct);
}

/* ����Ϊ�������ģʽ */
void DS18B20_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin   = DS18B20_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;  

	GPIO_Init(GPIO_DS18B20, &GPIO_InitStruct);
}

/* ��ȡ�ֽ� */
u8 DS18B20_Read_Byte(void)
{
	u8 i = 0, TempData = 0;

	for ( i = 0; i < 8; i++ )
	{
		TempData >>= 1;	

		DS18B20_IO_OUT();  /* ����Ϊ���ģʽ */
		DS18B20_DQ_Low();  /* �ܽ����� */
		delay_us(4);      
		DS18B20_DQ_High(); /* �ܽ�����, �ͷ����� */
		delay_us(10); 
		DS18B20_IO_IN();   /* ����Ϊ����ģʽ */

		/* ��ȡ�����Ǹߵ�ƽ */
		if ( GPIO_ReadInputDataBit(GPIO_DS18B20, DS18B20_Pin) == 1 )
		{
			TempData |= 0x80;	
		}

		delay_us(45);
	}

	return TempData;
}

/* д�ֽں��� */
void DS18B20_Write_Byte(u8 data)
{
	u8 i = 0;

	/* ����Ϊ���ģʽ */
	DS18B20_IO_OUT();  
	
	for ( i = 0; i < 8; i++ )
	{
		DS18B20_DQ_Low();   /* �ܽ����� */
		delay_us(15);  

		/* дһ�� bit */
		GPIO_WriteBit(GPIO_DS18B20, DS18B20_Pin, data & 0x01);
		
		delay_us(60);
		
		DS18B20_DQ_High();
		
		data >>= 1;
	}	
}

/* оƬ��λ */
u8 DS18B20_Reset(void)
{
	u8 count = 0;
	
	DS18B20_IO_OUT();  /* �ܽ�����Ϊ���ģʽ */
	DS18B20_DQ_Low();  /* ���͹ܽ� */
	delay_us(480);     /* ��ʱ 480us */
	DS18B20_DQ_High(); /* �ܽ����� */
	delay_us(80);      /* ��ʱ80us */

	DS18B20_IO_IN();

	/* �ȴ��豸��Ӧ */
	while ( GPIO_ReadInputDataBit(GPIO_DS18B20, DS18B20_Pin))
	{
		count++;

		delay_us(1);

		/* ��Ӧ��ʱ */
		if ( count > 60 )
		{
			return 1;
		}
	}

	delay_us(400); 

	return 0;
}

u8 DS18B20_GetID(u8 *ID)
{
	u8 i = 0, ret = 0;
	
	ret = DS18B20_Reset();     /* ��λ */

	/* û���豸 */
	if ( ret == 1 )
	{
		return 1;
	}
	
	DS18B20_Write_Byte(0x33);  /* ��ȡID */
	
	delay_ms(800);
		
	for ( i = 0; i < 8; i++ )
	{
		ID[i] = DS18B20_Read_Byte();
	}

	return 0;
}

/* �����¶� */
u8 DS18B20_GetTemp(u8 *ID, double *temp)
{	
	u8  TL = 0, TH = 0;
	u8  ret = 0, i;
	u16 Temp  = 0;
	
	ret = DS18B20_Reset(); 

	if ( ret == 1 )
	{
		return 1;
	}
	
	DS18B20_Write_Byte(0x55);  /* ����ָ��ID�ŵĴ����� */
	
	for ( i = 0; i < 8; i++ )
	{
		DS18B20_Write_Byte(ID[i]);
	}
	
	DS18B20_Write_Byte(0x44);  /* �¶�ת������ */

	delay_ms(800);

	ret = DS18B20_Reset(); 

	if ( ret == 1 )
	{
		return 1;
	}
	
	DS18B20_Write_Byte(0x55);  /* ����ָ��ID�ŵĴ����� */
	
	for ( i = 0; i < 8; i++ )
	{
		DS18B20_Write_Byte(ID[i]);
	}
	
	DS18B20_Write_Byte(0xBE);  /* ���¶����� */

	TL = DS18B20_Read_Byte();  /* ��ȡ���ֽ� */
	TH = DS18B20_Read_Byte();  /* ��ȡ���ֽ� */

	Temp = (TH << 8) + TL;

	/* ǰ5λΪ1,��ʾ�¶�Ϊ�� */
	if ( (Temp & 0xF800) == 0xF800 ) 
	{
		Temp  = ~Temp + 1;         /* ��������ȡ���� 1 */
		*temp = Temp * (-0.0625);
	}
	else
	{
		*temp = Temp * 0.0625;
	}

	return 0;
}

