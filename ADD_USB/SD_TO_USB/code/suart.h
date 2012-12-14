#ifndef _suart_h_
#define _suart_h_


void UartInit(void);
int putchar( int x );
char mygetchar(void);
void printk(char *str);
void printHex(unsigned int x);
void print10(unsigned int x);

#endif