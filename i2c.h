/*
 * i2c.h
 *
 *  Created on: Dec 27, 2024
 *      Author: khans
 */

#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#include "stm32f4xx.h"

#define 		I2C1_CLOCK_ENABLE()				(RCC->APB1ENR |= (1u << 21))
#define 		I2C1_CLOCK_DISABLE()			(RCC->APB1ENR &= ~(1u << 21))

typedef struct {

	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
	volatile uint32_t I2C_FLTR;

}I2C_RegDef_t;

#define 		I2C1		((I2C_RegDef_t*)(I2C1_BASE_ADDR))
#define 		I2C2		((I2C_RegDef_t*)(I2C2_BASE_ADDR))
#define 		I2C3		((I2C_RegDef_t*)(I2C3_BASE_ADDR))


#define 		I2C1_FREQ_4MHZ()		(I2C1->I2C_CR2 |= (0b000100 << 0))
#define 		I2C1_FREQ_16MHZ()		(I2C1->I2C_CR2 |= (0b010000 << 0))
#define 		I2C1_FREQ_45MHZ()		(I2C1->I2C_CR2 |= (0b101101 << 0))
#define 		I2C1_FREQ_50MHZ()		(I2C1->I2C_CR2 |= (0b110010 << 0))

#define 		I2C1_FREQ_SCL_100MHZ()		(I2C1->I2C_CCR = 80) // I2C1_FREQ_16MHZ İÇİN
#define 		I2C1_FREQ_SCL_100MHZ_225()	(I2C1->I2C_CCR = 225)
#define			I2C1_RISE_TIME_17()		(I2C1->I2C_TRISE = 17)
#define			I2C1_RISE_TIME_46()		(I2C1->I2C_TRISE = 46)

#define 		I2C1_ENABLE()			(I2C1->I2C_CR1 |= (1u << 0))
#define 		I2C1_DISABLE()			(I2C1->I2C_CR1 &= ~(1u << 0))

#define 		I2C1_ACK_ENABLE()		(I2C1->I2C_CR1 |= (1u << 10))
#define 		I2C1_ACK_DISABLE()		(I2C1->I2C_CR1 &= ~(1u << 10))

#define 		I2C1_START_GENERATION() (I2C1->I2C_CR1 |= (1u << 8))
#define 		I2C1_START_READ() 		(I2C1->I2C_SR1 & (1u << 0))
#define 		I2C1_STOP_GENERATION()	(I2C1->I2C_CR1 |= (1u << 9))

#define 		I2C1_SR1_TXE_READ()		((I2C1->I2C_SR1 & (1u << 7)) ? 1 : 0 )
#define 		I2C1_SR1_TXE_SET()		(I2C1->I2C_SR1 |= (1u << 7))
#define 		I2C1_SR1_BTF_READ()		((I2C1->I2C_SR1 & (1u << 2)) ? 1 : 0 )

#define			I2C1_SR1_READ()			(I2C1->I2C_SR1)
#define			I2C1_SR2_READ()			(I2C1->I2C_SR2)

void i2cInit(void);
void i2cWrite(uint8_t data);
void i2cAddress(uint8_t address);
void i2cStop(void);
void i2cWriteMulti(uint8_t* data, uint8_t size);
void i2cStart(void);
#endif /* I2C_H_ */
