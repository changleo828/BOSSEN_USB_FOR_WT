
#include <io430.h>

/*******************************************************************/

#ifndef NULL
#define NULL (void *)0
#endif

volatile char RxBuffer;
volatile char FlagRx;
#define UART_USE_0
//#define UART_USE_1
#ifdef UART_USE_0
void UartInit(void)
{
    /* reset uart */
    U0CTL |= SWRST;
    /* Set IO */
    P3SEL |= 0x30; 
    P3DIR |= BIT4;
    P3DIR &= ~BIT5;
#if 0
    /* CLK sel */
    U0TCTL |= SSEL0;
    /* baud Rate = 9600,uclk 32768Hz */
    UBR00 = 0x03;
    UBR10 = 0x00;
    UMCTL0 = 0x4A;
#else
    /* CLK sel */
    U0TCTL |= SSEL0 + SSEL1;
    /* baud Rate = 9600,uclk 8MHz */
    UBR00 = 0x11;
    UBR10 = 0x04;
    UMCTL0 = 0xAB;
#endif
    /* frame mode */
    U0CTL = (0*PENA)|    /* Parity disabled */
            (0*PEV)|
            (0*SPB)|     /* One stop bit */
            (1*CHAR)|    /* 8bit */
            (0*LISTEN)|
            (0*SYNC)|    /* uart mode */
            (0*MM)|
            (1*SWRST);
    /* Enable TX */
    ME1 |= UTXE0;
    /* clear reset uart */
    U0CTL &= ~SWRST;
    /* Enable Rx interrupt */
    //IE1 |= URXIE0;
}

#pragma vector=USART0RX_VECTOR
__interrupt void UART0RX (void)
{
    RxBuffer = U0RXBUF;
    U0TXBUF = RxBuffer;
    FlagRx = 1;
} 
#endif
#ifdef UART_USE_1
void UartInit(void)
{
    /* reset uart */
    U1CTL |= SWRST;
    /* Set IO */
    P3SEL |= 0xC0; 
    P3DIR |= BIT6;
    P3DIR &= ~BIT7;
#if 1
    /* CLK sel */
    U1TCTL |= SSEL0;
    /* baud Rate = 9600,uclk 32768Hz */
    UBR01 = 0x03;
    UBR11 = 0x00;
    UMCTL1 = 0x4A;
#else
    /* CLK sel */
    U0TCTL |= SSEL0 + SSEL1;
    /* baud Rate = 9600,uclk 8MHz */
    UBR01 = 0x41;
    UBR11 = 0x03;
    UMCTL1 = 0x49;
#endif
    /* frame mode */
    U1CTL = (0*PENA)|    /* Parity disabled */
            (0*PEV)|
            (0*SPB)|     /* One stop bit */
            (1*CHAR)|    /* 8bit */
            (0*LISTEN)|
            (0*SYNC)|    /* uart mode */
            (0*MM)|
            (1*SWRST);
    /* Enable RX TX */
    ME2 |= UTXE1 + URXE1;
    /* clear reset uart */
    U1CTL &= ~SWRST;
    /* Enable Rx interrupt */
    IE2 |= URXIE1;
}

#pragma vector=USART0RX_VECTOR
__interrupt void UART0RX (void)
{
    RxBuffer = U1RXBUF;
    U1TXBUF = RxBuffer;
    FlagRx = 1;
} 
#endif

#ifdef UART_USE_0

int putchar( int x )//注意不要改参数的类型和返回值的类型，否则printf调用是就有问题了。
{
    while(!(IFG1 & UTXIFG0));
    U0TXBUF = x;
    return x;
}
#else

int putchar( int x )//注意不要改参数的类型和返回值的类型，否则printf调用是就有问题了。
{
    while(!(IFG2 & UTXIFG1));
    U1TXBUF = x;
    return x;
}
#endif

char mygetchar(void)
{
    FlagRx = 0;
    while(!FlagRx);
    return RxBuffer;
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