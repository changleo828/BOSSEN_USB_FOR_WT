/* 2008.10.18
****************************************
**  Copyright  (C)  W.ch  1999-2009   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Interface for CH376 **
**  TC2.0@PC, KC7.0@MCS51             **
****************************************
*/
/* CH376 主机文件系统接口 */

/* MCS-51单片机C语言的U盘文件读写示例程序 */
/* 本程序用于演示检查U盘是否写保护,演示模拟计算机端的安全移除,也可以参考用于自行处理其它命令 */

/* C51   CH376HFT.C */
/* LX51  CH376HFT.OBJ */
/* OHX51 CH376HFT */

#include <reg52.h>
#include <stdio.h>
#include <string.h>

#include "..\HAL.H"
#include "..\HAL_BASE.C"
#include "..\DEBUG.H"
#include "..\DEBUG.C"
#include "..\PARA_HW.C"		/* 硬件总线并口连接方式 */
//#include "..\PARA_SW.C"		/* 软件模拟并口连接方式 */
//#include "..\SPI_HW.C"		/* 硬件SPI连接方式 */
//#include "..\SPI_SW.C"		/* 软件模拟SPI方式 */
//#include "..\UART_HW.C"		/* 硬件异步串口连接方式 */
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

UINT8		idata	buf[64];

/* 检查U盘是否写保护, 返回USB_INT_SUCCESS说明可以写,返回0xFF说明只读/写保护,返回其它值说明是错误代码 */
/* 其它BulkOnly传输协议的命令可以参考这个例子处理,修改前必须了解USB MassStorage和SCSI规范 */
UINT8	IsDiskWriteProtect( void )
{
	UINT8			s, SysBaseInfo;
	P_BULK_ONLY_CBW	pCbw;
	SysBaseInfo = CH376ReadVar8( VAR_SYS_BASE_INFO );  /* 当前系统的基本信息 */
	pCbw = (P_BULK_ONLY_CBW)buf;
	for ( s = 0; s != sizeof( pCbw -> CBW_CB_Buf ); s ++ ) pCbw -> CBW_CB_Buf[ s ] = 0;  /* 默认清0 */
	pCbw -> CBW_DataLen0 = 0x10;  /* 数据传输长度 */
	pCbw -> CBW_Flag = 0x80;  /* 传输方向为输入 */
	if ( SysBaseInfo & 0x40 ) {  /* SubClass-Code子类别非6 */
		pCbw -> CBW_CB_Len = 10;  /* 命令块的长度 */
		pCbw -> CBW_CB_Buf[0] = 0x5A;  /* 命令块首字节, MODE SENSE(10) */
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[8] = 0x10;
	}
	else {  /* SubClass-Code子类别为6 */
		pCbw -> CBW_CB_Len = 6;  /* 命令块的长度 */
		pCbw -> CBW_CB_Buf[0] = 0x1A;  /* 命令块首字节, MODE SENSE(6) */
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[4] = 0x10;
	}
	CH376WriteHostBlock( (PUINT8)pCbw, sizeof( BULK_ONLY_CBW ) );  /* 向USB主机端点的发送缓冲区写入数据块,剩余部分CH376自动填补 */
	s = CH376SendCmdWaitInt( CMD0H_DISK_BOC_CMD );  /* 对U盘执行BulkOnly传输协议 */
	if ( s == USB_INT_SUCCESS ) {
		s = CH376ReadBlock( buf );  /* 从当前主机端点的接收缓冲区读取数据块,返回长度 */
		if ( s > 3 ) {  /* MODE SENSE命令返回数据的长度有效 */
			if ( SysBaseInfo & 0x40 ) s = buf[3];  /* MODE SENSE(10), device specific parameter */
			else s = buf[2];  /* MODE SENSE(6), device specific parameter */
			if ( s & 0x80 ) return( 0xFF );  /* U盘写保护 */
			else return( USB_INT_SUCCESS );  /* U盘没有写保护 */
		}
		return( USB_INT_DISK_ERR );
	}
	CH376DiskReqSense( );  /* 检查USB存储器错误 */
	return( s );
}

/* 安全移除U盘, 返回USB_INT_SUCCESS说明可以安全移除,否则说明不能安全移除,只能强行移除 */
/* 在操作完U盘准备让用户拔出U盘前调用, 防止用户过早拔出U盘丢失数据 */
UINT8	SafeRemoveDisk( void )
{
	UINT8	i, s;
	for ( i = 0; i < 20; i ++ ) {  /* 有的U盘总是返回未准备好,不过可以被忽略 */
		mDelaymS( 50 );
		if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* 查询磁盘是否准备好 */
	}
	mDelaymS( 5 );
	s = CH376SendCmdDatWaitInt( CMD1H_SET_CONFIG, 0 );  /* 取消配置,会导致很多U盘的LED灯灭 */
	mDelaymS( 5 );
	if ( i < 20 && s == USB_INT_SUCCESS ) return( USB_INT_SUCCESS );  /* 操作成功 */
	else return( 0xFF );
/* 以下取消SOF包会导致绝大多数U盘的LED灯灭 */
/* 如果此处取消SOF, 那么与此对应, 在检测到U盘插入后应该再恢复为模式6即恢复SOF包 */
//	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
//	xWriteCH376Data( 0x05 );  /* 停止发出SOF包,检测到U盘插入后必须用模式6恢复SOF包发出 */
//	mDelaymS( 2 );
//	xReadCH376Data( );
//	xEndCH376Cmd( );
}

main( ) {
	UINT8	s;
	mDelaymS( 100 );  /* 延时100毫秒 */
	mInitSTDIO( );  /* 为了让计算机通过串口监控演示过程 */
	printf( "Start\n" );

	s = mInitCH376Host( );  /* 初始化CH376 */
	mStopIfError( s );
/* 其它电路初始化 */

	while ( 1 ) {
		printf( "Wait Udisk\n" );

//		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
//			mDelaymS( 100 );
//		}
		while ( 1 ) {  /* 与前几行的方法不同,此处是等到中断通知后再查询USB设备连接 */
			if ( Query376Interrupt( ) ) {  /* 查询CH376中断(INT#引脚为低电平) */
				if ( CH376DiskConnect( ) == USB_INT_SUCCESS ) break;  /* USB设备连接 */
			}
			mDelaymS( 100 );
		}

		mDelaymS( 200 );  /* 延时,可选操作,有的USB存储器需要几十毫秒的延时 */

/* 对于检测到USB设备的,最多等待10*50mS */
		for ( s = 0; s < 10; s ++ ) {  /* 最长等待时间,10*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* 初始化磁盘并测试磁盘是否就绪 */
		}
		s = CH376ReadBlock( buf );  /* 如果需要,可以读取数据块CH376_CMD_DATA.DiskMountInq,返回长度 */
		if ( s == sizeof( INQUIRY_DATA ) ) {  /* U盘的厂商和产品信息 */
			buf[ s ] = 0;
			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		}

		printf( "Check Disk Write Protect ? ...\n" );
		s = IsDiskWriteProtect( );  /* 检查U盘是否写保护, 返回USB_INT_SUCCESS说明可以写,返回0xFF说明只读/写保护,返回其它值说明是错误代码 */
		if ( s != USB_INT_SUCCESS && s != 0xFF ) {  /* 操作失败 */
			printf( "Again ...\n" );
			mDelaymS( 100 );
			s = IsDiskWriteProtect( );  /* 再试一次 */
		}
		if ( s == USB_INT_SUCCESS ) {  /* 可以写 */
			printf( "... No !\n" );
			printf( "Create a File\n" );
			s = CH376FileCreate( "\\NEWFILE.TXT" );  /* 在根目录下新建文件并打开,如果文件已经存在则先删除后再新建 */
			mStopIfError( s );
			buf[0] = 'O';
			buf[1] = 'K';
			s = CH376ByteWrite( buf, 2, NULL );  /* 向文件写入数据 */
			mStopIfError( s );
			printf( "Close\n" );
			s = CH376FileClose( TRUE );  /* 自动计算文件长度 */
			mStopIfError( s );
			if ( SafeRemoveDisk( ) == USB_INT_SUCCESS ) {  /* 安全移除U盘 */
				printf( "Safe Remove !\n" );
			}
			else {
				printf( "Unsafe Remove !\n" );
			}
		}
		else if ( s == 0xFF ) {  /* 写保护 */
			printf( "... Yes !\n" );
		}
		else {
			mStopIfError( s );  /* 显示错误代码 */
		}

		printf( "Take out\n" );

//		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
//			mDelaymS( 100 );
//		}
		while ( 1 ) {  /* 与前几行的方法不同,此处是等到中断通知后再查询USB设备连接 */
			if ( Query376Interrupt( ) ) {  /* 查询CH376中断(INT#引脚为低电平) */
				if ( CH376DiskConnect( ) != USB_INT_SUCCESS ) break;  /* USB设备断开 */
			}
			mDelaymS( 100 );
		}

		mDelaymS( 200 );

	}
}
