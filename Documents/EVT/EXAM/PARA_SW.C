/* CH376芯片 软件模拟8位并口连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include	"HAL.H"

/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH376芯片的引脚
      P2.0                 A0
      P2.6                 CS#    如果模拟出的并口上只有CH376,那么CS#可以直接接低电平,强制片选
      P3.6                 WR#
      P3.7                 RD#
      P0(8位端口)         D7-D0       */
sbit	CH376_A0	=	P2^0;
sbit	CH376_CS	=	P2^6;
sbit	CH376_WR	=	P3^6;
sbit	CH376_RD	=	P3^7;
#define	CH376_DATA_DAT_OUT( d )	{ P0 = d; }		/* 向并口输出数据 */
#define	CH376_DATA_DAT_IN( )	( P0 )			/* 从并口输入数据 */
#define	CH376_DATA_DIR_OUT( )					/* 设置并口方向为输出 */
#define	CH376_DATA_DIR_IN( )	{ P0 = 0xFF; }	/* 设置并口方向为输入 */

#define CH376_INT_WIRE			INT0	/* 假定CH376的INT#引脚,如果未连接那么也可以通过查询状态端口实现 */

void	CH376_PORT_INIT( void )  /* 由于使用通用I/O模拟并口读写时序,所以进行初始化 */
{
	CH376_CS = 1;
	CH376_WR = 1;
	CH376_RD = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN( );  /* 设置并口输入 */
}

//void	mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
//{
//}

#define	xEndCH376Cmd( )  /* 结束CH376命令,仅用于SPI接口方式 */

UINT8	xReadCH376Status( void )  /* 从CH376读状态,仅用于并口方式 */
{
	UINT8	mData;
	CH376_DATA_DIR_IN( );  /* 设置并口方向为输入 */
	CH376_A0 = 1;
	CH376_CS = 0;
	CH376_RD = 0;  /* 输出有效读控制信号, 读CH376芯片的状态端口 */
	CH376_CS = 0;  /* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS,强烈建议此处执行一条空指令延时以确保并口有足够时间输入数据 */
	mData = CH376_DATA_DAT_IN( );  /* 从CH376的并口输入数据 */
	CH376_RD = 1;  /* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	CH376_A0 = 0;
	return( mData );
}

void	xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
	CH376_DATA_DAT_OUT( mCmd );  /* 向CH376的并口输出数据 */
	CH376_DATA_DIR_OUT( );  /* 设置并口方向为输出 */
	CH376_A0 = 1;
	CH376_CS = 0;
	CH376_WR = 0;  /* 输出有效写控制信号, 写CH376芯片的命令端口 */
//	CH376_CS = 0;  /* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS */
	CH376_WR = 1;  /* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN( );  /* 禁止数据输出 */
/*	mDelay0_5uS( ); mDelay0_5uS( ); mDelay0_5uS( );*/  /* 延时1.5uS确保读写周期大于1.5uS,或者用状态查询代替 */
}

void	xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	CH376_DATA_DAT_OUT( mData );  /* 向CH376的并口输出数据 */
	CH376_DATA_DIR_OUT( );  /* 设置并口方向为输出 */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_WR = 0;  /* 输出有效写控制信号, 写CH376芯片的数据端口 */
//	CH376_CS = 0;  /* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS */
	CH376_WR = 1;  /* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	CH376_DATA_DIR_IN( );  /* 禁止数据输出 */
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
}

UINT8	xReadCH376Data( void )  /* 从CH376读数据 */
{
	UINT8	mData;
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
	CH376_DATA_DIR_IN( );  /* 设置并口方向为输入 */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_RD = 0;  /* 输出有效读控制信号, 读CH376芯片的数据端口 */
	CH376_CS = 0;  /* 该操作无意义,仅作延时,CH376要求读写脉冲宽度大于40nS,强烈建议此处执行一条空指令延时以确保并口有足够时间输入数据 */
	mData = CH376_DATA_DAT_IN( );  /* 从CH376的并口输入数据 */
	CH376_RD = 1;  /* 输出无效的控制信号, 完成操作CH376芯片 */
	CH376_CS = 1;
	return( mData );
}

/* 查询CH376中断(INT#低电平) */
UINT8	Query376Interrupt( void )
{
#ifdef	CH376_INT_WIRE
	return( CH376_INT_WIRE ? FALSE : TRUE );  /* 如果连接了CH376的中断引脚则直接查询中断引脚 */
#else
	return( xReadCH376Status( ) & PARA_STATE_INTB ? FALSE : TRUE );  /* 如果未连接CH376的中断引脚则查询状态端口 */
#endif
}

UINT8	mInitCH376Host( void )  /* 初始化CH376 */
{
	UINT8	res;
	CH376_PORT_INIT( );  /* 接口硬件初始化 */
	xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );
//	xEndCH376Cmd( );  // 并口方式不需要
	if ( res != 0x9A ) return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	xWriteCH376Data( 0x06 );
	mDelayuS( 20 );
	res = xReadCH376Data( );
//	xEndCH376Cmd( );  // 并口方式不需要
	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );  /* 设置模式错误 */
}
