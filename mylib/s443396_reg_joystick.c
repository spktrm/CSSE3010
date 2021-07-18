/**
 **************************************************************
 * @file mylib/s4433963_reg_joystick.c
 * @author Joseph Twin - 44339638
 * @date 19032021
 * @brief Joystick Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s4433963_reg_joystick_init() - Initialise GPIO pins and ADC
 * joystick_readxy() - Internal generic function to read X or Y joystick value.
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s443396_reg_joystick.h"
#include "debug_printf.h"
/* ---------------------------------------------------------------------------*/

void s4433963_reg_joystick_init(void) {

    // Initalise PC3/A2 as an Analog input.
	// Enable GPIOC Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 3);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (3 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (3 * 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 3) on EXTICR1.
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
	SYSCFG->EXTICR[0] &= ~(0x000F);

	EXTI->RTSR |= EXTI_RTSR_TR3;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR3;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM3;		//Enable external interrupt

	HAL_NVIC_SetPriority(BRD_A2_EXTI_IRQ, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_A2_EXTI_IRQ);

    // PA3/A0 Pin ADC1 Joystick X
    __GPIOA_CLK_ENABLE();
    GPIOA->MODER |= (0x03 << (3 * 2));			//Set bits for Analog input mode
  	GPIOA->OSPEEDR &= ~(0x03<<(3 * 2));
	GPIOA->OSPEEDR |= 0x02<<(3 * 2);  			// Fast speed
	GPIOA->PUPDR &= ~(0x03 << (3 * 2));			//Clear bits for no push/pull
    
    __ADC1_CLK_ENABLE();						//Enable ADC1 clock

	AdcHandleX.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	AdcHandleX.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandleX.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandleX.Init.ScanConvMode          = DISABLE;
	AdcHandleX.Init.ContinuousConvMode    = DISABLE;
	AdcHandleX.Init.DiscontinuousConvMode = DISABLE;
	AdcHandleX.Init.NbrOfDiscConversion   = 0;
	AdcHandleX.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandleX.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandleX.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandleX.Init.NbrOfConversion       = 1;
	AdcHandleX.Init.DMAContinuousRequests = DISABLE;
	AdcHandleX.Init.EOCSelection          = DISABLE;

    HAL_ADC_Init(&AdcHandleX);		//Initialise ADC

    // Configure ADC Channel
	AdcChanConfigX.Channel = ADC_CHANNEL_3;					//PA3 has Analog Channel 3 connected
	AdcChanConfigX.Rank         = 1;
	AdcChanConfigX.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigX.Offset       = 0;

    HAL_ADC_ConfigChannel(&AdcHandleX, &AdcChanConfigX);		//Initialise ADC Channel

    // PC0/A1 Pin ADC2 Joystick Y
    // __GPIOC_CLK_ENABLE();
  	GPIOC->MODER |= (0x03 << (0 * 2));			//Set bits for Analog input mode
  	GPIOC->OSPEEDR &= ~(0x03<<(0 * 2));
	GPIOC->OSPEEDR |= 0x02<<(0 * 2);  			// Fast speed
	GPIOC->PUPDR &= ~(0x03 << (0 * 2));			//Clear bits for no push/pull
	
    __ADC2_CLK_ENABLE();						//Enable ADC2 clock

    AdcHandleY.Instance = (ADC_TypeDef *)(ADC2_BASE);						//Use ADC2 (For PC0)
	AdcHandleY.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandleY.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandleY.Init.ScanConvMode          = DISABLE;
	AdcHandleY.Init.ContinuousConvMode    = DISABLE;
	AdcHandleY.Init.DiscontinuousConvMode = DISABLE;
	AdcHandleY.Init.NbrOfDiscConversion   = 0;
	AdcHandleY.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandleY.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandleY.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandleY.Init.NbrOfConversion       = 1;
	AdcHandleY.Init.DMAContinuousRequests = DISABLE;
	AdcHandleY.Init.EOCSelection          = DISABLE;

    HAL_ADC_Init(&AdcHandleY);		//Initialise ADC

    // Configure ADC Channel
	AdcChanConfigY.Channel = ADC_CHANNEL_10;					//PA3 has Analog Channel 3 connected
	AdcChanConfigY.Rank         = 1;
	AdcChanConfigY.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigY.Offset       = 0;

    HAL_ADC_ConfigChannel(&AdcHandleY, &AdcChanConfigY);		//Initialise ADC Channel

}

int joystick_readxy(ADC_HandleTypeDef *ADC_Handler) {

    // got this from your example code dr de souza
    HAL_ADC_Start(ADC_Handler);

    while (HAL_ADC_PollForConversion(ADC_Handler, 10) != HAL_OK);

    int val = HAL_ADC_GetValue(ADC_Handler);

    return val;
}

