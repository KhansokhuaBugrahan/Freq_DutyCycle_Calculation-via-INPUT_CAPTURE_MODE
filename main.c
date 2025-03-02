 /**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include <stdbool.h>
#include "gpio.h"
#include "utility.h"
#include "armcortexm4.h"
#include "timer.h"
#include "adc.h"
#include "i2c.h"
#include "i2clcd.h"

volatile uint32_t firstRise, secondRise;
volatile bool captureDone = false;
volatile bool captureDuty = false;
volatile bool flagDutyCycle = false;
volatile bool flagPolarity = false;

void TIM5_IRQHandler(void)
{
    if (TIMER5->TIMx_SR & (1 << 2))  // Check if capture event occurred
    {
    	TIMER5->TIMx_SR = ~(1 << 2);  // Clear interrupt flag


    	 if(TIMER5->TIMx_SR & (1u << 10)) // overcapture detection
	     {
		   TIMER5->TIMx_SR = ~(1u << 10);

		   return;
	     }

    	 static bool flag = false;
        if (!flag)
        {
            firstRise = TIMER5->TIMx_CCR2;
            flag = true;
		    if(flagDutyCycle)
			{
				TIMER5->TIMx_CCER |=  (1u << 5);
			}

        }
        else
        {

            secondRise = TIMER5->TIMx_CCR2;
            TIMER5->TIMx_CNT = 0;
            if(flagDutyCycle)
			{
				TIMER5->TIMx_CCER &= ~(1u << 5);
			}
            if(!flagDutyCycle)
            	captureDone = true;
            else
            	captureDuty = true;
            flag = false;

        }


    }
}
int main(void)
{
					fpuInit();
					sysTickInit();
					GPIO_Handle_t I2C1_SCL_PB6 = {  .PORTNAME = GPIOB,
													.PINCONF.PIN = GPIO_PIN_6,
													.PINCONF.MODE = GPIO_MODE_ALTARNATE,
													.PINCONF.OTYPE = GPIO_OTYPE_OD,
													.PINCONF.OSPEED = GPIO_OSPEED_VHIGH,
													.PINCONF.PUPD = GPIO_PUPD_PU,
													.PINCONF.AF = AF4

												 };
					GPIO_Handle_t I2C1_SDA_PB7 = {  .PORTNAME = GPIOB,
													.PINCONF.PIN = GPIO_PIN_7,
													.PINCONF.MODE = GPIO_MODE_ALTARNATE,
													.PINCONF.OTYPE = GPIO_OTYPE_OD,
													.PINCONF.OSPEED = GPIO_OSPEED_VHIGH,
													.PINCONF.PUPD = GPIO_PUPD_PU,
													.PINCONF.AF = AF4
												 };
				 gpioInit(&I2C1_SCL_PB6);
				 gpioInit(&I2C1_SDA_PB7);

		           GPIO_Handle_t ADC_DIMMER_POT_PA0 = {  .PORTNAME = GPIOA,
														.PINCONF.PIN = GPIO_PIN_0,
														.PINCONF.MODE = GPIO_MODE_ANALOG,
														.PINCONF.OTYPE = GPIO_OTYPE_PP,
														.PINCONF.OSPEED = GPIO_OSPEED_HIGH,
														.PINCONF.PUPD = GPIO_PUPD_NO,
														.PINCONF.AF = AFNO
		           	   	   	   	   	   	   	   	   	   };


				gpioInit(&ADC_DIMMER_POT_PA0);




				adc1Configuration(ADC_RESOLUTION_12, ADC_MODE_CONTINUOUS_CONV , ADC_CHANNEL_0, ADC_SAMPLING_28_CYCLE );
				adc1ChannelSequence(ADC_CHANNEL_0, ADC_CHANNEL_SEQUENCE_1);
				adc1SequenceLength(ADC_CONVERSION_LENGTH_1);




			   GPIO_Handle_t PWM_TIMER3_CH1_PB4 = {  .PORTNAME = GPIOB,
													.PINCONF.PIN = GPIO_PIN_4,
													.PINCONF.MODE = GPIO_MODE_ALTARNATE,
													.PINCONF.OTYPE = GPIO_OTYPE_PP,
													.PINCONF.OSPEED = GPIO_OSPEED_HIGH,
													.PINCONF.PUPD = GPIO_PUPD_PU,
													.PINCONF.AF = AF2
			   	   	   	   	   	   	   	   	   	   };

			   GPIO_Handle_t CCP_TIMER5_CH2_PA1 = {  .PORTNAME = GPIOA,
													.PINCONF.PIN = GPIO_PIN_1,
													.PINCONF.MODE = GPIO_MODE_ALTARNATE,
													.PINCONF.OTYPE = GPIO_OTYPE_PP,
													.PINCONF.OSPEED = GPIO_OSPEED_HIGH,
													.PINCONF.PUPD = GPIO_PUPD_PD ,
													.PINCONF.AF = AF2
												   };


				gpioInit(&PWM_TIMER3_CH1_PB4);
				gpioInit(&CCP_TIMER5_CH2_PA1);


				timerxConfig(TIMER3, 160, 100);   	// (100kHz) 10us counting, 1ms PWM period	1kHz PWM freq
				timer3PwmEnable(DUTY_CYCLE_70);
				timerxPeripheralEnable(TIMER3);



				timerxConfig(TIMER5, 160, 0xFFFF); // 1 tick 10us
				timerxPeripheralEnable(TIMER5);
				timer5InterruptEnable();

				timerxCaptureEnable(TIMER5, CC2_CH2, CCx_CAPTURE_TI2, CCx_RISING);



				adc1Init();
				i2cInit();
				lcdInit();



				uint32_t periodTicks = 0;
				float periodMs = 0;
				float frequencykHz = 0;
				float dutyCycleRatio = 0;
				char buffer[7];
				float lastDutyCycleRatio = 0;
				float epsilon = 0.1;
				while (1)
				{

				    if (captureDone)
				    {
				    	if(secondRise >= firstRise)
				    		periodTicks = secondRise - firstRise;
				    	else
				    		periodTicks =  (0xFFFF - firstRise) + secondRise;
				        periodMs = periodTicks  / 100.0f; // 10µs per tick
				        frequencykHz = 1.0 / periodMs;

				       captureDone = false; // Reset flag
				       flagDutyCycle = true;
						sprintf(buffer,"%.2f",frequencykHz);
						lcdPutCursor(0, 0);
						lcdSendString ("Freq(kHz): ");
						lcdSendString (buffer);
						lcdPutCursor(1, 0);
						lcdSendString ("  Duty(%):");
				    }
				    if (captureDuty)
					{

						if(secondRise >= firstRise)
							periodTicks = secondRise - firstRise;
						else
							periodTicks =  (0xFFFF - firstRise) + secondRise;

						dutyCycleRatio = ( (float)periodTicks / periodMs );

					   captureDuty = false;
					   while(!(ADC_1->ADC_SR & (1u << 1)))
						; // null statement , wait for data to be ready before reading it.
					   uint16_t adcVal = adc1ReadValue();

					uint16_t dutyCycle = (adcVal * 100) / 4095;

					if( dutyCycle != 0 && dutyCycle !=100)
					TIMER3->TIMx_CCR1 = dutyCycle;


					if ((lastDutyCycleRatio - dutyCycleRatio) > epsilon || (dutyCycleRatio - lastDutyCycleRatio) > epsilon)
					{
						memset(buffer, 0, sizeof(buffer));

						lcdPutCursor(1, 11);
						sprintf(buffer, "%.1f", dutyCycleRatio);
						lcdSendString(buffer);
						lcdSendString(" ");
					}
					lastDutyCycleRatio = dutyCycleRatio;

					}
				}
}
