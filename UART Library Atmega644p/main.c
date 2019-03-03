/*
 * UART Library Atmega644p.c
 *
 * Created: 18-05-2017 11:25:36
 * Author : Jagannath
 */ 
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "HEADER.h"

int main(void)
{
	initUart0(9600,NORMAL_SPEED,8,STOPBIT_1,NONE);
	initUart1(9600,NORMAL_SPEED,8,STOPBIT_1,NONE);

	serialWriteString0("Hello Serial UART0\r\n");
	serialWriteString1("Hello Serial UART1\r\n");
	serialWriteString0(JN);
	while (1) 
    {
		
    }
}

