/*
 * i2c.c
 *
 *  Created on: Dec 27, 2024
 *      Author: khans
 */
#include "i2c.h"
void i2cInit()
	 				{
					I2C1_CLOCK_ENABLE();
//						gpioInit(&I2C1_SCL_PB6); // pull-up farklı
//						gpioInit(&I2C1_SDA_PB7);
						I2C1->I2C_CR1 |= (1u << 15);
						I2C1->I2C_CR1 &= ~(1u << 15);

						I2C1_FREQ_4MHZ();
						I2C1_FREQ_SCL_100MHZ();
						I2C1_RISE_TIME_17();

//						I2C1_FREQ_45MHZ();
//						I2C1_FREQ_SCL_100MHZ_225(); //1-) ACK set edilmeden dene
//						I2C1_RISE_TIME_46();
						I2C1_ENABLE();
	 				}
void i2cWrite(uint8_t data)
{
		while(!(I2C1->I2C_SR1 & (1u << 7))); // wait for TxE to be set
		I2C1->I2C_DR = data;
		while(!(I2C1->I2C_SR1 & (1u << 2))); // wait for byte transfer finish
}
void i2cAddress(uint8_t address)
{
	I2C1->I2C_DR = address;
	while(!(I2C1->I2C_SR1 & (1u << 1)));
	(void)(I2C1->I2C_SR1 | I2C1->I2C_SR2); // CLEAR ADDR FLAG

}
void i2cWriteMulti(uint8_t* data, uint8_t size)
{
	while(!(I2C1->I2C_SR1 & (1u << 7))); // wait for TxE to be set

	while(size)
	{

		while(!(I2C1->I2C_SR1 & (1u << 7))); // wait for TxE to be set
		I2C1->I2C_DR = (volatile uint32_t)*data++;
		size--;
	}
	while(!(I2C1->I2C_SR1 & (1u << 2))); // wait for byte transfer finish
}
void i2cStart(void)
{
	I2C1_ACK_ENABLE();
	I2C1->I2C_CR1 |= (1u << 8);
	while(!(I2C1->I2C_SR1 & (1u << 0)));
}
void i2cStop(void)
{
	(I2C1->I2C_CR1 |= (1u << 9));
}
