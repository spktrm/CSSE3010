/**
 **************************************************************
 * @file mylib/s4433963_reg_pantilt.c
 * @author Joseph Twin - 44339638
 * @date 19032021
 * @brief PanTilt Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s443396_reg_pan_tilt_init() - initialise servo (GPIO, PWM, etc)
 * s4433963_reg_pantilt_write() - Generic function for writing an angle
 * s443396_pantilt_angle_read() - Read the current pan servo angle
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s443396_reg_pantilt.h"
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
/* ---------------------------------------------------------------------------*/

#define TIMER_RUNNING_FREQ  				50000           				//Frequency (in Hz)
#define PWM_PULSE_WIDTH_TICKS				2000 					//((PWM_PULSE_WIDTH_SECONDS)/(1/TIMER_RUNNING_FREQ))		//Period in timer ticks

void s443396_reg_pantilt_init(void)
{

	__GPIOE_CLK_ENABLE();

    //D6 PEs9
    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (9 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (9 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (9 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << (1 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (1 * 4));	//Set Alternate Function for pin (lower ARF register) 

	// Timer 3 clock enable
	__TIM1_CLK_ENABLE();

	/* Compute the prescaler value
	   Set the clock prescaler to 50kHz
	   SystemCoreClock is the system clock frequency */
	TIM1->PSC = ((SystemCoreClock / 2) / TIMER_RUNNING_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM1->CR1 &= ~TIM_CR1_DIR; 

	TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 		//Set pulse period to ~1s 
	TIM1->CCR1 = 150;	//Set pulse width to 0% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 4); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC1PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC1E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC1NE; 	// Clear CC2NE Bit for active high output
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter
	
    //D5 PE11
    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (11 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (11 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (11 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << (3 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (3 * 4));	//Set Alternate Function for pin (lower ARF register) 

	TIM1->CCR2 = 150;	//Set pulse width to 0% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter


}

void s443396_pantilt_angle_write(int type, int angle)
{
    
	if (angle < -85) {
		angle = -85;
	}

	if (angle > 85) {
		angle = 85;
	}

	uint32_t ticks;
	ticks = 0.5555556 * angle + 150;

    switch (type)
    {
    case 0:
        ticks += S443396_REG_PANTILT_PAN_ZERO_CAL_OFFSET;
        TIM1->CCR1 = ticks;
        debug_printf("%d\r\n", ticks);
        break;
    case 1:
        ticks += S443396_REG_PANTILT_TILT_ZERO_CAL_OFFSET;
        TIM1->CCR2 = ticks;
        debug_printf("%d\r\n", ticks);
        break;
    }
}

uint16_t s443396_pantilt_angle_read(int type)
{	

	switch (type)
    {
    case 0:
        return TIM1->CCR1;
        break;
    case 1:
        return TIM1->CCR2;
        break;
    }
}
