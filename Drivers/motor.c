#include "config.h"
#include "motor.h"


bool MotorOpenOption(void)
{
	/* �������ж��Ƿ񿪷�: 
	 * �ѿ���
	 * ������״̬��־����
	 * ��ص�ѹ����
	 * ���Ŷ˿�Ϊ��
	 * ������͸֧��
	 */
	if ((MeterParm.isUse == 1)                 && \
		((MeterParm.MeterSta.Val & 0x1F) == 0) && \
		(ADCAtOnce() == TRUE)                  && \
		(GetMagnetPort() == TRUE)              && \
		((MeterParm.RemainMoney + MeterParm.OverLimit) > (MeterParm.OverMoney + MeterParm.CurUnitPrice)))
	{
		return TRUE;
	}
	
	return FALSE;
}

/* ���� */
void OpenMotor(void)
{
	MotorSta.OpenEn = 1;
	WakeupState.MotorStatus = TRIG_ACTIVE;
}

/* �ط� */
void CloseMotor(uint8_t Flag)
{
	if (Flag == 1)  /* ǿ�ƹط� */
	{
		MeterParm.MeterSta.bits.force = 1;
	}
	MotorSta.CloseEn = 1;  /* ʹ�ܹط����� */
	WakeupState.MotorStatus = TRIG_ACTIVE;
}

static void OpenMotorProcess(void)
{
	static uint32_t OpenTimer;
	static uint8_t Openstart = 1;
	
	if ((MotorSta.OpenEn == 1) && (MotorSta.Close == 0))
	{
		MotorSta.Open = 1;
		MotorSta.OpenEn = 0;
	}
	
	if (MotorSta.Open == 1)
	{
		if (MotorSta.SW == MOTOR_OPEN)
		{
			MotorSta.Open = 0;
			if (MotorSta.OpenEn==0 && MotorSta.CloseEn==0)
			{
				WakeupState.MotorStatus = TRIG_SLEEP;
			}
			return;
		}
		
		if (Openstart == 1)
		{
			Openstart = 0;
			MOTOR_OPEN_ON();
			MOTOR_CLOSE_OFF();
			OpenTimer = TimeCnt;
		}
		else if (GetTimer(OpenTimer) >= 300)
		{
			Openstart = 1;
			MOTOR_OPEN_OFF();
			MOTOR_CLOSE_OFF();
			MotorSta.Open = 0;
			MotorSta.SW = MOTOR_OPEN;
			if (MotorSta.OpenEn==0 && MotorSta.CloseEn==0)
			{
				WakeupState.MotorStatus = TRIG_SLEEP;
			}
		}
	}
}


static void CloseMotorProcess(void)
{
	static uint32_t CloseTimer;
	static uint8_t CloseStart = 1;
	
	if ((MotorSta.CloseEn == 1) && (MotorSta.Open == 0))
	{
		MotorSta.Close = 1;
		MotorSta.CloseEn = 0;
	}
	
	if (MotorSta.Close == 1)
	{
		if (MotorSta.SW == MOTOR_CLOSE)
		{
			MotorSta.Close = 0;
			if (MotorSta.OpenEn==0 && MotorSta.CloseEn==0)
			{
				WakeupState.MotorStatus = TRIG_SLEEP;
			}
			return;
		}
		
		if (CloseStart == 1)
		{
			CloseStart = 0;
			MOTOR_CLOSE_ON();
			MOTOR_OPEN_OFF();
			CloseTimer = TimeCnt;
		}
		else if (GetTimer(CloseTimer) >= 30)
		{
			CloseStart = 1;
			MOTOR_OPEN_OFF();
			MOTOR_CLOSE_OFF();
			MotorSta.Close = 0;
			MotorSta.SW = MOTOR_CLOSE;
			if (MotorSta.OpenEn==0 && MotorSta.CloseEn==0)
			{
				WakeupState.MotorStatus = TRIG_SLEEP;
			}
		}
	}
}


void MotorProcess(void)
{
	OpenMotorProcess();
	
	CloseMotorProcess();
}


void MotorInit(void)
{
	GPIO_Init(MOTOR_CHECK_GPIO, MOTOR_CHECK_PIN, GPIO_Mode_In_FL_No_IT);
    GPIO_Init(MOTOR_OPEN_GPIO, MOTOR_OPEN_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(MOTOR_CLOSE_GPIO, MOTOR_CLOSE_PIN, GPIO_Mode_Out_PP_High_Slow);
 	
	MOTOR_OPEN_OFF();
	MOTOR_CLOSE_OFF();
	
	/* ���� */
//	MotorSta.SW = MOTOR_CLOSE;
//	OpenMotor();
	
	/* �ط� */
	MotorSta.SW = MOTOR_OPEN;
	CloseMotor(0);
}
