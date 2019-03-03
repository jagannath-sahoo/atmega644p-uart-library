/*
 * uart.h
 * Target Device: Atmega644P
 * Created: 18-05-2017 11:25:58
 *  Author: jagan
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define UART0_BUADRATE_HIGH UBRR0H
#define UART0_BUADRATE_LOW  UBRR0L

#define UART1_BUADRATE_HIGH UBRR1H
#define UART1_BUADRATE_LOW  UBRR1L

#define EVEN 1
#define ODD  0
#define NONE	 2
#define NORMAL_SPEED 1
#define DOUBLE_SPEED 0
#define STOPBIT_1	 1
#define STOPBIT_2	 2

#define BRC_Doublerate ((F_CPU/8/buadRate)-1)
#define BRC_Normal ((F_CPU/16/buadRate)-1)

#define RX_BUFFER_SIZE	256
#define TX_BUFFER_SIZE	256


/************************************************************************/
/*	Initialize UART Communication at UART0
	buadRate: 9600,57600,115200,DOUBLE_SPEED
	asynDoubleRate: NORMAL_SPEED or
	dataBitSize: 6-bit,7-bit,8-bit,9-bit
	setStopbit: STOPBIT_1 for 1 stop bit
				STOPBIT_2 for 2 stop bits
	setParityBit: EVEN or ODD or NO_PARITY                                                                     */
/************************************************************************/
void initUart0(uint32_t buadRate,char asynDoubleRate,char dataBitSize,char setStopBit,char setParityBit);

/************************************************************************/
/*	Initialize UART Communication at UART1
	buadRate: 9600,57600,115200,DOUBLE_SPEED
	asynDoubleRate: NORMAL_SPEED or
	dataBitSize: 6-bit,7-bit,8-bit,9-bit
	setStopbit: STOPBIT_1 for 1 stop bit
				STOPBIT_2 for 2 stop bits
	setParityBit: EVEN or ODD or NO_PARITY                                                                     */
/************************************************************************/
void initUart1(uint32_t buadRate,char asynDoubleRate,char dataBitSize,char setStopBit,char setParityBit);

/************************************************************************/
/* Read a character from Serial
	return type: char 
	                                                                  */
/************************************************************************/
char getChar0(void);

/************************************************************************/
/* Get the top character of rxBuffer
	return type: char     
	                                                                */
/************************************************************************/
char peekChar0(void);

/************************************************************************/
/* Read String from Serial
	return type: pointer character string
	                                                                    */
/************************************************************************/
char* serialReadString0();

/************************************************************************/
char getChar1(void);

/************************************************************************/
/* Get the top character of rxBuffer
	return type: char     
	                                                                */
/************************************************************************/
char peekChar1(void);

/************************************************************************/
/* Read String from Serial
	return type: pointer character string
	                                                                    */
/************************************************************************/
char* serialReadString1();

/************************************************************************/
/* Send A single Character using UART0
	c: Single Character                                                                    */
/************************************************************************/
void sendChar1(char c);

/************************************************************************/
/* Send A single Character using UART1
	c: Sigle Character                                                                    */
/************************************************************************/
void sendChar1(char c);

/************************************************************************/
/*  Append characters into the rxBuffer
	char: pass the character to add into rxBuffer  
	                                                              */
/************************************************************************/
void appendSerial0(char c);

/************************************************************************/
/*	Write string to Serial
	c[]: Pass the String to transmit 
	                                                               */
/************************************************************************/
void serialWriteString0(char c[]);

/************************************************************************/
/*  Append characters into the rxBuffer
	char: pass the character to add into rxBuffer  
	                                                              */
/************************************************************************/
void appendSerial1(char c);

/************************************************************************/
/*	Write string to Serial
	c[]: Pass the String to transmit 
	                                                               */
/************************************************************************/
void serialWriteString1(char c[]);

/************************************************************************/
/*	Find Key word in the Buffer
	findStr: Specify the key word to search in rx Buffer
	                                                                     */
/************************************************************************/
uint8_t serialFind0(char findStr[]);

/************************************************************************/
/*	Check if data is present to receive
	return: 1 for true else 0                                                          */
/************************************************************************/
uint8_t serialAvailable0();

/************************************************************************/
/*	Find Key word in the Buffer
	findStr: Specify the key word to search in rx Buffer
	                                                                     */
/************************************************************************/
uint8_t serialFind1(char findStr[]);

/************************************************************************/
/*	Check if data is present to receive
	return: 1 for true else 0                                                          */
/************************************************************************/
uint8_t serialAvailable1();

/************************************************************************/
/* Setup AVR Milli()                                                                     */
/************************************************************************/
void setupMilli();

/************************************************************************/
/* Return milliseconds                                                                     */
/************************************************************************/
uint64_t millis();

/************************************************************************/
/* Convert Hex String to Integer
	hex: hex Data in string format
    return: data in Int format                                                                 */
/************************************************************************/
int convertHexToInt(char* hex);

/************************************************************************/
/* Convert Int to String
	data: Int data
	return: String data                                                                    */
/************************************************************************/
char* convertIntToString(uint64_t data);

/************************************************************************/
/* convert Float To String
	data: float type
	return: string type                                                                     */
/************************************************************************/
char* convertFloatToString(float data);

#endif /* UART_H_ */