#ifndef __SPI_FLASH_H
#define	__SPI_FLASH_H


#define	METER_DAY_ADDR		(0x00)
#define	METER_MONTH_ADDR	(SAVE_DAY * sizeof(T_MeterData))

#define	SPI_CS_PIN			GPIO_Pin_4
#define	SPI_CS_GPIO			GPIOG
#define SPI_CS_ON()			GPIO_ResetBits(SPI_CS_GPIO, SPI_CS_PIN)
#define SPI_CS_OFF()		GPIO_SetBits(SPI_CS_GPIO, SPI_CS_PIN)


/* MB85RS256A�Ĵ������� */
#define MB85RS256A_WREN_INST	0x06	/* ����дʹ�������� */
#define MB85RS256A_WRDI_INST	0x04	/* д��ֹ */
#define MB85RS256A_RDSR_INST	0x05	/* ��״̬�Ĵ��� */
#define MB85RS256A_WRSR_INST	0x01	/* д״̬�Ĵ��� */
#define MB85RS256A_READ_INST	0x03	/* ���洢������ */
#define MB85RS256A_WRITE_INST	0x02	/* д�洢������ */
#define MB85RS256A_STATUS_REG	0x00
#define MB85RS256A_INIT_STATE	0x09
#define MB85RS256A_RDID_INST	0x9F	/* ������ID */
#define MB85RS256A_SLEEP_INST	0xB9	/* ˯��ģʽ */


void SPI_FLASH_Init(void);
void SPI_FLASH_Reset(void);
void SPI_FLASH_WriteByte(u8 data, u32 WriteAddr);
void SPI_FLASH_ReadByte(u8 *data, uint32_t ReadAddr);
void SPI_FLASH_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead);

#endif
