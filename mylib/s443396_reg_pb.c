/**
**************************************************************
* @file mylib/s4433963_reg_pb.c
* @author Joseph Twin - 44339638
* @date 03032021
* @brief Pushbutton Mylib Register Driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s443396_reg_pb_on_init() - Enabled the onboard pushbutton source
* s443396_reg_pb_on_isr() - Onboard Pushbutton Interrupt service routine
* s443396_reg_pb_iss_init() - Enabled the ISS pin sources
* s443396_reg_pb_iss_isr() - ISS pin source Interrupt service routine
***************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "s443396_reg_pb.h"
#include "board.h"
#include "processor_hal.h"
#include "s443396_reg_iss.h"

void s443396_reg_pb_on_init(void);
void s443396_reg_pb_on_isr(void);
void s443396_reg_pb_iss_init(void);
void s443396_reg_pb_iss_isr(void);

/*
 * @brief Enabled the onboard pushbutton source
 */
void s443396_reg_pb_on_init(void)
{
	// init structure for variables
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIOC Clock for push button
	BRD_USER_BUTTON_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = BRD_USER_BUTTON_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);
}

/*
 * @brief Onboard Pushbutton Interrupt service routine
 */
void s443396_reg_pb_on_isr(void)
{
	// Check if the pushbutton pin was pressed.
	s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_2);
}

/*
 * @brief Enabled the ISS pin sources
 */
void s443396_reg_pb_iss_init(void)
{
	// init structure for variables
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIOC Clock for A0 pin
	__BRD_A0_GPIO_CLK();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = BRD_A0_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(BRD_A0_EXTI_IRQ, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_A0_EXTI_IRQ);
}

/*
 * @brief ISS pin source Interrupt service routine
 */
void s443396_reg_pb_iss_isr(void)
{
	// Check if external A0 pin is connected.
	s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_1);
}
