
#include "pubdata.h"

/*
 * SysTick ����Ƕ���ж�����NVIC���Ʋ���һ����4���Ĵ���
 * STK_CSR  : 0xE000E010  -- ���ƼĴ���
 * STK_LOAD : 0xE000E014  -- ���ؼĴ���
 * STK_VAL  : 0xE000E018  -- ��ǰֵ�Ĵ���
 * STK_CALR : 0xE000E01C  -- У׼ֵ�Ĵ���
 */


int fputc(int ch, FILE *f)
{
	/* �������� */
	USART_SendData(USART2, (u8)ch);

	/* �ȴ�������� */
	while ( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );

	return ch;
}


