
#include "io430.h"
#include "HAL.h"
#include "FILE_SYS.H"

#ifndef NULL
#define NULL (void *)0
#endif

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
int putchar( int x )//注意不要改参数的类型和返回值的类型，否则printf调用是就有问题了。
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
    //U0TXBUF = RxBuffer;
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

int main( void )
{
    int i;
    UINT8	res;
    UINT8	s;
    //UINT8 name[] = "asdf.txt";
    //unsigned char RD_Data;
    
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR_bit.P1DIR0 = 1;
    P1OUT_bit.P1OUT0 = 1;
    //AppUart_init();
    UartInit();

    for(i = 0;i<30000;i++);
    printk("Code Start !\r\n");
    
    
#if 0
    res = mInitCH376Host();
    if(res == USB_INT_SUCCESS)
        printk("USB_INT_SUCCESS !\r\n");
    else
        printk("USB_INT_ERR_USB_UNKNOWN !\r\n");
    
        res = CH376DiskConnect( ); /* 查询U盘是否连接，返回USB_INT_SUCCESS则说明当前已连接 */ 
        if ( res==USB_INT_SUCCESS ) /* 已经连接 */ 
            printk("  Connect SUCCESS\r\n");
        else if ( res==ERR_DISK_DISCON ) /* 已经断开 */
            printk("  ERR_DISK_DISCON\r\n");
        else
            printk("  ---------------\r\n");
        
        
//初始化并检查U盘或者SD卡是否准备好，准备好后才能进行文件读写。示例： 
        s=CH376DiskMount( ); /* 查询U盘或SD卡是否准备好，有些U盘可能需多次调用才能成功 */ 
        if ( s!=USB_INT_SUCCESS ) 
            printk("  -- U is not OK !\r\n");/* 还未准备好 */ 
        else 
            printk("  -- U is OK !\r\n");/* 准备好了，可以读写数据 */
        
        /* 在根目录或者当前目录下新建文件，如果文件已经存在则先删除再新建 */ 
        CH376FileCreate( "/dBC.txt" );  
        
    CH376FileClose( TRUE );     /* 关闭当前已经打开的文件或者目录(文件夹) */ 
    while(1){
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 1;
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 0;
    };
#else

	//mDelaymS( 10 );  /* 延时100毫秒 */
    res = mInitCH376Host();
    if(res == USB_INT_SUCCESS)
        printk("USB_INT_SUCCESS !\r\n");
    else
        printk("USB_INT_ERR_USB_UNKNOWN !\r\n");
	while ( 1 ) {
		printk( "Insert USB disk\n" );
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS );  /* 等待U盘连接,SD卡模式下需要单片机自行检测SD卡插拔 */
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
				printk( "Name: ");
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
#endif
    //return 0;
}
