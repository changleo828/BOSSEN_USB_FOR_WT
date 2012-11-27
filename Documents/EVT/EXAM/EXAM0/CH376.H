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

#include <reg52.h>
#include <stdio.h>
#include <string.h>

#include "..\HAL.H"
#include "..\DEBUG.H"

extern	UINT8		idata	buf[64];

#define	USER_KEY_IN				INT1	/* P3.3, INT1, 默认为高电平, 当用户按键后为低电平, 本程序仅作查询 */

#ifdef __C51__
#pragma NOAREGS
#endif

/* 在P1.4连接一个LED用于监控演示程序的进度,低电平LED亮,正常工作时亮,按键时灭 */
sbit P1_4  = P1^4;
#define LED_OUT_INIT( )		{ P1_4 = 1; }	/* P1.4 高电平 */
#define LED_OUT_ACT( )		{ P1_4 = 0; }	/* P1.4 低电平驱动LED显示 */
#define LED_OUT_INACT( )	{ P1_4 = 1; }	/* P1.4 低电平驱动LED显示 */
sbit P1_5  = P1^5;
/* 在P1.5连接一个LED用于监控演示程序的进度,低电平LED亮,当U盘插入后亮 */
#define LED_UDISK_IN( )		{ P1_5 = 0; }	/* P1.5 低电平驱动LED显示 */
#define LED_UDISK_OUT( )	{ P1_5 = 1; }	/* P1.5 低电平驱动LED显示 */
sbit P1_6  = P1^6;
/* 在P1.6连接一个LED用于监控演示程序的进度,低电平LED亮,当对U盘写操作时亮 */
#define LED_WR_NOW( )		{ P1_6 = 0; }	/* P1.6 低电平驱动LED显示 */
#define LED_NOT_WR( )		{ P1_6 = 1; }	/* P1.6 低电平驱动LED显示 */
sbit P1_7  = P1^7;
/* 在P1.7连接一个LED用于监控当前USB模式,亮为USB设备方式,灭为USB主机方式 */
#define LED_DEVICE( )		{ P1_7 = 0; }	/* P1.7 低电平驱动LED显示 */
#define LED_HOST( )			{ P1_7 = 1; }	/* P1.7 低电平驱动LED显示 */

/* 公用子程序 */
UINT8	IsKeyPress( void );
void	device( void );
void	host( void );
