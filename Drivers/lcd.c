#include "config.h"
#include "lcd.h"


/* Constant table for number 0 - 9 */
const uint16_t NumberMap[23] = 
{  /* 0	    1	  2	    3	  4	    5	  6	    7	  8	    9 */
	0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, \
   /* A     b     C     d     E     F     I     P     u    -     _     =      ��*/
	0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E, 0x60, 0xCE, 0x38, 0x02, 0x10, 0x12, 0x00
};

/* ��Ҫ��ʾ������ˢ��LCD��ȥ */
void FlushDataToLCD(PT_DispData ptDispData)
{
  	uint8_t i;
	
	ptDispData->valve = MotorSta.SW;
	
	if (ptDispData->month != 0)
	{
		LCD->RAM[LCD_RAMRegister_0]  |= 0x04;	// ��
	}
	
	if (ptDispData->total != 0)
	{
		LCD->RAM[LCD_RAMRegister_0]  |= 0x08;	// ��
	}
	
	if (ptDispData->consumption != 0)
	{
		LCD->RAM[LCD_RAMRegister_11]  |= 0x02;	// ����
	}
	
	if (ptDispData->remain != 0)
	{
		LCD->RAM[LCD_RAMRegister_3]  |= 0x80;	// ����
	}

	if (ptDispData->lack != 0)
	{
		LCD->RAM[LCD_RAMRegister_7]  |= 0x08;	// ����
	}
	
	if (ptDispData->changebat != 0)
	{
		LCD->RAM[LCD_RAMRegister_3]  |= 0x40;	// �뻻���
	}
	
	if (ptDispData->price != 0)
	{
		LCD->RAM[LCD_RAMRegister_7]  |= 0x02;	// ����
	}
	
	if (ptDispData->readcard != 0)
	{
		LCD->RAM[LCD_RAMRegister_10]  |= 0x20;	// ����
	}
	
	if (ptDispData->success != 0)
	{
		LCD->RAM[LCD_RAMRegister_7]  |= 0x04;	// �ɹ�
	}
	
	if (ptDispData->success != 0)
	{
		LCD->RAM[LCD_RAMRegister_10]  |= 0x40;	// ����
	}
	
	if (ptDispData->PreColon != 0)
	{
		LCD->RAM[LCD_RAMRegister_12]  |= 0x20;	// ǰ�ֺ�
	}
	
	if (ptDispData->PosColon != 0)
	{
		LCD->RAM[LCD_RAMRegister_13]  |= 0x02;	// ��ֺ�
	}
	
	if (ptDispData->PrePoint != 0)
	{
		LCD->RAM[LCD_RAMRegister_11]  |= 0x80;	// ǰ��
	}
	
	if (ptDispData->PosPoint != 0)
	{
		LCD->RAM[LCD_RAMRegister_12]  |= 0x02;	// ���
	}
	
	if (ptDispData->valve != 0)	// ����(0:�ر�  1:����)
	{
		LCD->RAM[LCD_RAMRegister_0]  |= 0x02;	// ����
	}
	else	// �ر�
	{
		LCD->RAM[LCD_RAMRegister_0]  |= 0x02;
		LCD->RAM[LCD_RAMRegister_3]  |= 0x20;
	}
	
	if (ptDispData->warning != 0)
	{
		LCD->RAM[LCD_RAMRegister_10]  |= 0x80;	// ����
	}
	
	if (ptDispData->voltage != 0)
	{
		LCD->RAM[LCD_RAMRegister_12]  |= 0x80;	// ��ѹ
	}
	
	if (ptDispData->Yuan != 0)
	{
		LCD->RAM[LCD_RAMRegister_11]  |= 0x20;	// Ԫ
	}
	
	if (ptDispData->divide != 0)
	{
		LCD->RAM[LCD_RAMRegister_11]  |= 0x08;	// '/'��
	}
	
	if (ptDispData->cube != 0)
	{
		LCD->RAM[LCD_RAMRegister_12]  |= 0x08;	// ������
	}
	
	/* ��ʾ9������ */
	for (i = 0; i < 9; i++)
	{
		DispNumber(i, ptDispData->digit[i]);
	}
}

// ��ʾ
void DispNumber(uint8_t position, uint8_t num)
{
  	switch (position)
	{
		case 0:
			LCD->RAM[LCD_RAMRegister_0]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_0]  |= (((NumberMap[num]>>7)<<4) & 0x10);  // 1A
			
			LCD->RAM[LCD_RAMRegister_0]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_0]  |= (((NumberMap[num]>>6)<<5) & 0x20);  // 1B
			
			LCD->RAM[LCD_RAMRegister_7]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_7]  |= (((NumberMap[num]>>5)<<5) & 0x20);  // 1C
			
			LCD->RAM[LCD_RAMRegister_11] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_11] |= (((NumberMap[num]>>4)<<0) & 0x01);  // 1D

			LCD->RAM[LCD_RAMRegister_7]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_7]  |= (((NumberMap[num]>>3)<<4) & 0x10);  // 1E

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>2)<<0) & 0x01);  // 1F

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>1)<<1) & 0x02);  // 1G
			break;
			
		case 1:
			LCD->RAM[LCD_RAMRegister_0]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_0]  |= (((NumberMap[num]>>7)<<6) & 0x40);  // 2A
			
			LCD->RAM[LCD_RAMRegister_0]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_0]  |= (((NumberMap[num]>>6)<<7) & 0x80);  // 2B
			
			LCD->RAM[LCD_RAMRegister_7]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_7]  |= (((NumberMap[num]>>5)<<7) & 0x80);  // 2C
			
			LCD->RAM[LCD_RAMRegister_11] &= (~0x04);
			LCD->RAM[LCD_RAMRegister_11] |= (((NumberMap[num]>>4)<<2) & 0x04);  // 2D

			LCD->RAM[LCD_RAMRegister_7]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_7]  |= (((NumberMap[num]>>3)<<6) & 0x40);  // 2E

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>2)<<2) & 0x04);  // 2F
			
			LCD->RAM[LCD_RAMRegister_4]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>1)<<3) & 0x08);  // 2G
			break;
		
		case 2:
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>7)<<0) & 0x01);  // 3A
			
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>6)<<1) & 0x02);  // 3B

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>5)<<1) & 0x02);  // 3C
			
			LCD->RAM[LCD_RAMRegister_11] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_11] |= (((NumberMap[num]>>4)<<4) & 0x10);  // 3D

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>3)<<0) & 0x01);  // 3E

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>2)<<4) & 0x10);  // 3F

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>1)<<5) & 0x20);  // 3G
			break;

		case 3:
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>7)<<2) & 0x04);  // 4A
			
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>6)<<3) & 0x08);  // 4B

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>5)<<3) & 0x08);  // 4C
			
			LCD->RAM[LCD_RAMRegister_11] &= (~0x40);
			LCD->RAM[LCD_RAMRegister_11] |= (((NumberMap[num]>>4)<<6) & 0x40);  // 4D

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>3)<<2) & 0x04);  // 4E

			LCD->RAM[LCD_RAMRegister_4]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>2)<<6) & 0x40);  // 4F
			
			LCD->RAM[LCD_RAMRegister_4]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_4]  |= (((NumberMap[num]>>1)<<7) & 0x80);  // 4G
			break;
			
		case 4:
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>7)<<4) & 0x10);  // 5A
			
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>6)<<5) & 0x20);  // 5B

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>5)<<5) & 0x20);  // 5C
			
			LCD->RAM[LCD_RAMRegister_12] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_12] |= (((NumberMap[num]>>4)<<0) & 0x01);  // 5D

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>3)<<4) & 0x10);  // 5E

			LCD->RAM[LCD_RAMRegister_5]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>2)<<0) & 0x01);  // 5F
		
			LCD->RAM[LCD_RAMRegister_5]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>1)<<1) & 0x02);  // 5G
			break;

		case 5:
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>7)<<6) & 0x40);  // 6A
			
			LCD->RAM[LCD_RAMRegister_1]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_1]  |= (((NumberMap[num]>>6)<<7) & 0x80);  // 6B

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>5)<<7) & 0x80);  // 6C
			
			LCD->RAM[LCD_RAMRegister_12] &= (~0x04);
			LCD->RAM[LCD_RAMRegister_12] |= (((NumberMap[num]>>4)<<2) & 0x04);  // 6D

			LCD->RAM[LCD_RAMRegister_8]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_8]  |= (((NumberMap[num]>>3)<<6) & 0x40);  // 6E

			LCD->RAM[LCD_RAMRegister_5]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>2)<<2) & 0x04);  // 6F
	
			LCD->RAM[LCD_RAMRegister_5]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>1)<<3) & 0x08);  // 6G
			break;
		
		case 6:
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>7)<<0) & 0x01);  // 7A
			
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>6)<<1) & 0x02);  // 7B

			LCD->RAM[LCD_RAMRegister_9]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>5)<<1) & 0x02);  // 7C
			
			LCD->RAM[LCD_RAMRegister_12] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_12] |= (((NumberMap[num]>>4)<<4) & 0x10);  // 7D

			LCD->RAM[LCD_RAMRegister_9]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>3)<<0) & 0x01);  // 7E

			LCD->RAM[LCD_RAMRegister_5]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>2)<<4) & 0x10);  // 7F
	
			LCD->RAM[LCD_RAMRegister_5]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>1)<<5) & 0x20);  // 7G
			break;

		case 7:
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>7)<<2) & 0x04);  // 8A
			
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>6)<<3) & 0x08);  // 8B

			LCD->RAM[LCD_RAMRegister_9]  &= (~0x08);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>5)<<3) & 0x08);  // 8C
		
			LCD->RAM[LCD_RAMRegister_12] &= (~0x40);
			LCD->RAM[LCD_RAMRegister_12] |= (((NumberMap[num]>>4)<<6) & 0x40);  // 8D
	
			LCD->RAM[LCD_RAMRegister_9]  &= (~0x04);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>3)<<2) & 0x04);  // 8E

			LCD->RAM[LCD_RAMRegister_5]  &= (~0x40);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>2)<<6) & 0x40);  // 8F
			
			LCD->RAM[LCD_RAMRegister_5]  &= (~0x80);
			LCD->RAM[LCD_RAMRegister_5]  |= (((NumberMap[num]>>1)<<7) & 0x80);  // 8G
			break;

		case 8:
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>7)<<4) & 0x10);  // 9A
		
			LCD->RAM[LCD_RAMRegister_2]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_2]  |= (((NumberMap[num]>>6)<<5) & 0x20);  // 9B

			LCD->RAM[LCD_RAMRegister_9]  &= (~0x20);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>5)<<5) & 0x20);  // 9C
		
			LCD->RAM[LCD_RAMRegister_13] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_13] |= (((NumberMap[num]>>4)<<0) & 0x01);  // 9D
	
			LCD->RAM[LCD_RAMRegister_9]  &= (~0x10);
			LCD->RAM[LCD_RAMRegister_9]  |= (((NumberMap[num]>>3)<<4) & 0x10);  // 9E

			LCD->RAM[LCD_RAMRegister_6]  &= (~0x01);
			LCD->RAM[LCD_RAMRegister_6]  |= (((NumberMap[num]>>2)<<0) & 0x01);  // 9F

			LCD->RAM[LCD_RAMRegister_6]  &= (~0x02);
			LCD->RAM[LCD_RAMRegister_6]  |= (((NumberMap[num]>>1)<<1) & 0x02);  // 9G
			break;
			
		default:
			break;
	}
}

/* This function Clear the whole LCD RAM. */
void LCDClear(void)
{
	uint8_t counter = 0;
	
	memset(&DispData, 0, sizeof(T_DispData));
	memset(DispData.digit, 22, 9);				/* ��������ʾ���� */
	
	for (counter = 0; counter <= LCD_RAMRegister_13; counter++)
	{
		LCD->RAM[counter] = LCD_RAM_RESET_VALUE;
	}
}

/* LCD Ҫ��RTC���ʼ�� */
void LCDInit(void)
{
	/* Enable LCD/RTC clock */
//	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	
	LCD_DeInit();
	
	/* Initialize the LCD, ����Ϊ1/4���ã�1/3ƫѹģʽ */
	LCD_Init(LCD_Prescaler_4, LCD_Divider_16, LCD_Duty_1_4, 
             LCD_Bias_1_3, LCD_VoltageSource_Internal);
	
  	/* Mask register, For declare the segements used. */
	/* seg 1~7 */
  	LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFE);
	/* seg 8~15 */
  	LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
	/* seg 16~21 */
  	LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x3F);
	
  	/* To set contrast to mean value */
  	LCD_ContrastConfig(LCD_Contrast_Level_3);
  
  	LCD_DeadTimeConfig(LCD_DeadTime_0);
  	LCD_PulseOnDurationConfig(LCD_PulseOnDuration_3);
  
  	/* Enable LCD peripheral */
	LCD_Cmd(ENABLE);
	
//	LCDTest();
}

#if 0
void LCDTest(void)
{
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 9; j++)
		{
			DispNumber(j, i);
		}
		
		Soft_delay_ms(800);
	}

	
	LCDClear();
	
	memset(&DispData, 8, sizeof(T_DispData));
	FlushDataToLCD(&DispData);
	Soft_delay_ms(2000);
}
#endif
