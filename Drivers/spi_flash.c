#include "config.h"
#include "spi_flash.h"


static void SPI_FLASH_WaitBusy(void);

/* for test */
void SPI_FLASH_Test(void)
{
	uint8_t TempData[6] = {0};
	
	memset(TempData, 0xAA, 6);
	SPI_FLASH_WriteBuffer(TempData, 0x00, 6);
	memset(TempData, 0x00, 6);
	SPI_FLASH_ReadBuffer(TempData, 0x00, 6);
}

/* SPI FLASH��ʼ�� */
void SPI_FLASH_Init(void)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, ENABLE);
	
	/* SPI_CLOCK:PG5, SPI_MOSI: PG6, SPI_MISO: PG7 */
	GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);
	
	/* ����ģʽ������Ϊ���� */
	GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
	
	/* ��ʼ��SPI */
	SPI_Init(SPI2, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,\
			SPI_CPOL_High, SPI_CPHA_2Edge, \
			SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
    
	SPI_Cmd(SPI2, ENABLE);
	
	/* ����CS�ܽ� */
	GPIO_Init(SPI_CS_GPIO, SPI_CS_PIN, GPIO_Mode_Out_PP_High_Fast);
	SPI_CS_OFF();
	
//	SPI_FLASH_Test();
}

/* SPI FLASH ��дһ���ֽ� */
uint8_t SPI_ReadWriteByte(uint8_t byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET);
	/* Send byte through the SPI1 peripheral */
	SPI_SendData(SPI2, byte);
	/* Wait to receive a byte */
	while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET);
	/* Return the byte read from the SPI bus */
	return SPI_ReceiveData(SPI2);
 }

/* SPI_FLASHдʹ�� */
void SPI_FLASH_WriteEnable(void)   
{
	SPI_CS_ON();
    SPI_ReadWriteByte(MB85RS256A_WREN_INST);
	SPI_CS_OFF();
}

/* SPI_FLASHд��ֹ */
void SPI_FLASH_WriteDisable(void)   
{  
	SPI_CS_ON();
    SPI_ReadWriteByte(MB85RS256A_WRDI_INST);
	SPI_CS_OFF();
}

/* SPI_FLASHд��һ�ֽ����� */
void SPI_FLASH_WriteByte(u8 data, u32 WriteAddr)
{	
    SPI_FLASH_WriteEnable();
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(MB85RS256A_WRITE_INST);
//  SPI_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI_ReadWriteByte((u8)((WriteAddr)>>8));
	SPI_ReadWriteByte((u8)WriteAddr);
	SPI_ReadWriteByte(data);
	
	SPI_CS_OFF();
	
	SPI_FLASH_WaitBusy();
}

/* SPI_FLASH����һ�ֽ����� */
void SPI_FLASH_ReadByte(u8 *data, uint32_t ReadAddr)   
{
	SPI_CS_ON();
	
    SPI_ReadWriteByte(MB85RS256A_READ_INST);
//  SPI_ReadWriteByte((uint8_t)((ReadAddr)>>16));
    SPI_ReadWriteByte((uint8_t)((ReadAddr)>>8));
	SPI_ReadWriteByte((uint8_t)ReadAddr);
    *data = SPI_ReadWriteByte(0XFF);
	
	SPI_CS_OFF();
}

/* SPI FLASHд�뻺�������� */
void SPI_FLASH_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 	u16 i;
	
    SPI_FLASH_WriteEnable();
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(MB85RS256A_WRITE_INST);
//  SPI_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI_ReadWriteByte((u8)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
	{
		SPI_ReadWriteByte(pBuffer[i]);
	}
	
	SPI_CS_OFF();
	
	SPI_FLASH_WaitBusy();
}

/********************************************
** ��SPI FLASHָ����ַ��ȡָ�����ȵ�����
** pBuffer       - ���ݴ洢��
** ReadAddr      - ��������ʼ��ַ
** NumByteToRead - Ҫ��ȡ�����ݳ���
*********************************************/
void SPI_FLASH_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)  
{ 
 	uint16_t i;
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(MB85RS256A_READ_INST);
//  SPI_ReadWriteByte((uint8_t)((ReadAddr)>>16));
    SPI_ReadWriteByte((uint8_t)((ReadAddr)>>8));
    SPI_ReadWriteByte((uint8_t)ReadAddr);
    for (i = 0; i< NumByteToRead; i++)
	{
    	pBuffer[i] = SPI_ReadWriteByte(0XFF);
    }
	
	SPI_CS_OFF();
}

/* ��SPI FLASH״̬�Ĵ��� */
uint8_t SPI_FLASH_ReadSR(void)   
{  
	uint8_t value = 0;
	
	SPI_CS_ON();
	SPI_ReadWriteByte(MB85RS256A_RDSR_INST);
	value = SPI_ReadWriteByte(0xFF);
	SPI_CS_OFF();
	
	return value;
}

/* дSPI״̬�Ĵ��� */
void SPI_FLASH_WriteSR(uint8_t SRValue)
{   
	SPI_CS_ON();
	SPI_ReadWriteByte(MB85RS256A_WRSR_INST);    
	SPI_ReadWriteByte(SRValue); 
	SPI_CS_OFF();
}

/* �ȴ����� */
void SPI_FLASH_WaitBusy(void)
{   
	while((SPI_FLASH_ReadSR()&0x01) == 0x01);
}

/* �������ģʽ */
void SPI_FLASH_Sleep(void)   
{
  	SPI_CS_ON();
    SPI_ReadWriteByte(MB85RS256A_SLEEP_INST);
	SPI_CS_OFF();  	      
    Soft_delay_us(3);
}

void SPI_FLASH_Reset(void)
{
 	u16 i, NumByteToWrite = 0xFFFF;
	
    SPI_FLASH_WriteEnable();
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(MB85RS256A_WRITE_INST);
//  SPI_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI_ReadWriteByte((u8)(0x00));
    SPI_ReadWriteByte((u8)(0x00));
    for (i = 0; i < NumByteToWrite; i++)
	{
		SPI_ReadWriteByte(0x00);
	}
	
	SPI_CS_OFF();
	
	SPI_FLASH_WaitBusy();
}
