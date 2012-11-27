/* 以下为USB设备方式的应用程序, 完整程序在CH372/CH375/CH376调试工具程序包中CH372DBG.C */
/*
; 单片机内置USB调试固件程序  V1.0
; 用于连接CH372或者CH375或者CH376的单片机进行简单的调试功能
; 可以用include直接包含到应用系统的主程序中,或者添加到工程项目中
;
; Website:  http://winchiphead.com
; Email:    tech@winchiphead.com
; @2004.08
;****************************************************************************
*/

/* MCS-51单片机C语言, KC7.0 */
/* 用于其它类型单片机或者硬件资源不同时, 该程序应该根据需要进行局部修改 */

#include "CH376.H"

#define	xEndCH376Cmd( )  /* 结束CH376命令,仅用于SPI接口方式 */

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
#define MAX_DATA_SIZE		20			/* 单次命令处理的最大数据长度,有效值是1到56 */
#endif

typedef	struct	_USB_DOWN_PACKET {		/* 下传的数据包结构,用于命令/写数据 */
	UINT8	mCommand;					/* 命令码,见下面的定义 */
	UINT8	mCommandNot;				/* 命令码的反码,用于校验下传数据包 */
	union {
		UINT8	mByte[4];				/* 通用参数 */
		UINT16	mWord[2];				/* 通用参数,低字节在前,Little-Endian */
		UINT32	mDword;					/* 通用参数,低字节在前,Little-Endian */
		void	*mAddress;				/* 读写操作的起始地址,低字节在前,Little-Endian */
	} u;
	UINT8	mLength;					/* 下面的缓冲区的长度,读写操作的字节数 */
	UINT8	mBuffer[ MAX_DATA_SIZE ];	/* 数据缓冲区 */
}	USB_DOWN_PKT;

typedef	struct	_USB_UP_PACKET {		/* 上传的数据包结构,用于状态/读数据 */
	UINT8	mStatus;					/* 状态码,见下面的定义 */
	UINT8	mCommandNot;				/* 命令码的反码,用于校验上传数据包 */
	UINT8	mReserved[4];
	UINT8	mLength;					/* 下面的缓冲区的长度,读操作的字节数 */
	UINT8	mBuffer[ MAX_DATA_SIZE ];	/* 数据缓冲区 */
}	USB_UP_PKT;

typedef union	_USB_DATA_PACKET {		/* USB上传或者下传数据缓冲区 */
	USB_DOWN_PKT	down;
	USB_UP_PKT		up;
}	USB_DATA_PKT;

/* 命令码定义,按位说明
   位7为命令类型:  0=实现特定功能, 1=存储器和SFR读写
   对于"实现特定功能"命令类型:
       位6-位0为定义的具体命令码, 命令码为00H-7FH, 其中: 00H-3FH为通用标准命令, 40H-7FH为与应用系统有关的特定命令
       目前版本定义了以下通用标准命令:
           00H: 获取调试固件程序的版本,并取消未完成的上传数据块
           10H: 获取当前应用系统的版本和说明字符串
   对于"存储器和SFR读写"命令类型:
       位6为数据传输方向:      0=读操作/上传, 1=写操作/下传
       位5-位4为数据读写宽度:  00=以字节为单位/8位, 01=以字为单位/16位, 10=以双字为单位/32位, 11=以位为单位/1位
       位1-位0为存储器空间:    00=存取SFR, 01=存取内部RAM, 10=存取外部RAM, 11=存取程序ROM
       例如: 命令码80H为读SFR, 命令码83H为读程序ROM, 命令码C1H为写内部RAM, 命令码C2H为写外部RAM
   状态码定义: 00H为操作成功, 080H为命令不支持, 0FFH为未定义的错误 */

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


#define DELAY_START_VALUE		1	  /* 根据单片机的时钟选择初值,20MHz以下为0,30MHz以上为2 */

UINT8V	FreeUSBmS;

/* CH376初始化子程序 */
void	CH376DeviceInit( void ) {
	UINT8 i;
/* 设置USB工作模式, 必要操作 */
	xWriteCH376Cmd( CMD_SET_USB_MODE );
	xWriteCH376Data( 2 );  /* 设置为使用内置固件的USB设备方式 */
	for ( i=100; i!=0; i-- ) if ( xReadCH376Data( ) == CMD_RET_SUCCESS ) break;  /* 等待操作成功,通常需要等待10uS-20uS */
	xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
/*	if ( i == 0 ) { CH372/CH375/CH376芯片内部或者物理连接存在硬件错误 }; */

/* 下面启用USB中断,CH376的INT#引脚可以连接到单片机的中断引脚,中断为低电平有效或者下降沿有效,
   如果不使用中断,那么也可以用查询方式,由单片机程序查询CH376的INT#引脚为低电平 */
	IT0 = 0;  /* 置外部信号为低电平触发 */
	IE0 = 0;  /* 清中断标志 */
	EX0 = 1;  /* 允许CH376中断,假定CH376的INT#引脚连接到单片机的INT0 */
}

/* CH376中断服务程序,假定CH376的INT#引脚连接到单片机的INT0,使用寄存器组1 */
void	mCH376Interrupt( void ) interrupt 0 using 1 {
	UINT8			cnt;
	UINT8			dat;
	PUINT8			buf;
	PUINT8C			str;
/*	USB_DATA_PKT	udp;*/
#define	pudp		( (USB_DATA_PKT *)&buf )	/* 节约结构变量占用的内存,因为USB主从不会同时运行,所以USB主机的变量可以用于USB设备 */
#define	IntStatus		dat		/* 节约一个变量存储单元 */
	xWriteCH376Cmd( CMD_GET_STATUS );  /* 获取中断状态并取消中断请求 */
	FreeUSBmS = 0;  /* 清除USB空闲计时 */
	IntStatus = xReadCH376Data( );  /* 获取中断状态 */
	xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
/*	IE0 = 0;  清中断标志,与单片机硬件有关,对应于INT0中断 */
	if ( IntStatus == USB_INT_EP2_OUT ) {  /* 批量端点下传成功,接收到命令包 */
		xWriteCH376Cmd( CMD_RD_USB_DATA );  /* 从当前USB中断的端点缓冲区读取数据块,并释放缓冲区 */
		cnt = xReadCH376Data( );  /* 首先读取后续数据长度 */
		if ( cnt == 0 ) {  /* 长度为0,没有数据,在某些应用中也可以将长度0定义为一种特殊命令 */
			xWriteCH376Cmd( CMD_SET_ENDP7 );  /* 设置USB端点2的IN,也就是批量上传端点 */
			xWriteCH376Data( 0x0E );  /* 同步触发位不变,设置USB端点2的IN正忙,返回NAK,实际是清除上传缓冲区的已有内容 */
			xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
			return;
		}
		buf = (PUINT8)&pudp->down;  /* 指令命令包缓冲区 */
		do {
			*buf = xReadCH376Data( );  /* 接收命令包的数据 */
			buf ++;
		} while ( -- cnt );
		xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
		if ( pudp->down.mCommand != (UINT8)( ~ pudp->down.mCommandNot ) ) return;  /* 命令包反码校验错误,放弃该下传包 */
		if ( pudp->down.mCommand & USB_CMD_MEM_ACCESS ) {  /* 命令类型:存储器和SFR读写 */
			if ( ( pudp->down.mCommand & USB_CMD_MEM_WIDTH ) != USB_CMD_MEM_W_BYTE ) {  /* 本程序目前对MCS51只支持以字节为单位进行读写 */
				pudp->up.mLength = 0;
				pudp->up.mStatus = ERR_UNSUPPORT;  /* 命令不支持 */
			}
			else {  /* 以字节为单位进行读写 */
				for ( cnt = 0; cnt != pudp->down.mLength; cnt ++ ) {  /* 读写操作计数 */
					dat = pudp->down.mBuffer[ cnt ];  /* 准备写入的数据 */
					switch( pudp->down.mCommand & USB_CMD_MEM_SPACE ) {  /* 存储器空间 */
						case USB_CMD_MEM_S_SFR:
							switch ( pudp->down.u.mByte[0] ) {  /* 分析SFR地址 */
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
							if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) pudp->up.mStatus = ERR_UNSUPPORT;  /* 命令不支持 */
							else dat = *(PUINT8C)( pudp->down.u.mByte[0] | (UINT16)pudp->down.u.mByte[1] << 8 );
							break;
					}
					if ( ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) == 0 ) pudp->up.mBuffer[ cnt ] = dat;  /* 返回读出的数据 */
					pudp->down.u.mByte[0] ++;
					if ( pudp->down.u.mByte[0] == 0 ) pudp->down.u.mByte[1] ++;
				}
				if ( pudp->down.mCommand & USB_CMD_MEM_DIR_WR ) pudp->up.mLength = 0;  /* 写操作不返回数据 */
				pudp->up.mStatus = ERR_SUCCESS;
			}
		}
		else switch ( pudp->down.mCommand ) {  /* 命令类型:实现特定功能,分析命令码 */
			case USB_CMD_GET_FW_INFO:  /* 获取调试固件程序的版本,并取消未完成的上传数据块 */
				pudp->up.mBuffer[0] = THIS_FIRMWARE_VER;
				pudp->up.mLength = 1;
				pudp->up.mStatus = ERR_SUCCESS;
				xWriteCH376Cmd( CMD_SET_ENDP7 );  /* 设置USB端点2的IN,也就是批量上传端点 */
				xWriteCH376Data( 0x0E );  /* 同步触发位不变,设置USB端点2的IN正忙,返回NAK,实际是清除上传缓冲区的已有内容 */
				xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
				break;
			case USB_CMD_GET_APP_INFO:  /* 获取当前应用系统的版本和说明字符串 */
				pudp->up.mBuffer[0] = THIS_APP_SYS_VER;
				cnt = 0;
				str = THIS_APP_SYS_STR;
				while ( pudp->up.mBuffer[ cnt ] = *str ) { cnt ++; str ++; }  /* 说明字符串 */
				pudp->up.mLength = 1 + sizeof( THIS_APP_SYS_STR );
				pudp->up.mStatus = ERR_SUCCESS;
				break;
/* 			case MY_CMD_CH451: */
			default:  /* 命令不支持 */
				pudp->up.mLength = 0;
				pudp->up.mStatus = ERR_UNSUPPORT;
				break;
		}
		xWriteCH376Cmd( CMD_WR_USB_DATA7 );  /* 向USB端点2的发送缓冲区写入数据块 */
		cnt = pudp->up.mLength + (UINT8)( & ( (USB_UP_PKT *)0 ) -> mBuffer );
		xWriteCH376Data( cnt );  /* 首先写入后续数据长度 */
		buf = (PUINT8)&pudp->up;  /* 指向状态包缓冲区 */
		do {
			xWriteCH376Data( *buf );  /* 写入数据到CH376 */
			buf ++;
		} while ( -- cnt );
		xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
	}
	else if ( IntStatus == USB_INT_EP2_IN ) {  /* 批量数据发送成功,状态包已发送 */
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* 释放当前USB缓冲区,收到上传成功中断后,必须解锁USB缓冲区,以便继续收发 */
		xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
	}
	else if ( IntStatus == USB_INT_EP1_IN ) {  /* 中断数据发送成功,本程序未用到 */
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* 释放当前USB缓冲区 */
		xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
	}
/* 内置固件的USB方式下不应该出现其它中断状态 */
	else {
		xWriteCH376Cmd( CMD_UNLOCK_USB );  /* 释放当前USB缓冲区 */
		xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
	}
}

/* 关闭CH376的所有USB通讯 */
void	CH376OffUSB( void ) {
	EX0 = 0;  /* 关闭USB中断,本程序中USB主机模式下使用查询方式 */
/* 设置USB工作模式, 必要操作 */
	xWriteCH376Cmd( CMD_SET_USB_MODE );
	xWriteCH376Data( 0 );  /* 设置为未启用的USB设备方式 */
	xEndCH376Cmd( );  /* 结束CH376命令,仅用于SPI接口方式 */
	mDelaymS( 10 );  /* 为USB主从切换进行时间缓冲,这是必要的延时操作,用于让计算机认为USB设备已经撤离 */
/* 如果CH376仍然连接着计算机,而程序使CH376切换到USB主机模式,那么会导致与计算机之间双USB主机冲突 */
}

void	device( ) {
	CH376DeviceInit( );  /* 初始化USB设备模式 */
	FreeUSBmS = 0;  /* 清除USB空闲计时 */
	while( 1 ) {
		if ( IsKeyPress( ) ) {  /* 有键按下 */
			if ( FreeUSBmS >= 250 ) {  /* USB空闲超过250毫秒 */
				printf( "Exit USB device mode\n" );
				CH376OffUSB( );  /* 关闭USB设备 */
				return;
			}
		}
		if ( FreeUSBmS < 250 ) FreeUSBmS ++;  /* USB空闲计时,避免在USB通讯过程中由用户按键导致USB主从切换 */
		mDelaymS( 1 );
/* USB设备模式全部在中断服务中处理,主程序可以做其它事情,当然也可以在主程序中使用查询方式处理USB设备的通讯 */
	}
}
