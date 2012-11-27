
#include "io430.h"
#include "spi.h"

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
    if( (x>=0) && (x<=9) )
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



int main( void )
{
    int i;
    unsigned char RD_Data;
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR_bit.P1DIR0 = 1;
    P1OUT_bit.P1OUT0 = 1;
    //AppUart_init();
    UartInit();
    SPI_Init();
    printk("Code Start !\r\n");
    while(1){
        
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 1;
        for(i = 0;i<30000;i++);
        P1OUT_bit.P1OUT0 = 0;
        
        SPI_Send_Command(0x06);
        SPI_Send_Byte(0x55);
        RD_Data = SPI_Receive_Byte();
        EndSPI();
        
        printHex((int)RD_Data);
        printk("  -- check IC  -0xAA-");
        
        SPI_Send_Command(0x01);
        RD_Data = SPI_Receive_Byte();
        EndSPI();
        printHex((int)RD_Data);
        printk("  -- IC V  -0x43-\r\n");
        

    };

    //return 0;
}
