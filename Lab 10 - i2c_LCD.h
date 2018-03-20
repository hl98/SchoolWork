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
 * Routines for using an LCD with the PCF8574 chip and i2c buss
*/

#define I2C_WRITE 0
#define I2C_READ 1

//read the busy bit from the LCD display using the PCF8574 I2C interface
unsigned char I2C_LCD_Busy(unsigned char Slave) {

	unsigned char read_byte, Temp1;
	i2c_Start();      					// send Start
	i2c_Address(Slave, I2C_WRITE);	// Send  slave address with write operation 
	i2c_Write(0xFA);					// Set 4 data bits on the PCF8574 to HI (open coll)
	i2c_Write(0xFE);					// Now raise E to Hi, data gets output 4 bit mode
	i2c_Restart();						// Change modes to read
	i2c_Address(Slave, I2C_READ);	// Send slave address with read operation	
	while((i2c_Read(1))& 0x80);			// Wait till bit 7 turns to 0 use bitwise and
	Temp1 = i2c_Read(0);
	Temp1 &= 0x70;
	i2c_Restart();
	i2c_Address(Slave, I2C_WRITE);
	i2c_Write(0xFA);					// Set 4 data bits on the PCF8574 to HI (open coll)
	i2c_Write(0xFE);					// Now raise E to Hi, data gets output 4 bit mode
	i2c_Restart();						// Change modes to read	
	i2c_Address(Slave, I2C_READ);						
	read_byte = i2c_Read(0);			// Read lo byte
	read_byte >>= 4;
	read_byte += Temp1;
 	i2c_Stop();			  				// send Stop
	return read_byte;					// Now return cursor position	
										// If more than one byte to be read, (0) should
										// be on last byte only

}

//I2C_LCD_Init() sets up the LCD, ready, turned on, 4 bit 2 line mode and cleared @ home
void I2C_LCD_Init(unsigned char Slave) {
unsigned char Tvar1;
	i2c_Start();      					// send Start
	i2c_Address(Slave, I2C_WRITE);	// Send  slave address with write operation 
	i2c_Write(0x3C);					// Send the 3x command 3 times
	i2c_Write(0x38);					// one	
	i2c_Write(0x3C);					// Send the 3x command 3 times
	i2c_Write(0x38);					// two	
	i2c_Write(0x3C);					// Send the 3x command 3 times
	i2c_Write(0x38);					// three	
	i2c_Write(0x2C);					// set in 4 bit mode
	i2c_Write(0x28);	
	i2c_Write(0x0C);					// turn on and cursor blink, underline & home
	i2c_Write(0x08);	
	i2c_Write(0xFC);					// low byte
	i2c_Write(0xF8);
	Tvar1=I2C_LCD_Busy(Slave);
 	i2c_Stop();
}

//I2C_LCD_Pos(unsigned char Pos) moves cursor to specified position top line=0-F bottom 40-4F
void I2C_LCD_Pos(unsigned char Slave,unsigned char Pos) {
unsigned char HiBytex, LoBytex;
	HiBytex = Pos & 0x70;
	LoBytex = Pos << 4;
	i2c_Start();      					// send Start
	i2c_Address(Slave, I2C_WRITE);	// Send  slave address with write operation 
	i2c_Write(HiBytex | 0x8C);					// send Hi byte  bit 7 is the pos command
	i2c_Write(HiBytex | 0x88);					// 	
	i2c_Write(LoBytex | 0x0C);					// Send Lo byte	
	i2c_Write(LoBytex | 0x08);
	Pos =I2C_LCD_Busy(Slave);
 	i2c_Stop();
}

//IC2_LCD_Write_String(unsigned char *, char Max)  sends string to the I2C LCD display max=16 chars
void I2C_LCD_SWrite(unsigned char Slave,unsigned char * Data, char Max){
	int j, k;
	unsigned char H, L;
	i2c_Start();      					// send Start
	i2c_Address(Slave, I2C_WRITE);	// Send  slave address with write operation 

	for (j=0; j<Max; j++){
	
	H = Data[j] & 0xF0;
	L = Data[j] << 4 ;

	i2c_Write(H | 0x0D);					// Write hi byte
	i2c_Write(H | 0x09);	
	i2c_Write(L | 0x0D);					// low byte
	i2c_Write(L | 0x09);

	}
	I2C_LCD_Busy(Slave);
 	i2c_Stop();	
}

//I2C_LCD_Command(unsigned char); sends a command to the LCD
void I2C_LCD_Command(unsigned char Slave,unsigned char Cmd){

unsigned char HiBytex, LoBytex;
	HiBytex = Cmd & 0xF0;
	LoBytex = Cmd << 4;	
	i2c_Start();      					// send Start
	i2c_Address(Slave, I2C_WRITE);	// Send  slave address with write operation 
	i2c_Write(HiBytex | 0x0C);					// send Hi byte  
	i2c_Write(HiBytex | 0x08);					// 	
	i2c_Write(LoBytex | 0x0C);					// Send Lo byte	
	i2c_Write(LoBytex | 0x08);
	I2C_LCD_Busy(Slave);
 	i2c_Stop();
}
