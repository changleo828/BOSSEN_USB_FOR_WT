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
/* 本程序用于演示创建子目录,以及打开多级目录下的文件或者目录 */

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
#define		EN_DIR_CREATE	1	/* 支持新建多级子目录 */
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

UINT8		idata	buf[64];

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

		printf( "Create Level 1 Directory /YEAR2008 \n" );
		s = CH376DirCreate( "/YEAR2008" );  /* 新建或者打开目录,该目录建在根目录下 */
		if ( s == ERR_FOUND_NAME ) printf( "Found a file with same name\n" );
		mStopIfError( s );
		CH376FileClose( FALSE );  /* 关闭目录 */

/* 下面新建二级子目录 */
		printf( "Create Level 2 Directory /YEAR2008/MONTH05 \n" );
		strcpy( buf, "/YEAR2008/MONTH05" );  /* 目录名,该目录建在YEAR2008子目录下,YEAR2008目录必须事先存在,CH376DirCreatePath的文件名缓冲区必须在RAM中 */
		s = CH376DirCreatePath( buf );  /* 新建或者打开目录,该目录建在多级目录下,输入路径必须在RAM中 */
		if ( s == ERR_FOUND_NAME ) printf( "Found a file with same name\n" );
		mStopIfError( s );
//		CH376FileClose( FALSE );  /* 暂时不关闭目录,因为下面要在这个新建或者打开的目录中新建文件 */

/* 目录新建或者打开成功,下面在这个子目录中新建一个演示文件,
   两种方法:一是CH376FileCreatePath新建多级目录下的文件,二是CH376FileCreate在当前目录下新建文件(如果前面打开了上级目录并且没有关闭) */
		printf( "Create New File /YEAR2008/MONTH05/DEMO2008.TXT \n" );

//		strcpy( buf, "/YEAR2008/MONTH05/DEMO2008.TXT" );  /* 文件名,该目录建在YEAR2008/MONTH05多级目录下,目录必须事先存在,CH376FileCreatePath的文件名缓冲区必须在RAM中 */
//		s = CH376FileCreatePath( buf );  /* 新建多级目录下的文件,如果文件已经存在则先删除后再新建,输入路径必须在RAM中 */

		s = CH376FileCreate( "DEMO2008.TXT" );  /* 在当前目录下新建文件,如果文件已经存在则先删除后再新建 */

		mStopIfError( s );

		printf( "Write some data to file\n" );
		strcpy( buf, "This is 演示数据\xd\xa" );
		s = CH376ByteWrite( buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
		mStopIfError( s );

		printf( "Close\n" );
		s = CH376FileClose( TRUE );  /* 关闭文件,自动更新文件长度 */
		mStopIfError( s );

/* 下面打开多级目录下的文件 */
		printf( "Open File /YEAR2008/MONTH05/DEMO2008.TXT \n" );
		strcpy( buf, "/YEAR2008/MONTH05/DEMO2008.TXT" );  /* CH376FileOpenPath的文件名缓冲区必须在RAM中 */
		s = CH376FileOpenPath( buf );  /* 直接打开多级目录下的文件,这是CH376FileOpenPath子程序内部由多次逐级打开组合而成 */
		mStopIfError( s );

		printf( "Close\n" );
		s = CH376FileClose( FALSE );  /* 关闭文件 */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
