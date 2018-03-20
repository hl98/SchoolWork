/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

/*
 * Hi-Tech C I2C library for 16F1825
 * Master mode routines for I2C MSSP port to read and write to slave device 
 * Copyright (C)2011 HobbyTronics.co.uk 2011
 * Freely distributable.
*/

#define I2C_WRITE 0
#define I2C_READ 1

// Initialise MSSP1 port. (16F1829 on VIVA board - other devices may differ)
void i2c_Init(void){

	// Initialise I2C MSSP 1 module
	// Master 100KHz
	TRISB4=1;           	// set SCL and SDA pins as inputs
	TRISB6=1;

	SSP1CON1 = 0b00101000; 	// I2C enabled, Master mode
	SSP1CON2 = 0x00;
    // I2C Master mode, clock = FOSC/(4 * (SSP1ADD + 1)) 
    SSP1ADD = 9;    		// 100Khz @ 4Mhz Fosc

	SSP1STAT = 0b11000000; 	// Slew rate disabled

}

// i2c_Wait - wait for I2C transfer to finish
void i2c_Wait(void){
    while ( ( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) );  
}

// i2c_Start - Start I2C communication
void i2c_Start(void)
{
 	i2c_Wait();
	SSP1CON2bits.SEN=1;
}

// i2c_Restart - Re-Start I2C communication
void i2c_Restart(void){
 	i2c_Wait();
	SSP1CON2bits.RSEN=1;
}

// i2c_Stop - Stop I2C communication
void i2c_Stop(void)
{
 	i2c_Wait();
 	SSP1CON2bits.PEN=1;
}

// i2c_Write - Sends one byte of data
void i2c_Write(unsigned char data)
{
 	i2c_Wait();
 	SSP1BUF = data;
}

// i2c_Address - Sends Slave Address and Read/Write mode
// mode is either I2C_WRITE or I2C_READ
void i2c_Address(unsigned char address, unsigned char mode)
{
	unsigned char l_address;

	l_address=address<<1;
	l_address+=mode;
 	i2c_Wait();
 	SSP1BUF = l_address;
}

// i2c_Read - Reads a byte from Slave device
unsigned char i2c_Read(unsigned char ack)
{
	// Read data from slave
	// ack should be 1 if there is going to be more data read
	// ack should be 0 if this is the last byte of data read
 	unsigned char i2cReadData;

 	i2c_Wait();
	SSP1CON2bits.RCEN=1;
 	i2c_Wait();
 	i2cReadData = SSP1BUF;
 	i2c_Wait();
 	if ( ack ) SSP1CON2bits.ACKDT=0;			// Ack
	else       SSP1CON2bits.ACKDT=1;			// NAck
	SSP1CON2bits.ACKEN=1;   		            // send acknowledge sequence

	return( i2cReadData );
}
