/* ����ΪUSB�豸��ʽ��Ӧ�ó���, ����������CH372/CH375/CH376���Թ��߳������CH372DBG.C */
/*
; ��Ƭ������USB���Թ̼�����  V1.0
; ��������CH372����CH375����CH376�ĵ�Ƭ�����м򵥵ĵ��Թ���
; ������includeֱ�Ӱ�����Ӧ��ϵͳ����������,�������ӵ�������Ŀ��
;
; Website:  http://winchiphead.com
; Email:    tech@winchiphead.com
; @2004.08
;****************************************************************************
*/

/* MCS-51��Ƭ��C����, KC7.0 */
/* �����������͵�Ƭ������Ӳ����Դ��ͬʱ, �ó���Ӧ�ø�����Ҫ���оֲ��޸� */

#include "CH376.H"

#define	xEndCH376Cmd( )  /* ����CH376����,������SPI�ӿڷ�ʽ */

#ifdef __C51__
#pragma NOAREGS
#endif

#ifdef __C51__
#ifndef PUINT8X
typedef unsigned char xdata        *PUINT8X;
#endif
#ifndef PUINT8C
typedef unsigned char code         *PUINT8C;
#endif
#endif

#ifndef MAX_DATA_SIZE
#define MAX_DATA_SIZE		20			/* �����������������ݳ���,��Чֵ��1��56 */
#endif

typedef	struct	_USB_DOWN_PACKET {		/* �´������ݰ��ṹ,��������/д���� */
	UINT8	mCommand;					/* ������,������Ķ��� */
	UINT8	mCommandNot;				/* ������ķ���,����У���´����ݰ� */
	union {
		UINT8	mByte[4];				/* ͨ�ò��� */
		UINT16	mWord[2];				/* ͨ�ò���,���ֽ���ǰ,Little-Endian */
		UINT32	mDword;					/* ͨ�ò���,���ֽ���ǰ,Little-Endian */
		void	*mAddress;				/* ��д��������ʼ��ַ,���ֽ���ǰ,Little-Endian */
	} u;
	UINT8	mLength;					/* ����Ļ������ĳ���,��д�������ֽ��� */
	UINT8	mBuffer[ MAX_DATA_SIZE ];	/* ���ݻ����� */
}	USB_DOWN_PKT;

typedef	struct	_USB_UP_PACKET {		/* �ϴ������ݰ��ṹ,����״̬/������ */
	UINT8	mStatus;					/* ״̬��,������Ķ��� */
	UINT8	mCommandNot;				/* ������ķ���,����У���ϴ����ݰ� */
	UINT8	mReserved[4];
	UINT8	mLength;					/* ����Ļ������ĳ���,���������ֽ��� */
	UINT8	mBuffer[ MAX_DATA_SIZE ];	/* ���ݻ����� */
}	USB_UP_PKT;

typedef union	_USB_DATA_PACKET {		/* USB�ϴ������´����ݻ����� */
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
}	USB_DATA_PKT;

/* �����붨��,��λ˵��
   λ7Ϊ��������:  0=ʵ���ض�����, 1=�洢����SFR��д
   ����"ʵ���ض�����"��������:
       λ6-λ0Ϊ����ľ���������, ������Ϊ00H-7FH, ����: 00H-3FHΪͨ�ñ�׼����, 40H-7FHΪ��Ӧ��ϵͳ�йص��ض�����
       Ŀǰ�汾����������ͨ�ñ�׼����:
           00H: ��ȡ���Թ̼�����İ汾,��ȡ��δ��ɵ��ϴ����ݿ�
           10H: ��ȡ��ǰӦ��ϵͳ�İ汾��˵���ַ���
   ����"�洢����SFR��д"��������:
       λ6Ϊ���ݴ��䷽��:      0=������/�ϴ�, 1=д����/�´�
       λ5-λ4Ϊ���ݶ�д����:  00=���ֽ�Ϊ��λ/8λ, 01=����Ϊ��λ/16λ, 10=��˫��Ϊ��λ/32λ, 11=��λΪ��λ/1λ
       λ1-λ0Ϊ�洢���ռ�:    00=��ȡSFR, 01=��ȡ�ڲ�RAM, 10=��ȡ�ⲿRAM, 11=��ȡ����ROM
       ����: ������80HΪ��SFR, ������83HΪ������ROM, ������C1HΪд�ڲ�RAM, ������C2HΪд�ⲿRAM
   ״̬�붨��: 00HΪ�����ɹ�, 080HΪ���֧��, 0FFHΪδ����Ĵ��� */

#define USB_CMD_GET_FW_INFO		0x00
#define USB_CMD_GET_APP_INFO	0x10

#define USB_CMD_MEM_ACCESS		0x80
#define USB_CMD_MEM_DIR_WR		0x40
#define USB_CMD_MEM_WIDTH		0x0C
#define USB_CMD_MEM_W_BYTE		0x00
#define USB_CMD_MEM_W_WORD		0x04
#define USB_CMD_MEM_W_DWORD		0x08
#define USB_CMD_MEM_W_BIT		0x0C
#define USB_CMD_MEM_SPACE		0x03
#define USB_CMD_MEM_S_SFR		0x00
#define USB_CMD_MEM_S_IRAM		0x01
#define USB_CMD_MEM_S_XRAM		0x02
#define USB_CMD_MEM_S_ROM		0x03

#define ERR_SUCCESS				0x00
#define ERR_PARAMETER			0x10
#define ERR_UNSUPPORT			0x80
#define ERR_UNDEFINED			0xFF

#define THIS_FIRMWARE_VER		0x10
#define THIS_APP_SYS_VER		0x09
#define THIS_APP_SYS_STR		"CH376+MCS51"


#define DELAY_START_VALUE		1	  /* ���ݵ�Ƭ����ʱ��ѡ���ֵ,20MHz����Ϊ0,30MHz����Ϊ2 */

UINT8V	FreeUSBmS;

/* CH376��ʼ���ӳ��� */
void	CH376DeviceInit( void ) {
	UINT8 i;
/* ����USB����ģʽ, ��Ҫ���� */
	xWriteCH376Cmd( CMD_SET_USB_MODE );
	xWriteCH376Data( 2 );  /* ����Ϊʹ�����ù̼���USB�豸��ʽ */
	for ( i=100; i!=0; i-- ) if ( xReadCH376Data( ) == CMD_RET_SUCCESS ) break;  /* �ȴ������ɹ�,ͨ����Ҫ�ȴ�10uS-20uS */
	xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
/*	if ( i == 0 ) { CH372/CH375/CH376оƬ�ڲ������������Ӵ���Ӳ������ }; */

/* ��������USB�ж�,CH376��INT#���ſ������ӵ���Ƭ�����ж�����,�ж�Ϊ�͵�ƽ��Ч�����½�����Ч,
   �����ʹ���ж�,��ôҲ�����ò�ѯ��ʽ,�ɵ�Ƭ�������ѯCH376��INT#����Ϊ�͵�ƽ */
	IT0 = 0;  /* ���ⲿ�ź�Ϊ�͵�ƽ���� */
	IE0 = 0;  /* ���жϱ�־ */
	EX0 = 1;  /* ����CH376�ж�,�ٶ�CH376��INT#�������ӵ���Ƭ����INT0 */
}

/* CH376�жϷ������,�ٶ�CH376��INT#�������ӵ���Ƭ����INT0,ʹ�üĴ�����1 */
void	mCH376Interrupt( void ) interrupt 0 using 1 {
	UINT8			cnt;
	UINT8			dat;
	PUINT8			buf;
	PUINT8C			str;
/*	USB_DATA_PKT	udp;*/
#define	pudp		( (USB_DATA_PKT *)&buf )	/* ��Լ�ṹ����ռ�õ��ڴ�,��ΪUSB���Ӳ���ͬʱ����,����USB�����ı�����������USB�豸 */
#define	IntStatus		dat		/* ��Լһ�������洢��Ԫ */
	xWriteCH376Cmd( CMD_GET_STATUS );  /* ��ȡ�ж�״̬��ȡ���ж����� */
	FreeUSBmS = 0;  /* ���USB���м�ʱ */
	IntStatus = xReadCH376Data( );  /* ��ȡ�ж�״̬ */
	xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
/*	IE0 = 0;  ���жϱ�־,�뵥Ƭ��Ӳ���й�,��Ӧ��INT0�ж� */
	if ( IntStatus == USB_INT_EP2_OUT ) {  /* �����˵��´��ɹ�,���յ������ */
		xWriteCH376Cmd( CMD_RD_USB_DATA );  /* �ӵ�ǰUSB�жϵĶ˵㻺������ȡ���ݿ�,���ͷŻ����� */
		cnt = xReadCH376Data( );  /* ���ȶ�ȡ�������ݳ��� */
		if ( cnt == 0 ) {  /* ����Ϊ0,û������,��ĳЩӦ����Ҳ���Խ�����0����Ϊһ���������� */
			xWriteCH376Cmd( CMD_SET_ENDP7 );  /* ����USB�˵�2��IN,Ҳ���������ϴ��˵� */
			xWriteCH376Data( 0x0E );  /* ͬ������λ����,����USB�˵�2��IN��æ,����NAK,ʵ��������ϴ����������������� */
			xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
			return;
		}
		buf = (PUINT8)&pudp->down;  /* ָ������������� */
		do {
			*buf = xReadCH376Data( );  /* ��������������� */
			buf ++;
		} while ( -- cnt );
		xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
		if ( pudp->down.mCommand != (UINT8)( ~ pudp->down.mCommandNot ) ) return;  /* ���������У�����,�������´��� */
		if ( pudp->down.mCommand & USB_CMD_MEM_ACCESS ) {  /* ��������:�洢����SFR��д */
			if ( ( pudp->down.mCommand & USB_CMD_MEM_WIDTH ) != USB_CMD_MEM_W_BYTE ) {  /* ������Ŀǰ��MCS51ֻ֧�����ֽ�Ϊ��λ���ж�д */
				pudp->up.mLength = 0;
				pudp->up.mStatus = ERR_UNSUPPORT;  /* ���֧�� */
			}
			else {  /* ���ֽ�Ϊ��λ���ж�д */
				for ( cnt = 0; cnt != pudp->down.mLength; cnt ++ ) {  /* ��д�������� */
					dat = pudp->down.mBuffer[ cnt ];  /* ׼��д������� */
					switch( pudp->down.mCommand & USB_CMD_MEM_SPACE ) {  /* �洢���ռ� */
						case USB_CMD_MEM_S_SFR:
							switch ( pudp->down.u.mByte[0] ) {  /* ����SFR��ַ */
								case 0x80:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) P0 = dat;
									else dat = P0;
									break;
								case 0x87:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) PCON = dat;
									else dat = PCON;
									break;
								case 0x88:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) TCON = dat;
									else dat = TCON;
									break;
								case 0x89:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) TMOD = dat;
									else dat = TMOD;
									break;
								case 0x90:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) P1 = dat;
									else dat = P1;
									break;
								case 0x98:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) SCON = dat;
									else dat = SCON;
									break;
								case 0x99:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) SBUF = dat;
									else dat = SBUF;
									break;
								case 0xA0:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) P2 = dat;
									else dat = P2;
									break;
								case 0xA8:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) IE = dat;
									else dat = IE;
									break;
								case 0xB0:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) P3 = dat;
									else dat = P3;
									break;
								case 0xB8:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) IP = dat;
									else dat = IP;
									break;
								case 0xC8:
									if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) T2CON = dat;
									else dat = T2CON;
									break;
								default:
									dat = 0;
									break;
							}
							break;
						case USB_CMD_MEM_S_IRAM:
							if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) *(PUINT8)pudp->down.u.mByte[0] = dat;
							else dat = *(PUINT8)pudp->down.u.mByte[0];
							break;
						case USB_CMD_MEM_S_XRAM:
							if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) *(PUINT8X)( pudp->down.u.mByte[0] | (UINT16)pudp->down.u.mByte[1] << 8 )= dat;
							else dat = *(PUINT8X)( pudp->down.u.mByte[0] | (UINT16)pudp->down.u.mByte[1] << 8 );
							break;
						case USB_CMD_MEM_S_ROM:
							if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) pudp->up.mStatus = ERR_UNSUPPORT;  /* ���֧�� */
							else dat = *(PUINT8C)( pudp->down.u.mByte[0] | (UINT16)pudp->down.u.mByte[1] << 8 );
							break;
					}
					if ( ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) == 0 ) pudp->up.mBuffer[ cnt ] = dat;  /* ���ض��������� */
					pudp->down.u.mByte[0] ++;
					if ( pudp->down.u.mByte[0] == 0 ) pudp->down.u.mByte[1] ++;
				}
				if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) pudp->up.mLength = 0;  /* д�������������� */
				pudp->up.mStatus = ERR_SUCCESS;
			}
		}
		else switch ( pudp->down.mCommand ) {  /* ��������:ʵ���ض�����,���������� */
			case USB_CMD_GET_FW_INFO:  /* ��ȡ���Թ̼�����İ汾,��ȡ��δ��ɵ��ϴ����ݿ� */
				pudp->up.mBuffer[0] = THIS_FIRMWARE_VER;
				pudp->up.mLength = 1;
				pudp->up.mStatus = ERR_SUCCESS;
				xWriteCH376Cmd( CMD_SET_ENDP7 );  /* ����USB�˵�2��IN,Ҳ���������ϴ��˵� */
				xWriteCH376Data( 0x0E );  /* ͬ������λ����,����USB�˵�2��IN��æ,����NAK,ʵ��������ϴ����������������� */
				xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
				break;
			case USB_CMD_GET_APP_INFO:  /* ��ȡ��ǰӦ��ϵͳ�İ汾��˵���ַ��� */
				pudp->up.mBuffer[0] = THIS_APP_SYS_VER;
				cnt = 0;
				str = THIS_APP_SYS_STR;
				while ( pudp->up.mBuffer[ cnt ] = *str ) { cnt ++; str ++; }  /* ˵���ַ��� */
				pudp->up.mLength = 1 + sizeof( THIS_APP_SYS_STR );
				pudp->up.mStatus = ERR_SUCCESS;
				break;
/* 			case MY_CMD_CH451: */
			default:  /* ���֧�� */
				pudp->up.mLength = 0;
				pudp->up.mStatus = ERR_UNSUPPORT;
				break;
		}
		xWriteCH376Cmd( CMD_WR_USB_DATA7 );  /* ��USB�˵�2�ķ��ͻ�����д�����ݿ� */
		cnt = pudp->up.mLength + (UINT8)( & ( (USB_UP_PKT *)0 ) -> mBuffer );
		xWriteCH376Data( cnt );  /* ����д��������ݳ��� */
		buf = (PUINT8)&pudp->up;  /* ָ��״̬�������� */
		do {
			xWriteCH376Data( *buf );  /* д�����ݵ�CH376 */
			buf ++;
		} while ( -- cnt );
		xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
	}
	else if ( IntStatus == USB_INT_EP2_IN ) {  /* �������ݷ��ͳɹ�,״̬���ѷ��� */
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* �ͷŵ�ǰUSB������,�յ��ϴ��ɹ��жϺ�,�������USB������,�Ա�����շ� */
		xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
	}
	else if ( IntStatus == USB_INT_EP1_IN ) {  /* �ж����ݷ��ͳɹ�,������δ�õ� */
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* �ͷŵ�ǰUSB������ */
		xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
	}
/* ���ù̼���USB��ʽ�²�Ӧ�ó��������ж�״̬ */
	else {
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* �ͷŵ�ǰUSB������ */
		xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
	}
}

/* �ر�CH376������USBͨѶ */
void	CH376OffUSB( void ) {
	EX0 = 0;  /* �ر�USB�ж�,��������USB����ģʽ��ʹ�ò�ѯ��ʽ */
/* ����USB����ģʽ, ��Ҫ���� */
	xWriteCH376Cmd( CMD_SET_USB_MODE );
	xWriteCH376Data( 0 );  /* ����Ϊδ���õ�USB�豸��ʽ */
	xEndCH376Cmd( );  /* ����CH376����,������SPI�ӿڷ�ʽ */
	mDelaymS( 10 );  /* ΪUSB�����л�����ʱ�仺��,���Ǳ�Ҫ����ʱ����,�����ü������ΪUSB�豸�Ѿ����� */
/* ���CH376��Ȼ�����ż����,������ʹCH376�л���USB����ģʽ,��ô�ᵼ��������֮��˫USB������ͻ */
}

void	device( ) {
	CH376DeviceInit( );  /* ��ʼ��USB�豸ģʽ */
	FreeUSBmS = 0;  /* ���USB���м�ʱ */
	while( 1 ) {
		if ( IsKeyPress( ) ) {  /* �м����� */
			if ( FreeUSBmS >= 250 ) {  /* USB���г���250���� */
				printf( "Exit USB device mode\n" );
				CH376OffUSB( );  /* �ر�USB�豸 */
				return;
			}
		}
		if ( FreeUSBmS < 250 ) FreeUSBmS ++;  /* USB���м�ʱ,������USBͨѶ���������û���������USB�����л� */
		mDelaymS( 1 );
/* USB�豸ģʽȫ�����жϷ����д���,�������������������,��ȻҲ��������������ʹ�ò�ѯ��ʽ����USB�豸��ͨѶ */
	}
}