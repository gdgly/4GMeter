#include "config.h"
#include "counter.h"
#include "beep.h"
#include "key.h"


static uint8_t FlowNum = 0;		/* �������ϴ��� */
static uint8_t CountNum = 0;	/* �������ϴ��� */
static uint8_t MagnetNum = 0;	/* ���Ź��ϴ��� */

static uint8_t FlowFlag = 0;	/* ��������־ */
static uint8_t ExSampCount;     /* ��������� */
static uint32_t InSampTimer;	/* �ڲ����˲���ʱ */
static uint32_t ExSampTimer;	/* ������˲���ʱ */
static uint32_t MagnetTimer;	/* �����˲���ʱ */

/* ����������� */
static void ExSampProcess(void)
{
	uint32_t UsedGas;
	static uint8_t RemainFlag = 1;
	
	if (CountState.ExSampStatus==TRIG_ACTIVE && GetTimer(ExSampTimer)>=1)
	{
		if (!GPIO_ReadInputDataBit(SAMP_GPIO, EX_SAMP_PIN))
		{
			FlowFlag = 1;
			ExSampCount++;
			MeterParm.TotalGas++;
			UsedGas = MeterParm.TotalGas - MeterParm.CycleBaseGas;
			
			if (UsedGas <= ((uint32_t)MeterParm.CurPrice.GasDivide[0]*100))
			{
				MeterParm.CurUnitPrice = MeterParm.CurPrice.PriceDevide[0];
			}
			else if (UsedGas <= ((uint32_t)MeterParm.CurPrice.GasDivide[1]*100))
			{
				MeterParm.CurUnitPrice = MeterParm.CurPrice.PriceDevide[1];
			}
			else
			{
				MeterParm.CurUnitPrice = MeterParm.CurPrice.PriceDevide[2];
			}
			
			MeterParm.UsedMoney += MeterParm.CurUnitPrice;
			
			if (MeterParm.RemainMoney >= MeterParm.CurUnitPrice)
			{
				MeterParm.RemainMoney -= MeterParm.CurUnitPrice;
				
				if (MeterParm.RemainMoney < MeterParm.MarginWarn && RemainFlag==1)
				{
					AddNetTask(C3062, 10);  /* ������������һ�� */
					RemainFlag = 0;
				}
				else if (MeterParm.RemainMoney >= MeterParm.MarginWarn && RemainFlag==0)
				{
					RemainFlag = 1;
				}
				
				if ((MeterParm.RemainMoney + MeterParm.OverLimit) < MeterParm.CurUnitPrice)
				{
					CloseMotor(0);  /* ����ȫ���꣬�ط� */
					DBG_PRINTF("No remain money! \n");
				}
			}
			else
			{
				if (MeterParm.RemainMoney == 0)
				{
					MeterParm.OverMoney += MeterParm.CurUnitPrice;
				}
				else
				{
					MeterParm.OverMoney = MeterParm.CurUnitPrice - MeterParm.RemainMoney;
					MeterParm.RemainMoney = 0;
				}
				
				if (MeterParm.OverMoney >= MeterParm.OverLimit)  /* ����ȫ͸֧�����������ҹط� */
				{
					CloseMotor(0);
					DBG_PRINTF("No overdraft money! \n");
				}
			}
			
			EEPROMWriteBuffer((uint8_t *)&MeterParm, METER_PAM_ADDR, (uint8_t)sizeof(T_MeterParm));
			DBG_PRINTF("The total gas is %ld. \n", MeterParm.TotalGas);
		}
		
		CountState.ExSampStatus = TRIG_SLEEP;
	}
}

/* �ڲ��������� */
static void InSampProcess(void)
{
	uint32_t ofet;
	static uint8_t InSampCount;  /* �ڲ������� */
	
	if (CountState.InSampStatus==TRIG_ACTIVE && GetTimer(InSampTimer)>=1)
	{
		if (!GPIO_ReadInputDataBit(SAMP_GPIO, IN_SAMP_PIN))
		{
			InSampCount++;
			if (InSampCount > IN_SAMP_SUM)
			{
				if (ExSampCount == 0)
				{
					if (MeterParm.MeterSta.bits.samp == 0)  /* �������� */
					{
						CountNum++;
						if (CountNum == MeterParm.CountMax)
						{
							CloseMotor(1);  		/* ǿ�ƹط� */
							AddNetTask(C3062, 11);	/* ���������ϱ� */
							MeterParm.MeterSta.bits.samp = 1;
							ofet = offset(T_MeterParm, MeterSta);
							EEPROMWriteBuffer((uint8_t *)&MeterParm.MeterSta, METER_PAM_ADDR+ofet, 1);
						}
						else
						{
							CloseMotor(0);			/* �����ط� */
							AddNetTask(C3062, 11);	/* ���������ϱ� */
							DBG_PRINTF("In sample error! \n");
						}
					}
				}
				
				InSampCount = 0;
				ExSampCount = 0;
			}
		}
		
		CountState.InSampStatus = TRIG_SLEEP;
	}
}

/* ���Ŵ����� */
static void MagnetProcess(void)
{
	uint32_t ofet;
	
	if (CountState.MagnetStatus==TRIG_ACTIVE && GetTimer(MagnetTimer)>=25)
	{
		if (!GPIO_ReadInputDataBit(SAMP_GPIO, MAGNET_PIN))
		{
			if (MeterParm.MeterSta.bits.magnet == 0)  /* ���Ŵ��� */
			{
				MagnetNum++;
				if (MagnetNum > MeterParm.MagnetMax)
				{
					MagnetNum = 0;
					CloseMotor(1);  		/* ǿ�ƹط� */
					AddNetTask(C3062, 5);	/* �����ϱ� */
					DBG_PRINTF("Magnet error! \n");
					MeterParm.MeterSta.bits.magnet = 1;
					ofet = offset(T_MeterParm, MeterSta);
					EEPROMWriteBuffer((uint8_t *)&MeterParm.MeterSta, METER_PAM_ADDR+ofet, 1);
				}
				else
				{
					CloseMotor(0);      	/* �����ط� */
					AddNetTask(C3062, 5);	/* �����ϱ� */
					DBG_PRINTF("Magnet error! \n");
				}
			}
		}
		
		CountState.MagnetStatus = TRIG_SLEEP;
	}
}

/* ���������� */
void FlowProcess(void)
{
	uint32_t ofet;
	static uint8_t FlowErrNum = 0;
	static uint16_t FlowTimeCnt = 0;
	
	FlowTimeCnt++;
	if (FlowFlag == 1)
	{
		// DBG_PRINTF("The flow time count is %d. \n", FlowTimeCnt);
		FlowFlag = 0;
		if (36/FlowTimeCnt > MeterParm.FlowLimit)  /* ���� */
		{
			FlowErrNum++;
			if (FlowErrNum >= 10)
			{
				FlowErrNum = 0;
				FlowNum++;
				if (FlowNum > MeterParm.FlowMax)
				{
					FlowNum = 0;
					CloseMotor(1);  /* ǿ�ƹط� */
					AddNetTask(C3062, 10);
					MeterParm.MeterSta.bits.flow = 1;
					ofet = offset(T_MeterParm, MeterSta);
					EEPROMWriteBuffer((uint8_t *)&MeterParm.MeterSta, METER_PAM_ADDR+ofet, 1);
				}
				else
				{
					CloseMotor(0);  /* �����ط� */
					AddNetTask(C3062, 10);
					DBG_PRINTF("Over flow error! \n");
				}
			}
		}
		else
		{
			FlowErrNum = 0;
		}
		
		FlowTimeCnt = 0;
	}
}

/* ��ȡ���Ŷ˿ڵ�ƽ״̬ */
bool GetMagnetPort(void)
{
	if (!GPIO_ReadInputDataBit(SAMP_GPIO, MAGNET_PIN))
	{
		return FALSE;
	}
	
	return TRUE;
}


void SetFaultNumber(uint8_t data)
{
	MagnetNum = data & 0x03;
	CountNum  = (data>>2) & 0x03;
	FlowNum   = (data>>4) & 0x0F;
}


uint8_t GetFaultNumber(void)
{
	return ((MagnetNum & 0x03) | ((CountNum & 0x03)<<2) | ((FlowNum & 0x0F)<<4));
}

/* ������Ԫ�������� */
void CounterProcess(void)
{
	ExSampProcess();
	
	InSampProcess();
	
	MagnetProcess();
}

/* �������ʼ�� */
static void ExSampInit(void)
{
	GPIO_Init(SAMP_GPIO, EX_SAMP_PIN, GPIO_Mode_In_FL_IT);
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
	EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);
	ITC_SetSoftwarePriority(EXTI2_IRQn, ITC_PriorityLevel_3);
}

/* �ڲ�����ʼ�� */
static void InSampInit(void)
{
	GPIO_Init(SAMP_GPIO, IN_SAMP_PIN, GPIO_Mode_In_FL_IT);
	EXTI_ClearITPendingBit(EXTI_IT_Pin7);
	EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
	ITC_SetSoftwarePriority(EXTI7_IRQn, ITC_PriorityLevel_1);
}

/* ���ų�ʼ�� */
static void MagnetInit(void)
{
	GPIO_Init(SAMP_GPIO, MAGNET_PIN, GPIO_Mode_In_FL_IT);
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
	EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
	ITC_SetSoftwarePriority(EXTI3_IRQn, ITC_PriorityLevel_1);
}

/* ������Ԫ��ʼ�� */
void CounterInit(void)
{
	ExSampInit();

	InSampInit();
	
	MagnetInit();
}

/* ������ж� */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
	ExSampTimer = TimeCnt;
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
	WakeupState.CountStatus = TRIG_ACTIVE;
	CountState.ExSampStatus = TRIG_ACTIVE;
}

/* �ڲ����ж� */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
	InSampTimer = TimeCnt;
	EXTI_ClearITPendingBit(EXTI_IT_Pin7);
	WakeupState.CountStatus = TRIG_ACTIVE;
	CountState.InSampStatus = TRIG_ACTIVE;
}

/* �����ж� */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
	MagnetTimer = TimeCnt;
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
	WakeupState.CountStatus = TRIG_ACTIVE;
	CountState.MagnetStatus = TRIG_ACTIVE;
}
