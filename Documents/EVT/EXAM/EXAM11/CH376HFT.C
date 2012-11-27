/* 2008.10.18
****************************************
**  Copyright  (C)  W.ch  1999-2009   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Interface for CH376 **
**  TC2.0@PC, KC7.0@MCS51			 **
****************************************
*/
/* CH376 主机文件系统接口 */

/* MCS-51单片机C语言的U盘文件读写示例程序 */
/* 本程序是长文件名操作示例, 包括创建长文件名和从短文件名获得对应的长文件名 */

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
#define		EN_LONG_NAME		1	/* 支持长文件名 */
#include "..\FILE_SYS.H"

UINT8		idata	buf[64];
#define		GlobalBuf	buf		/* 长文件名子程序需要提供全局缓冲区,长度不小于64字节 */

#include "..\FILE_SYS.C"


//#define		MAX_PATH_LEN		80
//UINT8	xdata	PathNameBuf[ MAX_PATH_LEN ];		/* 存放全路径的短文件名 */
//#define		LONG_NAME_BUF_LEN	( LONG_NAME_PER_DIR * 20 )	/* 自行定义的长文件名缓冲区长度,最小值为LONG_NAME_PER_DIR*1 */
UINT8	xdata	LongNameBuf[ LONG_NAME_BUF_LEN ];	/* 存放长文件名 */

/* 长文件名示例1, 其中UNICODE是用小端编码(注:MCS51是大端格式的单片机,即每两个字节的前后两字节互换), 以下是LongName1里编码内容(中文长文件名):
建立长文件名，输入两个参数： 1.采用(unicode 大端)，字符串末尾用两个0表示结束;2.ANSI编码短文件名.TXT */
UINT8	code	LongName1[] =
{
	0xFA, 0x5E, 0xCB, 0x7A, 0x7F, 0x95, 0x87, 0x65, 0xF6, 0x4E, 0x0D, 0x54, 0x0C, 0xFF, 0x93, 0x8F,
	0x65, 0x51, 0x24, 0x4E, 0x2A, 0x4E, 0xC2, 0x53, 0x70, 0x65, 0x1A, 0xFF, 0x20, 0x00, 0x31, 0x00,
	0x2E, 0x00, 0xC7, 0x91, 0x28, 0x75, 0x28, 0x00, 0x75, 0x00, 0x6E, 0x00, 0x69, 0x00, 0x63, 0x00,
	0x6F, 0x00, 0x64, 0x00, 0x65, 0x00, 0x20, 0x00, 0x27, 0x59, 0xEF, 0x7A, 0x29, 0x00, 0x0C, 0xFF,
	0x57, 0x5B, 0x26, 0x7B, 0x32, 0x4E, 0x2B, 0x67, 0x3E, 0x5C, 0x28, 0x75, 0x24, 0x4E, 0x2A, 0x4E,
	0x30, 0x00, 0x68, 0x88, 0x3A, 0x79, 0xD3, 0x7E, 0x5F, 0x67, 0x3B, 0x00, 0x32, 0x00, 0x2E, 0x00,
	0x41, 0x00, 0x4E, 0x00, 0x53, 0x00, 0x49, 0x00, 0x16, 0x7F, 0x01, 0x78, 0xED, 0x77, 0x87, 0x65,
	0xF6, 0x4E, 0x0D, 0x54, 0x2E, 0x00, 0x54, 0x00, 0x58, 0x00, 0x54, 0x00
};

/* 长文件名示例2, 此处先只用普通单字节英文字符表示, 在主程序中会先将其转换为两字节的小端UNICODE编码, 然后再以UNICODE编码作为真正的长文件名 */
UINT8	code	LongName2[] =
{
	"This is Long Name File !!!.H"
};

main( )
{
	UINT8	s;
	UINT16	j;

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

/*==================== 以下演示创建及读取长文件名,创建两个(中英文各一个),然后读取这两个 ============================*/
		printf( "Create file with Chinese long name\n" );
		memcpy( LongNameBuf, LongName1, sizeof(LongName1) );  // 复制长文件名到LongNameBuf长文件名缓冲区
		LongNameBuf[sizeof(LongName1)] = 0x00;  // 末尾用两个0表示结束
		LongNameBuf[sizeof(LongName1)+1] = 0x00;
//		strcpy( PathNameBuf, "/C51/长文件名.TXT" );  // 复制该长文件名的ANSI编码的全路径的短文件名(8+3格式),短文件名由长文件名生成
/* 建立长文件名, 输入两个参数: 1.ANSI编码的全路径的短文件名; 2.以两个0表示结束的unicode字符的长文件名 */
		s = CH376CreateLongName( "/C51/长文件名.TXT", LongNameBuf );  /* 新建具有长文件名的文件 */
		if ( s == USB_INT_SUCCESS ) printf( "Created Chinese Long Name OK!!\n" );
		else if ( s == ERR_NAME_EXIST ) printf( "The short name already exist !\n" );
		else printf( "Error Code: %02X\n", (UINT16)s );

		printf( "Create file with English long name\n" );
		for ( j = 0; LongName2[j] != 0; j++ ) {  /* 复制并转换长文件名到LongNameBuf长文件名缓冲区 */
			LongNameBuf[j*2] = LongName2[j];  /* 将英文字符转换为两字节的小端UNICODE编码 */
			LongNameBuf[j*2+1] = 0x00;
		}
		LongNameBuf[j*2] = 0x00;  // 末尾用两个0表示结束
		LongNameBuf[j*2+1] = 0x00;
//		strcpy( PathNameBuf, "/LONGNAME.H" );  // 复制该长文件名的ANSI编码的全路径的短文件名(8+3格式),短文件名由长文件名生成(只要不冲突,其实随便定)
		s = CH376CreateLongName( "/LONGNAME.H", LongNameBuf );  /* 新建具有长文件名的文件 */
		if ( s == USB_INT_SUCCESS ) printf( "Created English Long Name OK!!\n" );
		else if ( s == ERR_NAME_EXIST ) printf( "The short name already exist !\n" );
		else printf( "Error Code: %02X\n", (UINT16)s );

		printf( "Get Chinese long name from short name\n" );
//		strcpy( PathNameBuf, "/C51/长文件名.TXT" );  // 短文件名的完整路径
		s = CH376GetLongName( "/C51/长文件名.TXT", LongNameBuf );  /* 由短文件名或者目录(文件夹)名获得相应的长文件名 */
		if ( s == USB_INT_SUCCESS ) {
			printf( "LongNameBuf: " );  // 以下显示缓冲区里的长文件名,长文件名以UNICODE小端编码方式存放,两个0表示结束
			for ( j=0; j<LONG_NAME_BUF_LEN; j+=2 ) {
				printf( "%02X-%02X,", (UINT16)(LongNameBuf[j]), (UINT16)(LongNameBuf[j+1]) );  /* UNICODE编码不方便直接显示,先按十六进制数据打印输出 */
				if ( *(PUINT16)(&LongNameBuf[j]) == 0 ) break;  /* 结束 */
			}
			printf( "END\n" );
		}
		else printf( "Error Code: %02X\n", (UINT16)s );

		printf( "Get English long name from short name\n" );
//		strcpy( PathNameBuf, "/LONGNAME.H" );  // 短文件名的完整路径
		s = CH376GetLongName( "/LONGNAME.H", LongNameBuf );  /* 由短文件名或者目录(文件夹)名获得相应的长文件名 */
		if ( s == USB_INT_SUCCESS ) {
			printf( "LongNameBuf: " );  // 以下显示缓冲区里的长文件名,长文件名以UNICODE小端编码方式存放,两个0表示结束
			for ( j=0; j<LONG_NAME_BUF_LEN; j+=2 ) {
				printf( "%02X-%02X,", (UINT16)(LongNameBuf[j]), (UINT16)(LongNameBuf[j+1]) );  /* UNICODE编码不方便直接显示,先按十六进制数据打印输出 */
				if ( *(PUINT16)(&LongNameBuf[j]) == 0 ) break;  /* 结束 */
			}
			printf( "END\n" );
			printf( "LongName is: " );  // 如果是英文字符的UNICODE编码,那么可以直接打印输出
			for ( j=0; j<LONG_NAME_BUF_LEN; j+=2 ) {  /* */
				printf( "%c", LongNameBuf[j] );  /* 英文UNICODE字符可以打印输出 */
				if ( *(PUINT16)(&LongNameBuf[j]) == 0 ) break;  /* 结束 */
			}
			printf( "\n" );
		}
		else printf( "Error Code: %02X\n", (UINT16)s );
/*==============================================================================*/

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
