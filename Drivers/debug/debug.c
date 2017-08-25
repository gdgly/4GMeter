#include "config.h"
#include "debug.h"


void DataPrint(uint8_t *data, uint8_t len)
{
  	uint8_t i;
	
	for (i = 0; i< len; i++)
	{
		printf("%02X ", *(data+i));
	}
	printf("\n");
}

/* ��ӡ���ڳ�ʼ�� */
void DebugInit(void)
{
    USART_DeInit(USART1);	/* ���Ĵ�����ֵ��λ */ 
    
	GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);        // ���ڽ���
	GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);   // ���ڷ���

	GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);       // ʹ���������
	GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);       // ʹ���������
    
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);   // ���ô���ʱ��

	USART_Init(USART1, (u32)115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, \
			  (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx)); 

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);

//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

	USART_Cmd(USART1, ENABLE);
}

#if 0
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        TempBuff[TempPoint++] = USART_ReceiveData8(USART1);
		if (TempPoint == 50)
		{
			TempPoint = 0;
		}

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

	if (USART_GetFlagStatus(USART1, USART_FLAG_OR) == SET)
	{
		USART_ClearFlag(USART1, USART_FLAG_OR);  // ���OR
//		USART_ReceiveData8(USART1); 			 // ��DR
	}
}
#endif
