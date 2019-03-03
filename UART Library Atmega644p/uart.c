/*
 * uart.c
 * Target Device: Atmega644P
 * Created: 18-05-2017 11:26:16
 *  Author: Jagannath Sahoo
 */ 

//#define F_CPU 16000000	//16Mhz
#define F_CPU 8000000//8Mhz
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


struct UART {
	char rxBuffer[RX_BUFFER_SIZE];
	char serialBuffer[TX_BUFFER_SIZE];
	uint16_t rxReadPos;
	uint16_t rxWritePos;
	uint16_t serialReadPos;
	uint16_t serialWritePos;
	}UART0,UART1;

//struct UART UART0,UART1;

uint64_t _millis = 0;
uint16_t _250us = 0;
char readString[RX_BUFFER_SIZE];
char strTemp[64];//Temp Space for conversion Int/Float to String
int numTemp;//Temp space for convert Hex string to Int 

void initUart0(uint32_t buadRate,char asynDoubleRate,char dataBitSize,char setStopBit,char setParityBit)
{
	//Setting Baud Rate
	if (asynDoubleRate == 1)
	{
		UART0_BUADRATE_HIGH = (BRC_Normal >> 8);
		UART0_BUADRATE_LOW = BRC_Normal;
	}
	else
	{
		UCSR0A |= (1<<U2X0);//Setting Double Speed Clock rate
		UART0_BUADRATE_HIGH = (BRC_Doublerate >> 8);
		UART0_BUADRATE_LOW = BRC_Doublerate;
	}
	//Enable RXEN0 and TXEN0 with Interrupt for TX AND RX
	//RXCIE0 and TXCIE0 Set Interrupt
	UCSR0B = (1 << RXEN0) | (1<< RXCIE0) | (1<<TXEN0) | (1<<TXCIE0);
	
	//Stop Bit
	//By Default on Reset Stop bit is clear
	if (setStopBit == 2)	UCSR0C |= (1 << USBS0); //Sets 2 stop bits
	
	//Data bit size
	if (dataBitSize == 6)	UCSR0C |= (1 << UCSZ00); //6-bit data length
	if (dataBitSize == 7)	UCSR0C |= (1 << UCSZ01); //7-bit data length
	if (dataBitSize == 7)	UCSR0C = (1 << UCSZ01) | (1<<UCSZ00);//8-bit data length
	if (dataBitSize == 7)	UCSR0C |= (7 << UCSZ00); //9-bit data length
	
	//Parity setup 1=> EVEN and 0 => ODD
	if (setParityBit == 1)	UCSR0C |= (1 << UPM01); //Sets parity to EVEN
	if (setParityBit == 0)	UCSR0C |= (1 << UPM01) | (1 << UPM00); //Sets parity to ODD
	
	sei();//Enabling Interrupts
	_delay_us(1000);
	
}

void initUart1(uint32_t buadRate,char asynDoubleRate,char dataBitSize,char setStopBit,char setParityBit)
{
	//Setting Baud Rate
	if (asynDoubleRate == 1)
	{
		UART1_BUADRATE_HIGH = (BRC_Normal >> 8);
		UART1_BUADRATE_LOW = BRC_Normal;
	}
	else
	{
		UCSR1A |= (1<<U2X1);//Setting Double Speed Clock rate
		UART1_BUADRATE_HIGH = (BRC_Doublerate >> 8);
		UART1_BUADRATE_LOW = BRC_Doublerate;
	}
	//Enable RXEN1 and TXEN1 with Interrupt for TX AND RX
	//RXCIE1 and TXCIE1 Set Interrupt
	UCSR1B = (1 << RXEN1) | (1<< RXCIE1) | (1<<TXEN1) | (1<<TXCIE1);
	
	//Stop Bit
	//By Default on Reset Stop bit is clear
	if (setStopBit == 2)	UCSR1C |= (1 << USBS0); //Sets 2 stop bits
	
	//Data bit size
	if (dataBitSize == 6)	UCSR1C |= (1 << UCSZ00); //6-bit data length
	if (dataBitSize == 7)	UCSR1C |= (1 << UCSZ01); //7-bit data length
	if (dataBitSize == 7)	UCSR1C = (1 << UCSZ01) | (1<<UCSZ00);//8-bit data length
	if (dataBitSize == 7)	UCSR1C |= (7 << UCSZ00); //9-bit data length
	
	//Parity setup 1=> EVEN and 0 => ODD
	if (setParityBit == 1)	UCSR1C |= (1 << UPM01); //Sets parity to EVEN
	if (setParityBit == 0)	UCSR1C |= (1 << UPM01) | (1 << UPM00); //Sets parity to ODD
	
	sei();//Enabling Interrupts
	_delay_us(1000);
}


/************************************************************************/
/* Receive Serial Operations                                                                     */
/************************************************************************/

//Read a character only
char getChar0()
{
	char ret = '\0';
	if (UART0.rxReadPos != UART0.rxWritePos)
	{
		ret = UART0.rxBuffer[UART0.rxReadPos];
		//_delay_ms(5);
		UART0.rxReadPos++;
		if (UART0.rxReadPos >= RX_BUFFER_SIZE)
		{
			UART0.rxReadPos = 0;
		}
	}
	return ret;
}

//get the rxBuffer top element
char peekChar0(void)
{
	char ret = '\0';
	if (UART0.rxReadPos != UART0.rxWritePos)
	{
		ret = UART0.rxBuffer[UART0.rxReadPos];
	}
	return ret;
}

//Receive String from Serial
char* serialReadString0()
{
	//_delay_ms(1000);
	//char readString[RX_BUFFER_SIZE];
	uint16_t i = 0;
	memset(readString,0,RX_BUFFER_SIZE);
	while(i < RX_BUFFER_SIZE)
	{
		readString[i] = getChar0();
		i++;
	}
	memset(UART0.rxBuffer,0,RX_BUFFER_SIZE);
	UART0.rxReadPos = 0;
	UART0.rxWritePos = 0;
	return readString;
}

//Read a character only
char getChar1()
{
char ret = '\0';
	if (UART1.rxReadPos != UART1.rxWritePos)
	{
		ret = UART1.rxBuffer[UART1.rxReadPos];
		//_delay_ms(5);
		UART1.rxReadPos++;
		if (UART1.rxReadPos >= RX_BUFFER_SIZE)
		{
			UART1.rxReadPos = 0;
		}
	}
	return ret;
}

//get the rxBuffer top element
char peekChar1(void)
{
	char ret = '\0';
	if (UART1.rxReadPos != UART1.rxWritePos)
	{
		ret = UART1.rxBuffer[UART1.rxReadPos];
	}
	return ret;
}

//Receive String from Serial
char* serialReadString1()
{
	//_delay_ms(1000);
	//char readString[RX_BUFFER_SIZE];
	uint16_t i = 0;
	memset(readString,0,RX_BUFFER_SIZE);
	while(i < RX_BUFFER_SIZE)
	{
		readString[i] = getChar1();
		i++;
	}
	memset(UART1.rxBuffer,0,RX_BUFFER_SIZE);
	UART1.rxReadPos = 0;
	UART1.rxWritePos = 0;
	return readString;
}
/************************************************************************/
/* Transmission Serial Operations                                                                     */
/************************************************************************/
// Add element to TX Buffer
void appendSerial0(char c)
{
	UART0.serialBuffer[UART0.serialWritePos] = c;
	UART0.serialWritePos++;
	if (UART0.serialWritePos >= TX_BUFFER_SIZE)//Re-initialize to SerialwritePos = 0
	{
		UART0.serialWritePos = 0;
	}
}

//Send String to Serial
void serialWriteString0(char c[])
{
	UART0.serialReadPos = 0;
	UART0.serialWritePos = 0;
	uint16_t i = 0;
	for (i = 0; i< strlen(c); i++)
	{
		appendSerial0(c[i]);
	}
	memset(UART0.rxBuffer,0,RX_BUFFER_SIZE);
	if (UCSR0A & (1<<UDRE0))//Flag set=> ready to receive
	{
		UCSR0B |= (1<<UDRIE0);
	}
}

// Add element to TX Buffer
void appendSerial1(char c)
{
	UART1.serialBuffer[UART1.serialWritePos] = c;
	UART1.serialWritePos++;
	if (UART1.serialWritePos >= TX_BUFFER_SIZE)//Re-initialize to SerialwritePos = 0
	{
		UART1.serialWritePos = 0;
	}
}

//Send String to Serial
void serialWriteString1(char c[])
{
	UART1.serialReadPos = 0;
	UART1.serialWritePos = 0;
	uint16_t i = 0;
	for (i = 0; i< strlen(c); i++)
	{
		appendSerial1(c[i]);
	}
	memset(UART1.rxBuffer,0,RX_BUFFER_SIZE);
	if (UCSR1A & (1<<UDRE1))//Flag set=> ready to receive
	{
		UCSR1B |= (1<<UDRIE1);
	}
}

//Send a Character only UART0
void sendChar0(char c)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

//Send a Character only UART1
void sendChar1(char c)
{
	while (!(UCSR1A & (1<<UDRE1)));
	UDR1 = c;
}

//Serial Find key Word
uint8_t serialFind0(char findStr[])
{
	//_delay_ms(500);//Remove in future
	if (strstr(serialReadString0(), findStr))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Serial Find key Word
uint8_t serialFind1(char findStr[])
{
	//_delay_ms(500);//Remove in future
	if (strstr(serialReadString1(), findStr))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//check serial data available
uint8_t serialAvailable0()
{
	if (!(UCSR0A & (1<<RXC0)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//check serial data available
uint8_t serialAvailable()
{
	if (!(UCSR1A & (1<<RXC1)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/************************************************************************/
/* Interrupt Service Routine to Receive                                                                     */
/************************************************************************/
ISR(USART0_RX_vect)
{
	while ( !(UCSR0A & (1<<RXC0)) );
	UART0.rxBuffer[UART0.rxWritePos] = UDR0;
	if (UART0.rxBuffer[UART0.rxWritePos] != 0)
	{
		UART0.rxWritePos++;
		if (UART0.rxWritePos >= RX_BUFFER_SIZE)
		{
			UART0.rxWritePos = 0;
		}
	}
}

ISR(USART1_RX_vect)
{
	while ( !(UCSR1A & (1<<RXC1)) );
	UART1.rxBuffer[UART1.rxWritePos] = UDR1;
	if (UART1.rxBuffer[UART1.rxWritePos] != 0)
	{
		UART1.rxWritePos++;
		if (UART1.rxWritePos >= RX_BUFFER_SIZE)
		{
			UART1.rxWritePos = 0;
		}
	}
}

/************************************************************************/
/* Interrupt Service Routine to Transmit                                                                     */
/************************************************************************/
ISR(USART0_UDRE_vect)
{
	if (UART0.serialReadPos != UART0.serialWritePos)//if equal then we are the end of array
	{
		if (UART0.serialBuffer[UART0.serialReadPos] != 0)
		{
			UDR0 = UART0.serialBuffer[UART0.serialReadPos];
			_delay_ms(5);
			UART0.serialReadPos++;
			if (UART0.serialReadPos >= TX_BUFFER_SIZE)
			{
				UART0.serialReadPos=0;//serialReadPos++;
			}
		}
		else
		{
			UART0.serialReadPos++;
		}
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);
		UCSR0A &= ~(1<<UDRE0);
	}
}

ISR(USART1_UDRE_vect)
{
	if (UART1.serialReadPos != UART1.serialWritePos)//if equal then we are the end of array
	{
		if (UART1.serialBuffer[UART1.serialReadPos] != 0)
		{
			UDR1 = UART1.serialBuffer[UART1.serialReadPos];
			_delay_ms(5);
			UART1.serialReadPos++;
			if (UART1.serialReadPos >= TX_BUFFER_SIZE)
			{
				UART1.serialReadPos=0;//serialReadPos++;
			}
		}
		else
		{
			UART1.serialReadPos++;
		}
	}
	else
	{
		UCSR1B &= ~(1<<UDRIE1);
		UCSR1A &= ~(1<<UDRE1);
	}
}

/************************************************************************/
/* TIMER 0 Programming                                                                      */
/************************************************************************/
//Setup AVR Millis()
void setupMilli()
{
	_millis = 0;
	/* interrupt setup */
	// Pre-scale timer0 to 1/64th the clock rate
	// overflow timer0 every 0.5ms
	TCCR0B |= (1<<CS01) | (1<<CS00);
	// enable timer overflow interrupt
	TIMSK0  |= 1<<TOIE0;
	// Enable global interrupts
	sei();
}
//Returns milliseconds time escape
uint64_t millis() {
	return _millis;
}
/************************************************************************/
/* AVR Millis() Timer0 Over flow Interrupt                                                                     */
/************************************************************************/
ISR(TIMER0_OVF_vect) {
	_250us += 250;
	while(_250us > 500){
		_millis++;
		_250us -= 500;
	}
}

/************************************************************************/
/* Data Conversion                                                                     */
/************************************************************************/
char* convertIntToString(uint64_t data)//use itoa() in future
{
	memset(strTemp,0,64);
	sprintf(strTemp, "%d",(int) data);
	return strTemp;
}

char* convertFloatToString(float data)//Error must be handled in future
{
	memset(strTemp,0,64);
	sprintf(strTemp, "%g",data);
	return strTemp;
}

int convertHexToInt(char hex[])
{
	numTemp = (int)strtol(hex, NULL, 16);
	return numTemp;
}
