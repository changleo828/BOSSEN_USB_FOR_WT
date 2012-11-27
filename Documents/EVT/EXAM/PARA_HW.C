/* CH376芯片 硬件标准8位并口总线连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include	"HAL.H"

/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH376芯片的引脚
      P2.0                 A0
      P2.6                 CS#    如果并口上只有CH376,那么CS#可以直接接低电平,强制片选 */

UINT8V	xdata	CH376_CMD_PORT	_at_ 0xBDF1;	/* 假定CH376命令端口的I/O地址 */
UINT8V	xdata	CH376_DAT_PORT	_at_ 0xBCF0;	/* 假定CH376数据端口的I/O地址 */

#define CH376_INT_WIRE			INT0	/* 假定CH376的INT#引脚,如果未连接那么也可以通过查询状态端口实现 */

void	CH376_PORT_INIT( void )  /* 由于使用标准并口读写时序,所以无需初始化 */
{
}

//void	mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
//{
//}

#define	xEndCH376Cmd( )  /* 结束CH376命令,仅用于SPI接口方式 */

#define	xReadCH376Status( )		( CH376_CMD_PORT )  /* 从CH376读状态,仅用于并口方式 */

void	xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
	UINT8	i;
	CH376_CMD_PORT = mCmd;
/*	mDelay0_5uS( ); mDelay0_5uS( ); mDelay0_5uS( );*/  /* 延时1.5uS确保读写周期大于1.5uS,或者用下面几行的状态查询代替 */
	for ( i = 10; i != 0; -- i ) {  /* 状态查询,等待CH376不忙,或者上面一行的延时1.5uS代替 */
		if ( ( xReadCH376Status( ) & PARA_STATE_BUSY ) == 0 ) break;  /* 检查状态端口的忙标志位 */
	}
}

#ifdef	FOR_LOW_SPEED_MCU  /* 不需要延时 */
#define	xWriteCH376Data( d )	{ CH376_DAT_PORT = d; }  /* 向CH376写数据 */
#define	xReadCH376Data( )		( CH376_DAT_PORT )  /* 从CH376读数据 */
#else
void	xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	CH376_DAT_PORT = mData;
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
}
UINT8	xReadCH376Data( void )  /* 从CH376读数据 */
{
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
	return( CH376_DAT_PORT );
}
#endif

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
