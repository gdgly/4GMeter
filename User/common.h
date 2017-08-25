#ifndef __COMMON_H
#define __COMMON_H


#define	SAVE_DAY			(60)
#define	SAVE_MONTH			(120)
#define	NUM_OF_PACKET		(6)
#define	NET_TASK_SIZE		(100)
#define	METER_PAM_ADDR		(0x00)
#define	METER_DATA_ADDR		sizeof(T_MeterParm)

#define offset(type, member) (uint32_t)(&(((type *)0)->member))

#define ConvertEndian16(X)	((((uint16_t)(X) & 0xff00) >> 8) | (((uint16_t)(X) & 0x00ff) << 8))
#define ConvertEndian32(X)	((((uint32_t)(X) & 0xff000000) >> 24) | \
							(((uint32_t)(X) & 0x00ff0000) >> 8) | \
							(((uint32_t)(X) & 0x0000ff00) << 8) | \
							(((uint32_t)(X) & 0x000000ff) << 24))

/* �������� */
typedef enum
{
	CYCLE_YEAR,
	CYCLE_HALF_YEAR,
	CYCLE_QUARTER,
	CYCLE_MONTH,
}tStepCycle;

/* ���ݼ۸�ṹ�� */
typedef struct _Price
{
    uint8_t PriceType;   	  // ���
    uint8_t PriceVer;		  // �汾��
	tStepCycle StepCycle;	  // ��������, 0:�� 1������ 2������ 3:��
    uint8_t ActMonth;         // ��Ч��
	uint8_t ActPeriod;		  // ��Ч��
    uint16_t GasDivide[2];    // ��������[��]
    uint16_t PriceDevide[3];  // �۸����[��]
}T_Price, *PT_Price;

/* ��վIP���� */
typedef struct _Socket
{
	uint8_t IP[15];
	uint8_t Port[6];
} T_Socket, *PT_Socket;

/* �����վ��Žṹ�� */
typedef struct SlaveAddr
{
	uint8_t MID[2];
	uint8_t ConAddr[5];
} T_SlaveAddr, *PT_SlaveAddr;

/* �� �� �� */
typedef struct _Date
{
	uint8_t yearH;
	uint8_t yearL;
	uint8_t month;
	uint8_t day;
} T_Date, *PT_Date;

/* ʱ �� �� */
typedef struct _Time
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} T_Time, *PT_Time;

/* ������Ѳ��� */
typedef struct _Wakeup
{
	T_Time   Time;
	uint16_t Duration;
	uint16_t Offset;
} T_Wakeup, *PT_Wakeup;

/* ������� */
typedef enum
{
	PRE_PAY,
	POST_PAY,
}tMeterType;

/* ���ʹ�ò��� */
typedef struct _MeterParm
{
	uint8_t isUse;				/* ������־ */
	T_Socket Socket;			/* ��վIP���� */
	tMeterType MeterType;		/* ������� */
	METER_STA MeterSta;			/* ���״̬ */
	//BYTE_VAL MeterFlag;		/* ��߱�־ */
	uint8_t RechargeNum;		/* ��ֵ��� */
	
	T_SlaveAddr SlaveAddr;		/* ��վ��ַ */
	uint16_t GPRSHeartTime;		/* �������� */
	
	uint32_t TotalGas;			/* �ۼ�������[0.01��]*/
	uint32_t TotalMoney;		/* �ۼƳ�ֵ���[0.01��] */
	uint16_t UploadCycle;		/* �ϱ����� */
	T_Wakeup Wakeup;			/* ������Ѳ��� */
	
	T_Price CurPrice;			/* ��ǰ�۸���� */
	T_Price NewPrice;			/* Ԥ���۸���� */
	
	uint32_t CornerLimit;		/* ���ڻ�ֵ */
	uint8_t FlowLimit;			/* �����޶�ֵ[1����] */
	uint8_t LittleUseGas;		/* ��ʱ��С�������� */
	uint8_t LongtimeProtect;	/* ��ʱ�䲻�ñ���ֵ */
	uint8_t ContinuousUseTime;	/* ��������ʱ�� */
	
	uint8_t FlowMax;			/* ���������������� */
	uint8_t CountMax;			/* ���������������� */
	uint8_t MagnetMax;			/* ���ű����������� */
	
	uint32_t OverLimit;			/* ͸֧���[0.01��] */
	uint32_t MarginWarn;       	/* ����������ʾ[0.01��] */
	uint32_t CycleBaseGas;		/* ��ǰ���ڻ���������[0.01��] */
	uint32_t DayBaseGas;		/* ����������������[0.01��] */
	uint32_t MonthBaseGas;		/* ����������������[0.01��] */
	uint32_t UsedMoney;			/* �ۼ�ʹ�ý��[0.01��] */
	uint32_t RemainMoney;		/* ʣ����[0.01��] */
	uint32_t OverMoney;			/* ��͸֧���[0.01��] */
	uint32_t CurUnitPrice;		/* ��ǰ���� */
	uint8_t DayPoint;			/* ����������ָ�� */
	uint8_t MonthPoint;			/* ����������ָ�� */
	T_Date MeterOutDate;		/* ��ߵ������� */
}T_MeterParm, *PT_MeterParm;

/* ������ʾ״̬ */
typedef enum
{
	KEY_DISP_INIT,
	KEY_DISP_RUN,
	KEY_DISP_WAIT,
}tKeyDispStates;

/* ���߹���״̬ */
typedef enum
{
	TRIG_SLEEP,
	TRIG_IDLE,
	TRIG_INIT,
	TRIG_ACTIVE,
	TRIG_WAIT,
}tTrigStatus;

/* ���߽ṹ�� */
typedef struct _WakeupState
{
	bool LowPowerStatus;
	tTrigStatus KeyStatus;
	tTrigStatus RTCStatus;
	tTrigStatus GPRSStatus;
	tTrigStatus CountStatus;
	tTrigStatus BatStatus;
	tTrigStatus MotorStatus;
} T_WakeupState, *PT_WakeupState;

/* �������߽ṹ�� */
typedef struct _CountState
{
	tTrigStatus InSampStatus;
	tTrigStatus ExSampStatus;
	tTrigStatus MagnetStatus;
} T_CountState, *PT_CountState;

/* ϵͳʱ�� */
typedef struct _SystemTime
{
	uint8_t yearH;
	uint8_t yearL;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} T_SystemTime, *PT_SystemTime;

/* ���ָ���ṹ�� */
typedef struct _MeterData
{
	T_SystemTime Time;
	uint16_t GasPot;  	/* ���ָ��С�� */
	uint32_t GasInt;  	/* ���ָ������ */
} T_MeterData, *PT_MeterData;

/* ��ʾ�ṹ�� */
typedef struct _DispData
{
	uint8_t month;		  // ��
	uint8_t total;		  // ��
	uint8_t consumption;  // ����
	uint8_t remain;		  // ����
	uint8_t lack;		  // ����
	uint8_t changebat;	  // �뻻���
	uint8_t price;		  // ����
	uint8_t readcard;	  // ����
	uint8_t success;	  // �ɹ�
	uint8_t error;		  // ����
	uint8_t PreColon;	  // ǰ�ֺ�
	uint8_t PosColon;	  // ��ֺ�
	uint8_t PrePoint;	  // ǰ��
	uint8_t PosPoint;	  // ���
	uint8_t valve;		  // ����
	uint8_t warning;	  // ����
	uint8_t voltage;	  // ��ѹ
	uint8_t Yuan;		  // Ԫ
	uint8_t divide;		  // '/'��
	uint8_t cube;		  // ������
	uint8_t digit[9];	  // 9������
} T_DispData, *PT_DispData;

/* ����״̬ */
typedef enum
{
	MOTOR_CLOSE,
	MOTOR_OPEN,
}tMoterStatus;

/* ���Ų��� */
typedef struct _MotorSta
{
	tMoterStatus SW;	/* ��ǰ����״̬: 0 - �أ�1 - �� */
	uint8_t Open;		/* ִ�п�������: 0 - ��Ч��1 - ���� */
	uint8_t OpenEn;
	uint8_t Close;		/* ִ�йط�����: 0 - ��Ч��1 - �ط� */
	uint8_t CloseEn;
} T_MotorSta, *PT_MotorSta;

/* ���屨��ID�ṹ�� */
typedef struct _PacketID
{
	uint8_t yearL;
	uint8_t month;
	uint8_t day;
	uint32_t Order;
} T_PacketID, *PT_PacketID;

/* ���籨��ͷ���ݽṹ�� */
typedef struct NetHead
{
	uint8_t Preamble;        	/* ��ʼ��0x68 */
	uint16_t PacketLen;			/* the whole packet length */
	uint16_t FuncCode;     		/* function code */
	uint8_t TranDirect;      	/* transmission direction, 0: master send out; 1: slave send out */ 
	uint8_t ResponseFlag;      	/* response flag, 0: request; 1: answer */
	T_SlaveAddr SlaveAddr;		/* ��վ��� */
	T_PacketID PacketID;		/* ����ID */
	uint16_t DataLen;			/* �����򳤶� */
}T_NetHead, *PT_NetHead;

/* �����������ṹ�� */
typedef struct _HeartBeat
{
	T_NetHead Head;
	uint8_t MeterAddr[7];
	uint16_t Crc;
	uint8_t End;
}T_HeartBeat, *PT_HeartBeat;

/* ����������� */
typedef enum
{
	PACKET_IDLE,
	PACKET_DAY,
	PACKET_MONTH,
}tPacketType;

/* �������״̬ */
typedef enum
{
	PACKET_STATE_IDLE,
	PACKET_STATE_INIT,
	PACKET_STATE_RUN,
}tPacketState;

/* �������ṹ�� */
typedef struct _MultiPacket
{
	T_NetHead NetHead;
	uint8_t End;
	uint8_t Node;
	uint8_t TotalNode;
	uint16_t FrameSeq;
	tPacketType PacketType;
	tPacketState PacketState;
} T_MultiPacket, *PT_MultiPacket;


extern float Vapp;
extern uint8_t LongPress;
extern uint8_t GPRSFlag;
extern bool LowPowerStatus;
extern T_MotorSta MotorSta;
extern T_DispData DispData;
extern T_MeterParm MeterParm;
extern T_MeterData MeterData;
extern T_SystemTime SystemTime;
extern T_WakeupState WakeupState;
extern T_CountState CountState;
extern T_HeartBeat HeartBeat;
extern T_MultiPacket MultiPacket;
extern uint8_t GPRSOpenData[];


bool IsBigEndian(void);
void SetMeterParm(void);
uint8_t GetMeterSta(void);
void Soft_delay_ms(uint16_t time);
void Soft_delay_us(uint16_t time);
void GetDecTime(PT_Time ptReadTime);
void SetBCDTime(PT_Time ptReadTime);
uint8_t LookForStr(uint8_t *s, uint8_t *t);
void ConvertEndian(uint8_t *buff, uint8_t num);
bool CompareData(uint8_t *a, uint8_t *b, uint16_t n);
uint16_t BCDToDec(uint8_t *bcd, uint8_t len);
void DecToBCD(uint16_t Dec, uint8_t *BCD, uint8_t len);
uint32_t NetComputeCRC(uint8_t *bufData, uint16_t buflen, uint8_t *pcrc);

#endif
