#include "io430.h"


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
void EndSPI(void)
{
    SCS_1;
}
void TSC(void)
{
    int i;
    i = 10;
    while(i--);
}
void SPI_Init(void)
{
    //IO setup
    P4SEL_bit.P4SEL0 = 0;
    P4SEL_bit.P4SEL6 = 0;
    P2SEL_bit.P2SEL0 = 0;
    P2SEL_bit.P2SEL4 = 0;
    
    P4DIR_bit.P4DIR0 = 1;
    P4DIR_bit.P4DIR6 = 0;
    P2DIR_bit.P2DIR0 = 1;
    P2DIR_bit.P2DIR4 = 1;
    
    //
    SCS_1;
    SCK_0;
}
void SPI_Send_Byte(unsigned char temp)
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
unsigned char SPI_Receive_Byte(void)
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
void SPI_Send_Command(unsigned char command)
{
    SCS_1;
    SCS_0;
    SPI_Send_Byte(command);
}









