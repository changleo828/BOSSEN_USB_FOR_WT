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

/* CH376评估板演示程序: 演示USB-HOST主机接口和USB-DEVICE设备接口的应用 */

/* 关于本程序中的CH376主机接口: HOST.C
       可以连接U盘, 支持U盘文件系统FAT12/FAT16/FAT32, 容量不限,
       插入U盘后, 该程序将U盘中的/C51/CH376HFT.C文件中的前200个字符显示出来,
       如果找不到原文件CH376HFT.C, 那么该程序将显示C51子目录下所有以CH376开头的文件名,
       如果找不到C51子目录, 那么该程序将显示根目录下的所有文件名,
       计算机端可以通过串口监控/调试工具软件以9600bps查看演示情况, 也可以使用CH341的串口工具或者模块的演示工具 */
/* 关于本程序中的CH376设备接口: DEVICE.C
       采用请求+应答模式通讯结构, 强调可靠性和交互性, 不追求传输速度,
       计算机端可以通过CH372/CH375/CH376的调试工具中的MCS51监控工具程序CH37XDBG.EXE实现对MCS51单片机的"完全"控制,
       可以读写MCS51单片机的任意外部RAM、内部RAM以及绝大多数SFR, 当然也能够进行数据通讯 */
/* 关于主从切换:
       本程序默认工作于USB-HOST主机方式, 当有USB设备连接时自动处理, 需要作为USB设备与计算机通讯时, 可以按评估板上的按钮由主程序进行切换 */

/* C51   CH376.C */
/* C51   HOST.C */
/* C51   DEVICE.C */
/* LX51  CH376.OBJ, HOST.OBJ, DEVICE.OBJ */
/* OHX51 CH376 */


#include "CH376.H"

UINT8	IsKeyPress( )
{
	if ( USER_KEY_IN == 0 ) {  /* 有键按下 */
		LED_OUT_INACT( );  /* LED灭 */
		mDelaymS( 50 );
		if ( USER_KEY_IN == 0 ) {
			while ( USER_KEY_IN == 0 );  /* 等待按键释放 */
			mDelaymS( 50 );
			while ( USER_KEY_IN == 0 );  /* 按键去抖动 */
			LED_OUT_ACT( );  /* LED亮以示工作 */
			return( 1 );
		}
		LED_OUT_ACT( );  /* LED亮以示工作 */
	}
	return( 0 );
}

main( ) {
	LED_OUT_INIT( );
	LED_OUT_ACT( );  /* 开机后LED闪烁一下以示工作 */
	mDelaymS( 100 );  /* 延时100毫秒 */
	LED_OUT_INACT( );
	mDelaymS( 100 );

	mInitSTDIO( );  /* 为了让计算机通过串口监控演示过程 */
	printf( "Start CH376 demo ...\n" );

	EA = 1;
	LED_OUT_ACT( );  /* LED亮以示工作 */
	while ( 1 ) {  /* 用户按键导致USB主从模式来回切换 */
		LED_HOST( );
		printf( "Set USB host mode\n" );
		host( );
		LED_DEVICE( );
		printf( "Set USB device mode\n" );
		device( );
	}
}
