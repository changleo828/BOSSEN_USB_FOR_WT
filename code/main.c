
#include "io430.h"
#include "HAL.h"
#include "FILE_SYS.H"

#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL (void *)0
#endif

UINT8		buf[64];

void UartInit(void)
{
    P4SEL_bit.P4SEL4 = 1;
    P4SEL_bit.P4SEL5 = 1;
    P4DIR_bit.P4DIR4 = 1;
    P4DIR_bit.P4DIR5 = 0;

    UCA1CTL0 = 0x00;
    UCA1CTL1 = 0x60;
    UCA1BRW = 0x03;
    UCA1MCTL = 0x06;
    UCA1IE = 0x01;
}
int putchar( int x )
{
    while(!UCA1IFG__SPI_bit.UCTXIFG);
    UCA1TXBUF = x;
    return x;
}
volatile unsigned char RxBuffer;
volatile unsigned char FlagRx;
#pragma vector=USCI_A1_VECTOR
__interrupt void UART1RX (void)
{
    RxBuffer = UCA1RXBUF;
    FlagRx = 1;
}
void printk(char *str)
{
    if(str == NULL)
        return;
    while(*str != '\0'){
        putchar(*str++);
    }
}
#if 0
#define IsHexChar(x)  ((((x)>=0)&&((x)<=9)) ? ('0'+(x)) : ('A' - 10 +(x)))
#else
char IsHexChar(unsigned char x)
{
    if( x<=9 )
        return '0' + x;
    else
        return 'A' - 10 + x;
}
#endif
void printHex(unsigned int x)
{
    //unsigned temp;
    printk("0x");
    putchar(IsHexChar((x>>12) & 0xF));
    putchar(IsHexChar((x>> 8) & 0xF));
    putchar(IsHexChar((x>> 4) & 0xF));
    putchar(IsHexChar((x>> 0) & 0xF));
}
void print10(unsigned int x)
{
    putchar('0' + x/10000);
    putchar('0' + x/1000%10);
    putchar('0' + x/100%10);
    putchar('0' + x/10%10);
    putchar('0' + x%10);
}

#define mWaitInterrupt  Wait376Interrupt /* 等待CH376中断并获取状态,主机端等待操作完成,返回操作状态 */
void	mStopIfError( UINT8 iErrCode ) {	/* 如果错误则停止运行并显示错误状态,正式应用还需要分析处理 */
	if ( iErrCode == USB_INT_SUCCESS ) return;
	printf( "Error status, %02X\n", (UINT16)iErrCode );
}
/* 例子:列举指定序号的目录下的所有文件 */

int main( void )
{
    int i;
    UINT8	res;
    UINT8	s;
    UINT8	NameBuf[20] = {0};
    
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR_bit.P1DIR0 = 1;
    P1OUT_bit.P1OUT0 = 1;

    UartInit();
code_start:
    for(i = 0;i<30000;i++);
    printk("Code Start !\r\n");
    printk("程序开始 !\r\n");
    
    do{
        for(i = 0;i<30000;i++);
        res = mInitCH376Host();
    }while(res != USB_INT_SUCCESS);
    printk("USB_INT_SUCCESS !\r\n");
    
    do{
        for(i = 0;i<30000;i++);
        res = CH376DiskConnect( ); /* 查询U盘是否连接，返回USB_INT_SUCCESS则说明当前已连接 */ 
    }while(res!=USB_INT_SUCCESS);
    printk("  Connect SUCCESS\r\n");
    
    do{
        for(i = 0;i<30000;i++);
        res = CH376DiskMount( ); /* 查询U盘或SD卡是否准备好，有些U盘可能需多次调用才能成功 */ 
    }while(res!=USB_INT_SUCCESS);
    printk("  U is OK !\r\n");

    strcpy( NameBuf, "\\NEWFILE000.TXT" );  /* 目标文件名 */
    printf( "Create\n" );
    s = CH376FileCreatePath( NameBuf );  /* 新建多级目录下的文件,支持多级目录路径,输入缓冲区必须在RAM中 */
    mStopIfError( s );
    
    printf( "Write\n" );
    strcpy( buf, "找不到/C51/CH376HFT.C文件\xd\n" );
    s = CH376ByteWrite( buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
    mStopIfError( s );
    
    printf( "Close\n" );
    s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
    mStopIfError( s );
    
    do{
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 1;
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 0;            
        res = CH376DiskConnect( ); /* 查询U盘是否连接，返回USB_INT_SUCCESS则说明当前已连接 */ 
        
    }while(res!=ERR_DISK_DISCON);
    printk("  ERR_DISK_DISCON\r\n");
    goto code_start;

}
