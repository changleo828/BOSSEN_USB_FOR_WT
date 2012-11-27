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
/* 本程序用于演示列出指定目录下的所有文件，以及用于搜索/枚举文件名 */

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

typedef struct _FILE_NAME {
	UINT32	DirStartClust;				/* 文件所在目录的起始簇号 */
//	UINT32	Size;						/* 文件长度 */
	UINT8	Name[8+1+3+1];				/* 文件名,共8+3字节,分隔符,结束符,因为未包含上级目录名所以是相对路径 */
	UINT8	Attr;						/* 文件属性 */
} FILE_NAME;
#define		MAX_FILE_COUNT		40
FILE_NAME	xdata	FileNameBuffer[ MAX_FILE_COUNT ];	/* 文件名结构 */
UINT16		FileCount;
UINT8		idata	buf[64];

/* 例子:列举指定序号的目录下的所有文件 */
UINT8	ListFile( UINT8 index )
/* 输入参数index是指目录在结构中的序号 */
{
	UINT8			s;
	P_FAT_DIR_INFO	pDir;
	PUINT8			pNameBuf;
	UINT32			CurrentDirStartClust;  /* 保存当前目录的起始簇号,用于加快文件枚举和打开速度 */
	CH376WriteVar32( VAR_START_CLUSTER, FileNameBuffer[ index ].DirStartClust );  /* 将当前目录的上级目录的起始簇号设置为当前簇号,相当于打开上级目录 */
	printf( "List Directory: %s\n", FileNameBuffer[ index ].Name );  /* 显示当前要列举的目录名 */
	s = CH376FileOpen( FileNameBuffer[ index ].Name );  /* 打开目录,仅为了获取目录的起始簇号以提高速度 */
	if ( s == USB_INT_SUCCESS ) return( ERR_FOUND_NAME );  /* 应该是打开了目录,但是返回结果是打开了文件 */
	else if ( s != ERR_OPEN_DIR ) return( s );
	if ( index ) CurrentDirStartClust = CH376ReadVar32( VAR_START_CLUSTER );  /* 不是根目录,获取目录的起始簇号 */
	else CurrentDirStartClust = 0;  /* 是根目录 */
	CH376FileClose( FALSE );  /* 对于根目录一定要关闭 */
	CH376WriteVar32( VAR_START_CLUSTER, CurrentDirStartClust );  /* 当前目录的起始簇号,相当于打开当前目录 */
	CH376SetFileName( "*" );  /* 设置将要操作的文件的文件名,通配符支持所有文件和子目录 */
	xWriteCH376Cmd( CMD0H_FILE_OPEN );  /* 枚举文件和目录 */
	xEndCH376Cmd( );
	while ( 1 ) {
		s = Wait376Interrupt( );
		if ( s == USB_INT_DISK_READ ) {  /* 请求数据读出 */
/* 在文件枚举过程中,不能执行其它可能产生中断的操作命令,例如,如果需要获取长文件名,那么可以将枚举出的文件名保存并在枚举结束后获取其长文件名 */
			CH376ReadBlock( buf );  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
			xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录,先发出下一个命令再分析上行读出的数据可以让CH376与单片机分别同时工作,提高速度 */
			xEndCH376Cmd( );
			pDir = (P_FAT_DIR_INFO)buf;  /* 当前文件目录信息 */
			if ( pDir -> DIR_Name[0] != '.' ) {  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
				if ( pDir -> DIR_Name[0] == 0x05 ) pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
				if ( pDir -> DIR_Name[8] == 'H' && pDir -> DIR_Name[9] == ' '  /* 比较文件扩展名分析文件类型的范例 */
					|| pDir -> DIR_Name[8] == 'E' && pDir -> DIR_Name[9] == 'X' && pDir -> DIR_Name[10] == 'E' ) {
					printf( "This is a .H or .EXE file\n" );
				}
				if ( FileCount < MAX_FILE_COUNT ) {  /* 文件名结构缓冲区足够 */
					pNameBuf = & FileNameBuffer[ FileCount ].Name;  /* 文件名结构中的文件名缓冲区 */
					for ( s = 0; s < 11; s ++ ) {  /* 复制文件名,长度为11个字符 */
						if ( pDir -> DIR_Name[ s ] != 0x20 ) {  /* 有效字符 */
							if ( s == 8 ) {  /* 处理扩展名 */
								*pNameBuf = '.';  /* 分隔符 */
								pNameBuf ++;
							}
							*pNameBuf = pDir -> DIR_Name[ s ];  /* 复制文件名的一个字符 */
							pNameBuf ++;
						}
					}
					*pNameBuf = 0;  /* 当前文件名完整路径的结束符 */
					FileNameBuffer[ FileCount ].DirStartClust = CurrentDirStartClust;  /* 记录当前目录的起始簇号,用于加快文件打开速度 */
					FileNameBuffer[ FileCount ].Attr = pDir -> DIR_Attr;  /* 记录文件属性 */
					if ( pDir -> DIR_Attr & ATTR_DIRECTORY ) printf( "Dir %4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );  /* 判断是目录名 */
					else printf( "File%4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );  /* 判断是文件名 */
					FileCount ++;  /* 子目录计数 */
				}
				else {  /* 文件名结构缓冲区太小,结构数量不足 */
					printf( "FileName Structure Full\n" );
					s = Wait376Interrupt( );
					CH376EndDirInfo( );  /* 获取完FAT_DIR_INFO结构 */
					break;  /* 强行终止枚举 */
				}
			}
		}
		else {
			if ( s == ERR_MISS_FILE ) s = USB_INT_SUCCESS;  /* 没有找到更多的匹配文件 */
			break;
		}
	}
/*	if ( s == USB_INT_SUCCESS ) return( s );*/  /* 操作成功 */
	return( s );
}

UINT8	ListAll( void )  /* 以广度优先的算法枚举整个U盘中的所有文件及目录 */
{
	UINT8	s;
	UINT16	OldFileCount;
	UINT16	RealReadCount;
	FileNameBuffer[ 0 ].Name[0] = '/';  /* 根目录,是完整路径名,除根目录是绝对路径之外都是相对路径 */
	FileNameBuffer[ 0 ].Name[1] = 0;
	FileNameBuffer[ 0 ].DirStartClust = 0;  /* 根目录的起始簇号 */
	FileNameBuffer[ 0 ].Attr = ATTR_DIRECTORY;  /* 根目录也是目录,作为第一个记录保存 */
	for ( OldFileCount = 0, FileCount = 1; OldFileCount < FileCount; OldFileCount ++ ) {  /* 尚有新枚举到的文件名结构未进行分析,FileCount处于变化之中 */
		if ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) {  /* 是目录则继续进行深度搜索 */
			s = ListFile( OldFileCount );  /* 枚举目录,记录保存到结构中,FileCount可能会改变 */
			if ( s != USB_INT_SUCCESS ) return( s );
		}
	}

/* U盘中的文件及目录全部枚举完毕,下面开始根据结构记录依次打开文件 */
	printf( "Total file&dir = %d, Open every file:\n", FileCount );
	for ( OldFileCount = 0; OldFileCount < FileCount; OldFileCount ++ ) {
		if ( ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) == 0 ) {  /* 是文件则打开,目录则跳过 */
			printf( "Open file: %s\n", FileNameBuffer[ OldFileCount ].Name );
			CH376WriteVar32( VAR_START_CLUSTER, FileNameBuffer[ OldFileCount ].DirStartClust );  /* 将当前文件所在上级目录的起始簇号设置为当前簇号,相当于打开上级目录 */
			s = CH376FileOpen( FileNameBuffer[ OldFileCount ].Name );  /* 打开文件 */
			if ( s == USB_INT_SUCCESS ) {  /* 成功打开了文件 */
				s = CH376ByteRead( buf, sizeof(buf), &RealReadCount );  /* 以字节为单位从当前位置读取数据块 */
				if ( RealReadCount != sizeof(buf) ) printf( "File size = %d\n", RealReadCount );  /* 文件长度不足sizeof(buf) */
				CH376FileClose( FALSE );  /* 不做写操作可以无需关闭 */
			}
		}
	}
}

main( ) {
	UINT8	i, s;
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

/* 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的 */
		for ( i = 0; i < 100; i ++ ) {  /* 最长等待时间,100*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			s = CH376DiskMount( );  /* 初始化磁盘并测试磁盘是否就绪 */
			if ( s == USB_INT_SUCCESS ) break;  /* 准备好 */
			else if ( s == ERR_DISK_DISCON ) break;  /* 检测到断开,重新检测并计时 */
			if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) break;  /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
		}
		if ( s == ERR_DISK_DISCON ) {  /* 检测到断开,重新检测并计时 */
			printf( "Device gone\n" );
			continue;
		}
		if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) {  /* 未知USB设备,例如USB键盘、打印机等 */
			printf( "Unknown device\n" );
			goto UnknownUsbDevice;
		}
		i = CH376ReadBlock( buf );  /* 如果需要,可以读取数据块CH376_CMD_DATA.DiskMountInq,返回长度 */
		if ( i == sizeof( INQUIRY_DATA ) ) {  /* U盘的厂商和产品信息 */
			buf[ i ] = 0;
			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		}
		mDelaymS( 20 );

		printf( "List all file \n" );
		s = ListAll( );  /* 枚举整个U盘中的所有文件及目录 */
		mStopIfError( s );

UnknownUsbDevice:
		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
