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
/* 本程序演示扇区读写,修改文件长度,查询磁盘剩余空间,
   用于将ADC模数采集的数据添加到U盘文件MY_ADC.TXT中,为提高速度而以扇区为单位读写U盘文件,不支持SD卡,
   本范例演示在扇区模式下处理零碎数据,同时兼顾操作方便和较高速度,需要单片机系统提供足够的RAM作为文件数据缓冲区FILE_DATA_BUF */

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
#define		EN_DISK_QUERY		1	/* 启用磁盘查询 */
#define		EN_SECTOR_ACCESS	1	/* 启用扇区读写 */
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

UINT8		idata	buf[64];

UINT16	total;		/* 记录当前缓冲在FILE_DATA_BUF中的数据长度 */
UINT32	NewSize;	/* 临时变量 */
UINT8	xdata	FileDataBuf[0x4000];	/* 文件缓冲区,太小则速度慢 */

/* 将准备写入U盘的零碎数据进行集中缓冲,组合成大数据块时再通过CH376真正写入U盘 */
/* 这样做的好处是: 提高速度(因为大数据块写入时效率高), 减少U盘损耗(U盘内部的内存寿命有限,不宜频繁擦写) */
void	mFlushBufferToDisk( UINT8 force )
/* force = 0 则自动刷新(检查缓冲区中的数据长度,满则写盘,不满则暂时放在缓冲区中), force != 0 则强制刷新(不管缓冲区中的数据有多少都写盘,通常在系统关机前应该强制写盘) */
{
	UINT8	s;
	if ( force ) {  /* 强制刷新 */
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;  /* 将缓冲区中的字节数转换为扇区数(除以DEF_SECTOR_SIZE),长度加上DEF_SECTOR_SIZE-1用于确保写入最后的零头数据 */
		if ( s ) {  /* 有数据 */
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* 以扇区为单位向文件写入数据,写入缓冲区中的所有数据,含最后的零头 */
			mStopIfError( s );
/* 有些U盘可能会要求在写数据后等待一会才能继续操作,所以,如果在某些U盘中发生数据丢失现象,建议在每次写入数据后稍作延时再继续 */
			mDelaymS( 1 );  /* 写后延时,可选的,大多数U盘不需要 */
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* 将刚才已写入U盘的零头数据复制到缓冲区的头部 */
			total &= DEF_SECTOR_SIZE - 1;  /* 缓冲区中只剩下刚才已写入U盘的零头数据,继续保留在缓冲区中是为了方便以后在其后面追加数据 */
		}
		NewSize = CH376GetFileSize( );  /* 读取当前文件长度,如果没有零头数据,那么文件长度是DEF_SECTOR_SIZE的倍数 */
		if ( total ) NewSize -= DEF_SECTOR_SIZE - total;  /* 以扇区为单位,有零头数据,计算出真正的文件长度(有效数据的长度) */
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* 将正确的当前文件长度写入CH376内存 */
		printf( "Current file size is %ld\n", NewSize );
		s = CH376SecWrite( FileDataBuf, 0, NULL );  /* 写0长度,实际是刷新文件长度,将CH376内存中当前文件长度信息真正写入U盘或者SD卡中 */
		mStopIfError( s );
		s = CH376SecLocate( 0xFFFFFFFF );  /* 重新回到原文件的尾部,以扇区为单位,所以会忽略文件尾部的零头数据,下面如果再写入数据将覆盖尾部零头数据,不过该零头数据有一份副本保留在缓冲区中,所以请放心 */
		mStopIfError( s );
	}
	else if ( total >= sizeof( FileDataBuf ) - DEF_SECTOR_SIZE ) {  /* 缓冲区中的数据快要满了,所以应该先将缓冲区中的原有数据写入U盘 */
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );  /* 将缓冲区中的字节数转换为扇区数(除以DEF_SECTOR_SIZE),最后的零头数据先不管,以扇区为单位向文件写入数据 */
		mStopIfError( s );
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* 将刚才未写入U盘的零头数据复制到缓冲区的头部 */
		total &= DEF_SECTOR_SIZE - 1;  /* 缓冲区中只剩下刚才未写入U盘的零头数据 */
/* 注意,此处U盘或者SD卡中的文件长度仍然是以前的值,即此时突然断电,那么虽然物理上的数据已在U盘/SD卡中,但文件长度没有包括这些数据,导致常归应用程序无法取得数据 */
	}
}

main( ) {
	UINT8	s;
	UINT8	month, date, hour;
	UINT16	year, adc;

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

/* 检查U盘或者SD卡的剩余空间 */
		printf( "DiskQuery: " );
		s = CH376DiskQuery( (PUINT32)buf );  /* 查询磁盘剩余空间信息,扇区数 */
		mStopIfError( s );
//		printf( "free cap = %ld MB\n", *(PUINT32)buf * DEF_SECTOR_SIZE / 1000000 );  /* 未用扇区数乘以扇区大小后,除以1M换算为兆字节单位 */
		printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );  /* 上面一行的计算可能会溢出,换个顺序计算 */

/* 如果MY_ADC.TXT文件已经存在则添加数据到尾部,如果不存在则新建文件 */
		printf( "Open\n" );
		s = CH376FileOpen( "/MY_ADC.TXT" );  /* 打开文件,该文件在根目录下 */
		if ( s == USB_INT_SUCCESS ) {  /* 文件存在并且已经被打开,移动文件指针到尾部以便添加数据 */
			NewSize = CH376GetFileSize( );  /* 读取当前文件长度,如果没有零头数据,那么文件长度是DEF_SECTOR_SIZE的倍数 */
			printf( "File size = %ld\n", NewSize );
			printf( "Locate tail\n" );
			s = CH376SecLocate( 0xFFFFFFFF );  /* 以扇区为单位移到文件尾部,如果文件尾部有不足一个扇区的零头数据则被忽略,如果不做处理那么零头数据将可能被写入数据覆盖 */
			mStopIfError( s );
			total = NewSize & ( DEF_SECTOR_SIZE - 1 );  /* 上次保存文件时如果尾部有零头数据,那么先取得零头字节数,不满扇区长度的零碎数据 */
			printf( "Read last tail = %d Bytes\n", total );
			if ( total ) {  /* 原尾部有零头数据,那么先调入内存,以便接着添加数据 */
				s = CH376SecRead( FileDataBuf, 1, NULL );  /* 从文件读取尾部零头数据 */
				mStopIfError( s );
				s = CH376SecLocate( 0xFFFFFFFF );  /* 重新回到原文件的尾部,下面如果写入数据将覆盖原尾部零头数据,不过原零头数据刚才已经被读入内存,所以请放心 */
				mStopIfError( s );
			}
		}
		else if ( s == ERR_MISS_FILE ) {  /* 没有找到文件,必须新建文件 */
			printf( "Create\n" );
			s = CH376FileCreate( NULL );  /* 新建文件并打开,如果文件已经存在则先删除后再新建,不必再提供文件名,刚才已经提供给CH376FileOpen */
			mStopIfError( s );
			total = 0;  /* 此前没有零头数据 */
			NewSize = 0;
		}
		else mStopIfError( s );  /* 打开文件时出错 */
		printf( "Write begin\n" );
		total += sprintf( FileDataBuf + total, "在本次添加数据之前,该文件已有数据的长度是 %ld 字节\xd\xa", NewSize );  /* 将新数据添加到缓冲区的尾部,累计缓冲区内的数据长度 */
		mFlushBufferToDisk( 0 );  /* 自动刷新缓冲区,检查缓冲区是否已满,满则写盘 */
		printf( "Write ADC data\n" );
		TR0=1;  /* 用定时器0的计数值代替ADC数据 */
		for ( month = 1; month != 12; month ++ ) {  /* 因为测试板上没有实时时钟芯片,所以用循环方式模拟月份 */
			for ( date = 1; date != 30; date ++ ) {  /* 因为测试板上没有实时时钟芯片,所以用循环方式模拟日期 */
				year = 2004;  /* 假定为2004年 */
				hour = TL1 & 0x1F;  /* 因为测试板上没有实时时钟芯片,所以用定时器1的计数代替进行演示 */
/*				adc = get_adc_data( ); */
				adc = ( (UINT16)TH0 << 8 ) | TL0;  /* 因为测试板上没有ADC,所以用定时器0的计数代替ADC数据演示 */
				total += sprintf( FileDataBuf + total, "Year=%04d, Month=%02d, Date=%02d, Hour=%02d, ADC_data=%u\xd\xa", year, (UINT16)month, (UINT16)date, (UINT16)hour, adc );  /* 将二制制数据格式为一行字符串 */
				if ( month == 6 && ( date & 0x0F ) == 0 ) mFlushBufferToDisk( 1 );  /* 强制刷新缓冲区,定期强制刷新缓冲区,这样在突然断电后可以减少数据丢失 */
				else mFlushBufferToDisk( 0 );  /* 自动刷新缓冲区,检查缓冲区是否已满,满则写盘 */
				printf( "Current total is %d\n", total );  /* 用于监控检查 */
			}
		}
		printf( "Write end\n" );
		total += sprintf( FileDataBuf + total, " ********************************* " );  /* 将新数据添加到缓冲区的尾部,累计缓冲区内的数据长度 */
		total += sprintf( FileDataBuf + total, "这次的ADC数据到此结束,程序即将退出\xd\xa" );  /* 将新数据添加到缓冲区的尾部,累计缓冲区内的数据长度 */
		mFlushBufferToDisk( 1 );  /* 强制刷新缓冲区,因为系统要退出了,所以必须强制刷新 */
		printf( "Close\n" );
		s = CH376FileClose( FALSE );  /* 关闭文件,扇区模式通常不自动更新文件长度,因为强制刷新缓冲区时已经更新了文件长度,所以这里不需要自动更新文件长度 */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
