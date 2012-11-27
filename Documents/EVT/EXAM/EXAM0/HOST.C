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
/* 本程序演示字节读写,文件枚举, 用于将U盘中的/C51/CH376HFT.C文件中的前200个字符显示出来,
   如果找不到原文件CH376HFT.C, 那么该程序将显示C51子目录下所有以CH376开头的文件名,
   如果找不到C51子目录, 那么该程序将显示根目录下的所有文件名 */

#include "CH376.H"

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

void	host( ) {
	UINT8	i, s;
	UINT8	TotalCount;
	UINT16	RealCount;
	P_FAT_DIR_INFO	pDir;

	s = mInitCH376Host( );  /* 初始化CH376 */
	mStopIfError( s );
/* 其它电路初始化 */

	while ( 1 ) {
		printf( "Wait Udisk/SD\n" );
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
			if ( IsKeyPress( ) ) {  /* 有键按下 */
				printf( "Exit USB host mode\n" );
				return;
			}
			mDelaymS( 100 );  /* 没必要频繁查询 */
		}
		LED_UDISK_IN( );  /* LED亮 */
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
		printf( "Open\n" );
		strcpy( buf, "\\C51\\CH376HFT.C" );  /* 源文件名,多级目录下的文件名和路径名必须复制到RAM中再处理,而根目录或者当前目录下的文件名可以在RAM或者ROM中 */
		s = CH376FileOpenPath( buf );  /* 打开文件,该文件在C51子目录下 */
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
		}
		else {  /* 找到文件或者出错 */
			mStopIfError( s );
			TotalCount = 200;  /* 准备读取总长度 */
			printf( "从文件中读出的前%d个字符是:\n",(UINT16)TotalCount );
			while ( TotalCount ) {  /* 如果文件比较大,一次读不完,可以再调用CH376ByteRead继续读取,文件指针自动向后移动 */
				if ( TotalCount > sizeof(buf) ) i = sizeof(buf);  /* 剩余数据较多,限制单次读写的长度不能超过缓冲区大小 */
				else i = TotalCount;  /* 最后剩余的字节数 */
				s = CH376ByteRead( buf, i, &RealCount );  /* 以字节为单位读取数据块,单次读写的长度不能超过缓冲区大小,第二次调用时接着刚才的向后读 */
				mStopIfError( s );
				TotalCount -= (UINT8)RealCount;  /* 计数,减去当前实际已经读出的字符数 */
				for ( s=0; s!=RealCount; s++ ) printf( "%C", buf[s] );  /* 显示读出的字符 */
				if ( RealCount < i ) {  /* 实际读出的字符数少于要求读出的字符数,说明已经到文件的结尾 */
					printf( "\n" );
					printf( "文件已经结束\n" );
					break;
				}
			}
			printf( "Close\n" );
			s = CH376FileClose( FALSE );  /* 关闭文件 */
			mStopIfError( s );
		}

UnknownUsbDevice:
		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		LED_UDISK_OUT( );  /* LED灭 */
		mDelaymS( 100 );
	}
}
