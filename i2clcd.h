/*
 * i2c-lcd.h
 *
 *  Created on: Jan 17, 2025
 *      Author: khans
 */

#ifndef I2CLCD_H_
#define I2CLCD_H_

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup
#define	LCD_CLEAR_LINE		("                ")
void lcdInit (void);   // initialize lcd

void lcdSendCmd (char cmd);  // send command to the lcd

void lcdSenData (char data);  // send data to the lcd

void lcdSendString (char *str);  // send string to the lcd

void lcdPutCursor(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcdClear (void);

#endif /* I2CLCD_H_ */
