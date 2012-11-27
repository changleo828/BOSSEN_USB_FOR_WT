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
/* 本程序演示字节读写,用于将ADC模数采集的数据添加到U盘或者SD卡文件MY_ADC.TXT中,
   如果文件存在那么将数据添加到文件末尾,如果文件不存在那么新建文件后添加数据 */

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

main( ) {
	UINT8	s;
	UINT8	month, date, hour;
	UINT16	adc;

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

/* 如果MY_ADC.TXT文件已经存在则添加数据到尾部,如果不存在则新建文件 */
		printf( "Open\n" );
		s = CH376FileOpen( "/MY_ADC.TXT" );  /* 打开文件,该文件在根目录下 */
		if ( s == USB_INT_SUCCESS ) {  /* 文件存在并且已经被打开,移动文件指针到尾部以便添加数据 */
			printf( "File size = %ld\n", CH376GetFileSize( ) );  /* 读取当前文件长度 */
			printf( "Locate tail\n" );
			s = CH376ByteLocate( 0xFFFFFFFF );  /* 移到文件的尾部 */
			mStopIfError( s );
		}
		else if ( s == ERR_MISS_FILE ) {  /* 没有找到文件,必须新建文件 */
			printf( "Create\n" );
			s = CH376FileCreate( NULL );  /* 新建文件并打开,如果文件已经存在则先删除后再新建,不必再提供文件名,刚才已经提供给CH376FileOpen */
			mStopIfError( s );
		}
		else mStopIfError( s );  /* 打开文件时出错 */
		printf( "Write begin\n" );
		s = sprintf( buf, "此前文件长度= %ld 字节\xd\xa", CH376GetFileSize( ) );  /* 注意字符串长度不能溢出buf,否则加大缓冲区或者分多次写入 */
		s = CH376ByteWrite( buf, s, NULL );  /* 以字节为单位向文件写入数据 */
		mStopIfError( s );
		printf( "Write ADC data\n" );
		TR0=1;  /* 用定时器0的计数值代替ADC数据 */
		for ( hour = 8; hour != 20; hour ++  ) {  /* 用循环方式添加12行数据 */
			month = 5;  /* 假定是5月 */
			date = TL1 & 0x1F;  /* 因为测试板上没有实时时钟芯片,所以用定时器1的计数代替进行演示 */
/*			adc = get_adc_data( ); */
			adc = ( (UINT16)TH0 << 8 ) | TL0;  /* 因为测试板上没有ADC,所以用定时器0的计数代替ADC数据演示 */
			s = sprintf( buf, "%02d.%02d.%02d ADC=%u\xd\xa", (UINT16)month, date, (UINT16)hour, adc );  /* 将二制制数据格式为一行字符串 */
			s = CH376ByteWrite( buf, s, NULL );  /* 以字节为单位向文件写入数据 */
/* 有些U盘可能会要求在写数据后等待一会才能继续操作,所以,如果在某些U盘中发生数据丢失现象,建议在每次写入数据后稍作延时再继续 */
			mStopIfError( s );
			printf( "Current offset ( file point ) is %ld\n", CH376ReadVar32( VAR_CURRENT_OFFSET ) );  /* 读取当前文件指针 */
		}
//		CH376ByteWrite( buf, 0, NULL );  /* 以字节为单位向文件写入数据,因为是0字节写入,所以只用于更新文件的长度,当阶段性写入数据后,可以用这种办法更新文件长度 */
		printf( "Write end\n" );
		strcpy( buf, "今天的ADC数据到此结束\xd\xa" );
		s = CH376ByteWrite( buf, strlen( buf ), NULL );  /* 以字节为单位向文件写入数据 */
		mStopIfError( s );
/* 如果实际产品中有实时时钟,可以根据需要将文件的日期和时间修改为实际值,参考EXAM10用CH376DirInfoRead/CH376DirInfoSave实现 */
		printf( "Close\n" );
		s = CH376FileClose( TRUE );  /* 关闭文件,自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度 */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
