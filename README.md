# Freq_DutyCycle_Calculation-via-INPUT_CAPTURE_MODE
Board: STM32F407G-DISC1                          
IDE: STMCubeIDE                                                              

Let's assume, the PWM signal generated by TIMER3 configurated  in the context is external                                
and we want to find out its duty cycle and frequency.                                                    
We assume period is constant. In this case, we only need to calculate the frequency once.                                            
                                                                                                    
We change the duty cycle with a potentiometer dynamically to observe changes and control the accuracy.                
we calculate the duty cycle  and the frequency using TIMER5 with input capture mode and monitor them via LCD.                                      

As you can guess, input channel PA1 connected to PB4 PWM output.                                        
Also, in the output of the PWM PB4 we have a LED to obsorve changes easier.                                
At the same time, we control the led brightness with the PWM signal.

After detecting two consecutive rising edge, we can calculate the frequency.                                
Then, we need to capture rising edge and the following falling edge to calculate the duty cycle.                    
That means we need to change the polarity, we do that inside the handler.                
Also, we reset CNT register inside the isr to avoid miscalculation at high duty cycles.                                                  
It makes capturing smoother.                                    
We change the polarity of the edge inside the handler.                    
