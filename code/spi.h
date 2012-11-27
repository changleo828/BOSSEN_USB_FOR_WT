#ifndef _SPI_H_
#define _SPI_H_
void SPI_Init(void);

unsigned char SPI_Receive_Byte(void);
void SPI_Send_Byte(unsigned char temp);
void SPI_Send_Command(unsigned char command);
void EndSPI(void);
#endif