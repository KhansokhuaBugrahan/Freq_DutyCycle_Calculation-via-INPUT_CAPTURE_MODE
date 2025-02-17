/*
 * i2c-lcd.c
 *
 *  Created on: Jan 17, 2025
 *      Author: khans
 */


#include <i2clcd.h>
#include "i2c.h"
#include "armcortexm4.h"




void lcdWrite (uint8_t Address, const uint8_t *Data, int size)
{
	i2cStart();
	i2cAddress(Address);
	for(int i=0; i < size; i++)
		i2cWrite(*Data++);
	i2cStop();
}

void lcdSendCmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	lcdWrite (SLAVE_ADDRESS_LCD, data_t, 4);
}

void lcdSendData(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	lcdWrite (SLAVE_ADDRESS_LCD, data_t, 4);

}

void lcdClear(void)
{
	lcdSendCmd (0x80);
	for (int i=0; i<70; i++)
		lcdSendData (' ');

}

void lcdPutCursor(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcdSendCmd (col);
}


void lcdInit (void)
{
	// 4 bit initialisation
	delay_ms(50);
	  // wait for >40ms
	lcdSendCmd (0x30);
	delay_ms(5);  // wait for >4.1ms
	lcdSendCmd (0x30);
	delay_ms(1);  // wait for >100us
	lcdSendCmd (0x30);
	delay_ms(10);
	lcdSendCmd (0x20);  // 4bit mode
	delay_ms(10);

  // dislay initialisation
	lcdSendCmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay_ms(5);
	lcdSendCmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	delay_ms(5);
	lcdSendCmd (0x01);  // clear display
	delay_ms(5);
	lcdSendCmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay_ms(5);
	lcdSendCmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    delay_ms(5);
}

void lcdSendString (char *str)
{
	while (*str)
		lcdSendData (*str++);
}
