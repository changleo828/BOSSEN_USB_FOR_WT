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
/* 本程序演示字节读写,文件枚举,文件复制, 用于将U盘中的/C51/CH376HFT.C文件中的小写字母转成大写字母后, 写到新建的文件NEWFILE.TXT中,
   如果找不到原文件CH376HFT.C, 那么该程序将显示C51子目录下所有以CH376开头的文件名, 并新建NEWFILE.TXT文件并写入提示信息,
   如果找不到C51子目录, 那么该程序将显示根目录下的所有文件名, 并新建NEWFILE.TXT文件并写入提示信息 */

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
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

UINT8		idata	buf[64];

UINT8	CopyAndConvertFile( PUINT8 SrcFileName, PUINT8 TarFileName ) {  /* 文件复制,以字节方式复制,缓冲区越大速度越快 */
/* SrcFileName 源文件名,支持路径分隔符和多级目录,字符串必须存放于RAM中
   TarFileName 目标文件名,支持路径分隔符和多级目录,字符串必须存放于RAM中 */
	UINT8	s;
	UINT16	ThisLen, cnt;
	UINT32	FileSize, ByteCount;
	UINT8	TarName;
	UINT32	TarUpDirClust;
	ByteCount = 0;
	do {
		printf( "OpenSrc\n" );  /* 注意打印输出会浪费时间,减低平均复制速度 */
		s = CH376FileOpenPath( SrcFileName );  /* 打开多级目录下的文件,输入缓冲区必须在RAM中 */
		if ( s != USB_INT_SUCCESS ) return( s );
		if ( ByteCount == 0 ) {  /* 首次 */
			FileSize = CH376GetFileSize( );  /* 读取当前文件长度 */
			printf( "SrcFileSz=%ld\n", FileSize );
		}
		else {  /* 再次进入 */
			s = CH376ByteLocate( ByteCount );  /* 以字节为单位移动当前文件指针到上次复制结束位置 */
			if ( s != USB_INT_SUCCESS ) return( s );
		}
		printf( "Read\n" );
		s = CH376ByteRead( buf, sizeof( buf ), &ThisLen );  /* 以字节为单位从当前位置读取数据块,请求长度同缓冲区大小,返回实际长度在ThisLen中 */
		if ( s != USB_INT_SUCCESS ) return( s );
//		s = CH376FileClose( FALSE );  /* 关闭文件,对于读操作可以不必关闭文件 */
//		if ( s != USB_INT_SUCCESS ) return( s );

		for ( cnt=0; cnt < ThisLen; cnt ++ ) {  /* 将缓冲区中的小写字符转换为大写 */
			s = buf[ cnt ];
			if ( s >= 'a' && s <= 'z' ) buf[ cnt ] = s - ( 'a' - 'A' );
		}

		if ( ByteCount == 0 ) {  /* 首次,目标文件尚未存在 */
			printf( "CreateTar\n" );
			TarName = CH376SeparatePath( TarFileName );  /* 从路径中分离出最后一级文件名或目录名,返回最后一级文件名或目录名的偏移 */
			if ( TarName ) {  /* 是多级目录 */
				s = CH376FileOpenDir( TarFileName, TarName );  /* 打开多级目录下的最后一级目录,即打开新建文件的上级目录 */
				if ( s != ERR_OPEN_DIR ) {  /* 因为是打开上级目录,所以,如果不是成功打开了目录,那么说明有问题 */
					if ( s == USB_INT_SUCCESS ) return( ERR_FOUND_NAME );  /* 中间路径必须是目录名,如果是文件名则出错 */
					else if ( s == ERR_MISS_FILE ) return( ERR_MISS_DIR );  /* 中间路径的某个子目录没有找到,可能是目录名称错误 */
					else return( s );  /* 操作出错 */
				}
				TarUpDirClust = CH376ReadVar32( VAR_START_CLUSTER );  /* 上级目录的起始簇号 */
			}
			else TarUpDirClust = 0;  /* 默认是根目录的起始簇号 */
/* 在当前目录下进行文件新建或者打开操作,比全路径多级目录下的文件新建或者打开操作的速度快,
   所以目标文件的新建或者打开采用此法处理(本程序源文件是直接打开全路径多级目录下的文件,为提高速度,也可参照此法加快文件打开),
   为了实现当前目录下的文件新建或者打开操作,参考上面几行代码,
   首先,要获得文件所在的上级目录的起始簇号,相当于打开上级目录,通过CH376FileOpenPath打开上级目录获得,
   其次,要获得文件的直接短文件名(去掉上级目录名,不含任何路径分隔符,保留最后一级文件名),通过CH376SeparatePath分析目标文件名获得 */
			s = CH376FileCreate( &TarFileName[TarName] );  /* 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除 */
			if ( s != USB_INT_SUCCESS ) return( s );
		}
		else {  /* 再次进入,目标文件已存在 */
			printf( "OpenTar\n" );
			CH376WriteVar32( VAR_START_CLUSTER, TarUpDirClust );  /* 将目标文件所在的上级目录的起始簇号设置为当前簇号,相当于打开上级目录 */
			s = CH376FileOpen( &TarFileName[TarName] );  /* 打开文件 */
			if ( s != USB_INT_SUCCESS ) return( s );
			s = CH376ByteLocate( ByteCount );  /* 以字节为单位移动当前文件指针到上次复制结束位置 */
			if ( s != USB_INT_SUCCESS ) return( s );
		}
		printf( "Write\n" );
		s = CH376ByteWrite( buf, ThisLen, NULL );  /* 以字节为单位向当前位置写入数据块,除非没有磁盘空间,否则返回实际长度总是与ThisLen相等 */
		if ( s != USB_INT_SUCCESS ) return( s );
		printf( "CloseTar\n" );
		s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
		if ( s != USB_INT_SUCCESS ) return( s );
		ByteCount += ThisLen;
		if ( ThisLen < sizeof( buf ) ) {  /* 实际读出字节数小于请求读出字节数,说明原文件结束 */
			if ( ByteCount != FileSize ) printf( "Error on SourceFile reading" );
			break;
		}
	} while( ByteCount < FileSize );
	return( USB_INT_SUCCESS );
}

main( ) {
	UINT8			i, s;
	P_FAT_DIR_INFO	pDir;
	UINT8	xdata	SrcName[64];
	UINT8	xdata	TarName[64];
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

/* 读取原文件 */
		strcpy( SrcName, "\\C51\\CH376HFT.C" );  /* 源文件名,多级目录下的文件名和路径名必须复制到RAM中再处理,而根目录或者当前目录下的文件名可以在RAM或者ROM中 */
		strcpy( TarName, "\\NEWFILE.TXT" );  /* 目标文件名 */
		printf( "Open\n" );
		s = CH376FileOpenPath( SrcName );  /* 打开文件,该文件在C51子目录下 */
		if ( s == ERR_MISS_DIR || s == ERR_MISS_FILE ) {  /* 没有找到目录或者没有找到文件 */
/* 列出文件,完整枚举可以参考EXAM13全盘枚举 */
			if ( s == ERR_MISS_DIR ) strcpy( buf, "\\*" );  /* C51子目录不存在则列出根目录下的文件 */
			else strcpy( buf, "\\C51\\CH376*" );  /* CH376HFT.C文件不存在则列出\C51子目录下的以CH376开头的文件 */
			printf( "List file %s\n", buf );
			s = CH376FileOpenPath( buf );  /* 枚举多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
			while ( s == USB_INT_DISK_READ ) {  /* 枚举到匹配的文件 */
				CH376ReadBlock( buf );  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
				pDir = (P_FAT_DIR_INFO)buf;  /* 当前文件目录信息 */
				if ( pDir -> DIR_Name[0] != '.' ) {  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
					if ( pDir -> DIR_Name[0] == 0x05 ) pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
					pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
					printf( "*** EnumName: %s\n", pDir -> DIR_Name );  /* 打印名称,原始8+3格式,未整理成含小数点分隔符 */
				}
				xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
				xEndCH376Cmd( );
				s = Wait376Interrupt( );
			}
			if ( s != ERR_MISS_FILE ) mStopIfError( s );  /* 操作出错 */
			printf( "Create\n" );
			s = CH376FileCreatePath( TarName );  /* 新建多级目录下的文件,支持多级目录路径,输入缓冲区必须在RAM中 */
			mStopIfError( s );
			printf( "Write\n" );
			strcpy( buf, "找不到/C51/CH376HFT.C文件\xd\n" );
			s = CH376ByteWrite( buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
			mStopIfError( s );
			printf( "Close\n" );
			s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
			mStopIfError( s );
		}
		else {  /* 找到文件或者出错 */
			mStopIfError( s );
			s = CopyAndConvertFile( SrcName, TarName );  /* 文件复制 */
			mStopIfError( s );
		}

/* 删除某文件 */
/*		printf( "Erase\n" );
		s = CH376FileErase( "/OLD" );  删除文件
		if ( s != USB_INT_SUCCESS ) printf( "Error: %02X\n", (UINT16)s );  显示错误
*/

/* 检查U盘或者SD卡的剩余空间 */
/*		printf( "DiskQuery: " );
		s = CH376DiskQuery( (PUINT32)buf );  查询磁盘剩余空间信息,扇区数
		mStopIfError( s );
//		printf( "free cap = %ld MB\n", *(PUINT32)buf * DEF_SECTOR_SIZE / 1000000 );  未用扇区数乘以扇区大小后,除以1M换算为兆字节单位
		printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );  上面一行的计算可能会溢出,换个顺序计算 */

UnknownUsbDevice:
		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}
