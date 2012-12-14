
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
    int j;
    UINT8	res;
    UINT8	s;
    UINT8	NameBuf[20] = {0};
    UINT8       CountT = 0;
    UINT32      U_D;
    PUINT32     pU_D;
    pU_D = &U_D;
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR_bit.P1DIR0 = 1;
    P1OUT_bit.P1OUT0 = 1;

    UartInit();
code_start:
    CountT ++;
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

    s = CH376DiskCapacity(pU_D);
    mStopIfError( s );
    printk("  U盘大小:");
    printHex((unsigned int)U_D);
    printHex((unsigned int)(U_D>>16));
    printk("\r\n");
    
    s = CH376DiskQuery(pU_D);
    mStopIfError( s );
    printk("  剩余大小:");
    printHex((unsigned int)U_D);
    printHex((unsigned int)(U_D>>16));
    printk("\r\n");
    strcpy( NameBuf, "\\2012.TXT" );  /* 目标文件名 */
    
#if 0
    
    printf( "Open\r\n" );
    s = CH376FileOpenPath( NameBuf );
    mStopIfError( s );
    if ( s == ERR_MISS_FILE ){
        printf( "Create\r\n" );
        s = CH376FileCreatePath( NameBuf );  /* 新建多级目录下的文件,支持多级目录路径,输入缓冲区必须在RAM中 */
        mStopIfError( s );
    }
    
    printf( "CH376ByteLocate\r\n" );
    s = CH376ByteLocate(0xFFFFFFFF);
    mStopIfError( s );
    
    printf( "Write\r\n" );
    strcpy( buf, "时间        数据  ---\xd\n" );
    s = CH376ByteWrite( buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
    mStopIfError( s );
    
    strcpy( buf, "2012-12-01  01    ---\xd\n" );
    buf[9] = '0' + CountT;
    
    for(i = 0; i < 100;i ++){
        buf[12] = '0' + i/10;
        buf[13] = '0' + i%10;
        printf( "Write %s" ,buf);
        s = CH376ByteWrite( buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
        mStopIfError( s );            
    }
    
    printf( "Close\r\n" );
    s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
    mStopIfError( s );
#endif
/* 如果MY_ADC.TXT文件已经存在则添加数据到尾部,如果不存在则新建文件 */
        printf( "Open\n" );
        s = CH376FileOpen( NameBuf );  /* 打开文件,该文件在根目录下 */
        if ( s == USB_INT_SUCCESS ) {  /* 文件存在并且已经被打开,移动文件指针到尾部以便添加数据 */
                printf( "File size = %ld\n", CH376GetFileSize( ) );  /* 读取当前文件长度 */
                printf( "Locate tail\n" );
                s = CH376ByteLocate( 0xFFFFFFFF );  /* 移到文件的尾部 */
                mStopIfError( s );
        }
        else if ( s == ERR_MISS_FILE ) {  /* 没有找到文件,必须新建文件 */
                printf( "Create\n" );
                s = CH376FileCreate( NULL );  /* 新建文件并打开,如果文件已经存在则先删除后再新建,不必再提供文件名,刚才已经提供给CH376FileOpen */
                mStopIfError( s );
        }
        else mStopIfError( s );  /* 打开文件时出错 */
        printf( "Write begin\n" );
        s = sprintf( buf, "此前文件长度= %ld 字节\xd\xa", CH376GetFileSize( ) );  /* 注意字符串长度不能溢出buf,否则加大缓冲区或者分多次写入 */
        s = CH376ByteWrite( buf, s, NULL );  /* 以字节为单位向文件写入数据 */
        mStopIfError( s );
        printf( "Write ADC data\n" );
        printf( "Current offset ( file point ) is :        ");
        for(i = 0;i<1000;i++){
            s = sprintf( buf, "%02d.%02d.%02d.%d\xd\xa", 2012 + i/365, 1 + (i / 30) % 12, 1 + i % 30, i );  
            /* 将二制制数据格式为一行字符串 */
            s = CH376ByteWrite( buf, s, NULL );  /* 以字节为单位向文件写入数据 */
            /* 有些U盘可能会要求在写数据后等待一会才能继续操作,
            所以,如果在某些U盘中发生数据丢失现象,
            建议在每次写入数据后稍作延时再继续 */
            for(j = 0;j<30;j++);
            mStopIfError( s );
            printf( "\b\b\b\b\b\b" );
            printf( "%6ld", CH376ReadVar32( VAR_CURRENT_OFFSET ) );  
            /* 读取当前文件指针 */
        }
        
        printf( "Write end\n" );
        strcpy( buf, "今天的ADC数据到此结束\xd\xa" );
        s = CH376ByteWrite( buf, strlen( buf ), NULL );  /* 以字节为单位向文件写入数据 */
        mStopIfError( s );
/* 如果实际产品中有实时时钟,可以根据需要将文件的日期和时间修改为实际值,参考EXAM10用CH376DirInfoRead/CH376DirInfoSave实现 */
        printf( "Close\n" );
        s = CH376FileClose( TRUE );  /* 关闭文件,自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度 */
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
