#ifndef __DEBUG_H
#define __DEBUG_H


#define DBG_PRINTF	printf			/* ������ӡ�� */
//#define DBG_PRINTF(...)			/* �رմ�ӡ�� */

#define DATA_PRINTF	DataPrint		/* ������ӡ�� */
//#define INFO_PRINTF(...)			/* �رմ�ӡ�� */

void DebugInit(void);
void DataPrint(uint8_t *data, uint8_t len);
void DebugPrintData(uint8_t *chr, uint8_t num);

#endif