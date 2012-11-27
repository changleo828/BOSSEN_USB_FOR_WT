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
/* 本程序用于演示处理文件目录项FAT_DIR_INFO,例如:修改文件名,设置文件的创建日期和时间等 */

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

/* 修改指定文件的文件名,如果是C文件则修改为TXT文件 */
/* 输入参数:   iOldName 原始文件名 */
/* 返回状态码: USB_INT_SUCCESS = 修改文件名成功,
               其它状态码参考CH376INC.H */
UINT8	RenameFileName( PUINT8 iOldName )
{
	UINT8			s;
	P_FAT_DIR_INFO	pDir;
	s = CH376FileOpen( iOldName );  /* 打开原文件 */
	if ( s == USB_INT_SUCCESS ) {
		/* 文件读写操作等... */
		s = CH376DirInfoRead( );  /* 读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中 */
		if ( s == USB_INT_SUCCESS ) {
			CH376ReadBlock( buf );  /* 从内存缓冲区读取FAT_DIR_INFO数据块,返回长度总是sizeof(FAT_DIR_INFO) */
			pDir = (P_FAT_DIR_INFO)buf;  /* 当前文件目录信息 */
			if ( pDir -> DIR_Name[8] == 'C' && pDir -> DIR_Name[9] == ' ' ) {  /* 文件扩展名是C */
				pDir -> DIR_Name[8] = 'T';  /* 修改文件扩展名为TXT */
				pDir -> DIR_Name[9] = 'X';  /* 同样方法可以修改文件主名 */
				pDir -> DIR_Name[10] = 'T';
				s = CH376DirInfoRead( );  /* 读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中 */
				if ( s == USB_INT_SUCCESS ) {  /* 当前文件的目录信息FAT_DIR_INFO已经调入内存缓冲区 */
					CH376WriteOfsBlock( &buf[8], 8, 3 );  /* 向内部缓冲区偏移地址8(跳过主文件名)写入3个字节(修改的扩展名) */
					s = CH376DirInfoSave( );  /* 保存文件的目录信息 */
					if ( s == USB_INT_SUCCESS ) {  /* 成功修改并保存 */
						/* 文件读写操作等... */
					}
				}
			}
			else {  /* 文件扩展名不是C */
			}
		}
		CH376FileClose( FALSE );  /* 关闭文件 */
	}
	return( s );
}

/* 为指定文件设置创建日期和时间 */
/* 输入参数:   原始文件名在iFileName中, 新的创建日期和时间: iCreateDate, iCreateTime */
/* 返回状态码: USB_INT_SUCCESS = 设置成功,
               其它状态码参考CH376INC.H */
UINT8	SetFileCreateTime( PUINT8 iFileName, UINT16 iCreateDate, UINT16 iCreateTime )
{
	UINT8	s;
	s = CH376FileOpen( iFileName );  /* 打开文件 */
	if ( s == USB_INT_SUCCESS ) {
		/* 文件读写操作等... */
		s = CH376DirInfoRead( );  /* 读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中 */
		if ( s == USB_INT_SUCCESS ) {
			buf[0] = (UINT8)iCreateTime;  /* 文件创建的时间,低8位在前 */
			buf[1] = (UINT8)(iCreateTime>>8);  /* 文件创建的时间,高8位在后 */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtTime ), 2 );  /* 向内部缓冲区偏移地址DIR_CrtTime写入2个字节 */
			buf[0] = (UINT8)iCreateDate;  /* 文件创建的日期,低8位在前 */
			buf[1] = (UINT8)(iCreateDate>>8);  /* 文件创建的日期,高8位在后 */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtDate ), 2 );  /* 向内部缓冲区偏移地址DIR_CrtDate写入2个字节 */
			s = CH376DirInfoSave( );  /* 保存文件的目录信息 */
			if ( s == USB_INT_SUCCESS ) {  /* 成功修改并保存 */
				/* 文件读写操作等... */
			}
		}
		CH376FileClose( FALSE );  /* 关闭文件 */
	}
	return( s );
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
		printf( "Wait Udisk/SD\n" );
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );  /* 延时,可选操作,有的USB存储器需要几十毫秒的延时 */

/* 对于检测到USB设备的,最多等待10*50mS */
		for ( s = 0; s < 10; s ++ ) {  /* 最长等待时间,10*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* 初始化磁盘并测试磁盘是否就绪 */
		}
//		s = CH376ReadBlock( buf );  /* 如果需要,可以读取数据块CH376_CMD_DATA.DiskMountInq,返回长度 */
//		if ( s == sizeof( INQUIRY_DATA ) ) {  /* U盘的厂商和产品信息 */
//			buf[ s ] = 0;
//			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
//		}

		printf( "Open and rename CH376HFT.C to CH376HFT.TXT \n" );
		s = RenameFileName( "/CH376HFT.C" );  /* 修改文件名, 该文件在根目录下, C文件 => TXT文件 */
		if ( s == ERR_MISS_FILE ) printf( "File /CH376HFT.C not found !\n" );
		mStopIfError( s );
		printf( "Set file create date & time to 2004.06.08 15:39:20 \n" );
		s = SetFileCreateTime( "/CH376HFT.TXT", MAKE_FILE_DATE( 2004, 6, 8 ), MAKE_FILE_TIME( 15, 39, 20 ) );  /* 为指定文件设置创建日期和时间 */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
