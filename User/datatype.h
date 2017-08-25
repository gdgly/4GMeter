#ifndef _DATATYPE_H
#define _DATATYPE_H


typedef unsigned char		BYTE;	// 8-bit unsigned

typedef union _BYTE_VAL
{
    BYTE Val;
    struct
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    } bits;
} BYTE_VAL, BYTE_BITS;

typedef union _METER_STA
{
    BYTE Val;
    struct
    {
		unsigned char flow:1;     /* ���� */
		unsigned char magnet:1;   /* ���� */
		unsigned char samp:1;     /* ���� */
		unsigned char defend:1;   /* ���� */
		unsigned char force:1;    /* ǿ�� */
        unsigned char motor:1;    /* ����״̬*/
        unsigned char margin:1;   /* ������ */
        unsigned char newmeter:1; /* �±� */
    } bits;
} METER_STA, METER_BITS;

#endif
