#include "config.h"
#include "spi_flash.h"


uint8_t TempData[6] = {0};

void SPI_FLASH_WaitBusy(void);
void SPI_FLASH_EraseSector(u32 Dst_Addr);

/* for test */
void SPI_FLASH_Test(void)
{
	memset(TempData, 0xAA, 6);
//	SPI_FLASH_Write(TempData, 0x00, 6);
//	memset(TempData, 0x00, 6);
//	SPI_FLASH_Read(TempData, 0x00, 6);
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
	
	SPI_FLASH_Test();
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

/* ��SPI FLASH״̬�Ĵ��� */
uint8_t SPI_FLASH_ReadSR(void)
{  
	uint8_t value = 0;
	
	SPI_CS_ON();
	SPI_ReadWriteByte(W25X_ReadStatusReg);
	value = SPI_ReadWriteByte(0Xff);
	SPI_CS_OFF();
	
	return value;
}

/* дSPI״̬�Ĵ��� */
void SPI_FLASH_WriteSR(uint8_t SRValue)
{   
	SPI_CS_ON();
	SPI_ReadWriteByte(W25X_WriteStatusReg);    
	SPI_ReadWriteByte(SRValue); 
	SPI_CS_OFF();
}

/* SPI_FLASHDдʹ�� */
void SPI_FLASH_WriteEnable(void)   
{
	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_WriteEnable);
	SPI_CS_OFF();
}

/* SPI_FLASHDд��ֹ */
void SPI_FLASH_WriteDisable(void)   
{  
	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_WriteDisable);
	SPI_CS_OFF();
} 

/* ��оƬID */
uint16_t SPI_FLASH_ReadID(void)
{
	uint16_t DeviceID = 0;
	
    SPI_CS_ON();
	
    /* Send read id instruction */  
    SPI_ReadWriteByte(0x90);
    SPI_ReadWriteByte(0X00);
    SPI_ReadWriteByte(0X00);
    SPI_ReadWriteByte(0X00);
	
    /* Read a byte from the FLASH */
    DeviceID = (SPI_ReadWriteByte(0xFF)<<8);  
    DeviceID |= SPI_ReadWriteByte(0xFF);
	
    SPI_CS_OFF();
    
	return DeviceID; 
}

// ��SPI FLASHָ����ַ��ȡָ�����ȵ�����
// pBuffer - ���ݴ洢��
// ReadAddr - ��������ʼ��ַ
// NumByteToRead - Ҫ��ȡ�����ݳ���
void SPI_FLASH_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{ 
 	uint16_t i;
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(W25X_ReadData);
    SPI_ReadWriteByte((uint8_t)((ReadAddr)>>16));
    SPI_ReadWriteByte((uint8_t)((ReadAddr)>>8));
    SPI_ReadWriteByte((uint8_t)ReadAddr);
    for(i = 0; i< NumByteToRead; i++)
	{
    	pBuffer[i] = SPI_ReadWriteByte(0XFF);
    }
	
	SPI_CS_OFF();
}

/* SPI FLASH��һҳ��0~65535����д������256���ֽڵ����� */
void SPI_FLASH_WritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 	u16 i;
	
    SPI_FLASH_WriteEnable();
	
	SPI_CS_ON();
	
    SPI_ReadWriteByte(W25X_PageProgram);
    SPI_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI_ReadWriteByte((u8)WriteAddr);
	
    for(i = 0; i < NumByteToWrite; i++)
	{
		SPI_ReadWriteByte(pBuffer[i]);
	}
	
	SPI_CS_OFF();
	
	SPI_FLASH_WaitBusy();
}

/* ��Ч��дSPI�����뱣֤��д�ĵ�ַ����Ϊ0xFF�� */
void SPI_Flash_WriteNoCheck(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)   
{
	u16 pageremain;
	
	pageremain = 256 - WriteAddr%256;
	
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite;
	}
	
	while(1)
	{	   
		SPI_FLASH_WritePage(pBuffer, WriteAddr, pageremain);
		
		if (NumByteToWrite == pageremain)
		{
			break;
		}
	 	else  /* NumByteToWrite>pageremain */
		{
			pBuffer   += pageremain;
			WriteAddr += pageremain;
			NumByteToWrite -= pageremain;
			
			if (NumByteToWrite > 256)
			{
				pageremain = 256;
			}
			else
			{
				pageremain = NumByteToWrite;
			}
		}
	}  
}

/* SPI FLASH ��ָ���ط�д��ָ�����ȵ����� */ 		   
u8 SPI_FLASH_BUF[4096];
void SPI_FLASH_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 i;
	u32 secpos;
	u16 secoff;
	u16 secremain;
 	
	secpos = WriteAddr/4096;
	secoff = WriteAddr%4096;
	secremain = 4096 - secoff;
	
	if (NumByteToWrite <= secremain)
	{
		secremain = NumByteToWrite;
	}
	
	while(1)
	{	
		SPI_FLASH_Read(SPI_FLASH_BUF, secpos*4096, 4096);
		for (i = 0; i < secremain; i++)
		{
			if (SPI_FLASH_BUF[secoff+i] != 0xFF)
			{
				break;
			}
		}
		
		if (i < secremain)
		{
			SPI_FLASH_EraseSector(secpos);
			
			for (i = 0; i < secremain; i++)
			{
				SPI_FLASH_BUF[i+secoff] = pBuffer[i];
			}
			
			SPI_Flash_WriteNoCheck(SPI_FLASH_BUF, secpos*4096, 4096); 
		}
		else
		{
			SPI_Flash_WriteNoCheck(pBuffer, WriteAddr, secremain);
		}
		
		if(NumByteToWrite == secremain)
		{
			break;
		}
		else
		{
			secpos++;
			secoff = 0;
		   	pBuffer += secremain;
			WriteAddr += secremain;
		   	NumByteToWrite -= secremain;
			if (NumByteToWrite > 4096)
			{
				secremain = 4096;
			}
			else
			{
				secremain = NumByteToWrite;
			}
		}
	}	 
}

/* ��������оƬ */
void SPI_Flash_Erase_Chip(void)
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitBusy();
  	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_ChipErase);
	SPI_CS_OFF();      
	SPI_FLASH_WaitBusy();
}

/* ����һ������ */
void SPI_FLASH_EraseSector(u32 Dst_Addr)
{
	Dst_Addr *= 4096;
	
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitBusy();
  	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_SectorErase);
    SPI_ReadWriteByte((u8)((Dst_Addr)>>16));
    SPI_ReadWriteByte((u8)((Dst_Addr)>>8)); 
    SPI_ReadWriteByte((u8)Dst_Addr);
	SPI_CS_OFF();	      
    SPI_FLASH_WaitBusy();
}

/* �ȴ����� */
void SPI_FLASH_WaitBusy(void)
{   
	while((SPI_FLASH_ReadSR()&0x01) == 0x01);
}

/* �������ģʽ */
void SPI_FLASH_PowerDown(void)   
{ 
  	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_PowerDown);
	SPI_CS_OFF();  	      
    Soft_delay_us(3);
}

/* ���� */
void SPI_FLASH_WAKEUP(void)
{  
  	SPI_CS_ON();
    SPI_ReadWriteByte(W25X_ReleasePowerDown);
	SPI_CS_OFF();	      
    Soft_delay_us(3);
}
