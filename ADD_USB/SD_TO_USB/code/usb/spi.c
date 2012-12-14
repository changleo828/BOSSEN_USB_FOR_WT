#include "io430.h"
#include	"HAL.H"



#define   SPI_DO_L    ( P5OUT &=~ BIT2 )
#define   SPI_DO_H    ( P5OUT |= BIT2 )
#define   SPI_SCL_L   ( P5OUT &=~ BIT3 )
#define   SPI_SCL_H   ( P5OUT |= BIT3 )
#define   SPI_CS_L    ( P5OUT &=~ BIT4 )
#define   SPI_CS_H    ( P5OUT |= BIT4 )

#if 0
#define SCK  (P4OUT_bit.P4OUT0)
#define SCS  (P2OUT_bit.P2OUT4)
#define SDO  (P2OUT_bit.P2OUT0)
#define SDI  (P4IN_bit.P4IN6)

#define SDO_0    (SDO = 0)
#define SDO_1    (SDO = 1)
#define SCK_0    (SCK = 0)
#define SCK_1    (SCK = 1)
#define SCS_0    (SCS = 0)
#define SCS_1    (SCS = 1)
#define IsSDI()  (SDI)
#endif

#define SDO_0    SPI_DO_L
#define SDO_1    SPI_DO_H
#define SCK_0    SPI_SCL_L
#define SCK_1    SPI_SCL_H
#define SCS_0    SPI_CS_L
#define SCS_1    SPI_CS_H
#define IsSDI()  ( P5IN & BIT1 )


void mDelayuS(unsigned int x)
{
    int i;
    
    while(x--)
        for(i = 0;i <200;i ++);
}
void mDelaymS(unsigned int x)
{
    while(x--)
        mDelayuS(10);
}
void xEndCH376Cmd(void)
{
    SCS_1;
}
void TSC(void)
{
    int i;
    i = 10;
    while(i--);
}
void CH376_PORT_INIT(void)
{
    //IO setup
    P5SEL &= 0xE1;
    P5DIR &= 0xFD;
    P5DIR |= 0x1C;
    //
    SCS_1;
    SCK_0;
}
void xWriteCH376Data(unsigned char temp)
{
    int i;
    
    for(i = 0;i < 8;i ++)
    {
        SCK_0;
        if((temp<<i) & 0x80){
            SDO_1;
        }else{
            SDO_0;
        }
        SCK_1;
    }
}
unsigned char xReadCH376Data(void)
{
    unsigned char temp = 0;
    int i;
    
    for(i = 0;i < 8;i ++)
    {
        SCK_0;
        temp <<= 1;
        if(IsSDI())
            temp |= 0x01;
        SCK_1;
    }
    return temp;
}
void xWriteCH376Cmd(unsigned char command)
{
    SCS_1;
    SCS_0;
    xWriteCH376Data(command);
}

/* 查询CH376中断(INT#低电平) */
unsigned char	Query376Interrupt( void )
{
    return( IsSDI() ? FALSE : TRUE );  
    /* 如果未连接CH376的中断引脚则查询兼做中断输出的SDO引脚状态 */
}

UINT8	mInitCH376Host( void )  /* 初始化CH376 */
{
    UINT8	res;
    CH376_PORT_INIT( );  /* 接口硬件初始化 */
    xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* 测试单片机与CH376之间的通讯接口 */
    xWriteCH376Data( 0x65 );
    res = xReadCH376Data( );
    xEndCH376Cmd( );
    if ( res != 0x9A ) return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
    xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
    xWriteCH376Data( 0x06 );
    mDelayuS( 20 );
    res = xReadCH376Data( );
    xEndCH376Cmd( );

    xWriteCH376Cmd( CMD20_SET_SDO_INT );  /* 设置SPI的SDO引脚的中断方式 */
    xWriteCH376Data( 0x16 );
    xWriteCH376Data( 0x90 );  /* SDO引脚在SCS片选无效时兼做中断请求输出 */
    xEndCH376Cmd( );

    if ( res == CMD_RET_SUCCESS ) 
        return( USB_INT_SUCCESS );
    else 
        return( ERR_USB_UNKNOWN );  /* 设置模式错误 */
}







