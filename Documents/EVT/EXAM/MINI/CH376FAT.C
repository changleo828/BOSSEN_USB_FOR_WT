/* 该程序用80行C代码就能够读取FAT32/FAT16/FAT12文件系统U盘或者SD卡的根目录,可以看到根目录下的文件名,该程序没有任何错误处理,仅作为实验参考 */
/* 单片机读写U盘的程序分为4层: 硬件USB接口层, BulkOnly传输协议层, RBC/SCSI命令层, FAT文件系统层 */

#include <stdio.h>
#include "..\CH376INC.H"		/* 定义CH376命令代码及返回状态 */
#include <reg52.h>			/* 以下定义适用于MCS-51单片机,其它单片机参照修改 */
UINT8V	xdata	CH376_CMD_PORT _at_ 0xBDF1;	/* CH376命令端口的I/O地址 */
UINT8V	xdata	CH376_DAT_PORT _at_ 0xBCF0;	/* CH376数据端口的I/O地址 */

/* ********** 硬件USB接口层,无论如何这层省不掉,单片机总要与CH376接口吧 ************************************************************ */
void	mDelaymS( UINT8 delay ) {		/* 以毫秒为单位延时,不精确,适用于24MHz时钟MCS51 */
	UINT8	i, j, c;
	for ( i = delay; i != 0; i -- ) {
		for ( j = 200; j != 0; j -- ) c += 3;  /* 在24MHz时钟下延时500uS */
		for ( j = 200; j != 0; j -- ) c += 3;  /* 在24MHz时钟下延时500uS */
	}
}
void	xWriteCH376Cmd( UINT8 cmd ) {	/* 向CH376的命令端口写入命令,周期不小于2uS,如果单片机较快则延时 */
	CH376_CMD_PORT=cmd;
	for ( cmd = 2; cmd != 0; cmd -- );	/* 发出命令码前后应该各延时1.5uS,对于MCS51可以不需要延时 */
}
#define	xWriteCH376Data( d )	{ CH376_DAT_PORT=d; }	/* 向CH376的数据端口写入数据,周期不小于0.6uS,如果单片机较快则延时,MCS51单片机较慢所以无需延时 */
#define	xReadCH376Data( )		( CH376_DAT_PORT )		/* 从CH376的数据端口读出数据,周期不小于0.6uS,如果单片机较快则延时,MCS51单片机较慢所以无需延时 */
UINT8 mWaitInterrupt( void ) {	/* 等待CH376中断并获取状态,主机端等待操作完成,返回操作状态 */
	while( CH376_CMD_PORT & PARA_STATE_INTB );  /* 查询等待CH376操作完成中断(INT#低电平) */
	xWriteCH376Cmd( CMD_GET_STATUS );  /* 产生操作完成中断,获取中断状态 */
	return( xReadCH376Data( ) );
}
/* ********** BulkOnly传输协议层,被CH376内置了,无需编写单片机程序 ************************************************************ */
/* ********** RBC/SCSI命令层,被CH376内置了,无需编写单片机程序 **************************************************************** */
/* ********** FAT文件系统层,虽然被CH376内置了,但是要写程序发出命令及收发数据 ************************************************** */
void	mInitSTDIO( void ) {	/* 仅用于调试用途及显示内容到PC机,与该程序功能完全无关,为printf和getkey输入输出初始化串口 */
	SCON = 0x50; PCON = 0x80; TMOD = 0x20; TH1 = 24000000/32/9600; TR1 = 1; TI = 1;  /* 24MHz晶振, 9600bps */
}
void	mStopIfError( UINT8 iErrCode ) {	/* 如果错误则停止运行并显示错误状态,正式应用还需要分析处理 */
	if ( iErrCode == USB_INT_SUCCESS ) return;
	printf( "Error status, %02X\n", (UINT16)iErrCode );
}
main( ) {
	UINT8	s;
	mDelaymS( 100 );  /* 延时100毫秒 */
	mInitSTDIO( );
	xWriteCH376Cmd( CMD_SET_USB_MODE );  /* 初始化CH376,设置USB工作模式 */
	xWriteCH376Data( 6 );  /* 模式代码,自动检测USB设备连接,代码3用于SD卡 */
	while ( 1 ) {
		printf( "Insert USB disk\n" );
		while ( mWaitInterrupt( ) != USB_INT_CONNECT );  /* 等待U盘连接,SD卡模式下需要单片机自行检测SD卡插拔 */
		mDelaymS( 250 );  /* 延时等待U盘进入正常工作状态 */
		xWriteCH376Cmd( CMD_DISK_MOUNT );  /* 初始化U盘并测试是否就绪,实际是识别U盘的类型和文件系统,在所有读写操作之前必须进行此步骤 */
		s = mWaitInterrupt( );  /* 等待中断并获取状态 */
		if ( s != USB_INT_SUCCESS ) {  /* 有的U盘一次初始化不成功,再试一次 */
			mDelaymS( 100 );  /* 出错重试 */
			xWriteCH376Cmd( CMD_DISK_MOUNT );  /* 初始化磁盘并测试磁盘是否就绪 */
			s = mWaitInterrupt( );  /* 等待中断并获取状态 */
		}
		mStopIfError( s );
		xWriteCH376Cmd( CMD_SET_FILE_NAME );  /* 设置将要操作的文件的文件名,通配符支持所有文件和子目录 */
		xWriteCH376Data( '/' );
		xWriteCH376Data( '*' );
		xWriteCH376Data( 0 );
		xWriteCH376Cmd( CMD_FILE_OPEN );  /* 枚举文件和目录 */
		while ( 1 ) {
			s = mWaitInterrupt( );
			if ( s == USB_INT_DISK_READ ) {  /* 请求数据读出 */
				xWriteCH376Cmd( CMD01_RD_USB_DATA0 );
				printf( "Name: ");
				xReadCH376Data( );  /* 长度总是sizeof(FAT_DIR_INFO) */
				for ( s = 0; s != 8+3; s ++ ) printf( "%c", xReadCH376Data( ) );
				printf( "         (%c)\n", xReadCH376Data( )&ATTR_DIRECTORY?'#':'*' );  /* 目录显示#后缀,文件显示*后缀 */
				for ( s = 11; s != sizeof( FAT_DIR_INFO ); s ++ ) xReadCH376Data( );  /* 取出后续数据 */
				xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
			}
			else if ( s == ERR_MISS_FILE ) break;  /* 没有找到更多的匹配文件,正常结束 */
			else mStopIfError( s );  /* 操作出错 */
		}
		while ( mWaitInterrupt( ) != USB_INT_DISCONNECT );  /* 等待U盘拔出 */
		mDelaymS( 250 );
	}
}
