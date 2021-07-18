/**
**************************************************************
* @file mylib/s4433963_reg_pb.c
* @author Joseph Twin - 44339638
* @date 03032021
* @brief LTA1000G LED Bar Display Mylib Register Driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s443396_reg_lta1000g_init() - Initialise LED Bar GPIO pins as outputs
* lta1000g_seg_set() - Set the value (0 or 1) for the corresponding LED Bar
* segment GPIO pin segment GPIO pin
* s443396_reg_lta1000g_write() - Write the LED Bar segments high or low
***************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "s443396_reg_lta1000g.h"

/*
 * @brief Initialise LED Bar GPIO pins as outputs
 */
void s443396_reg_lta1000g_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// Enable the GPIO A, B and C Clock
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

	GPIO_InitStructure.Pin = BRD_D16_PIN;
	HAL_GPIO_Init(BRD_D16_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D17_PIN;
	HAL_GPIO_Init(BRD_D17_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D18_PIN;
	HAL_GPIO_Init(BRD_D18_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D19_PIN;
	HAL_GPIO_Init(BRD_D19_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D20_PIN;
	HAL_GPIO_Init(BRD_D20_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D21_PIN;
	HAL_GPIO_Init(BRD_D21_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D22_PIN;
	HAL_GPIO_Init(BRD_D22_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D23_PIN;
	HAL_GPIO_Init(BRD_D23_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D24_PIN;
	HAL_GPIO_Init(BRD_D24_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = BRD_D25_PIN;
	HAL_GPIO_Init(BRD_D25_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * @brief Set the value (0 or 1) for the corresponding LED Bar segment GPIO pin
 */
void lta1000g_seg_set(int segment, unsigned char segment_value)
{
	if (segment == 0)
	{
		HAL_GPIO_WritePin(BRD_D16_GPIO_PORT, BRD_D16_PIN, segment_value);
	}
	else if (segment == 1)
	{
		HAL_GPIO_WritePin(BRD_D17_GPIO_PORT, BRD_D17_PIN, segment_value);
	}
	else if (segment == 2)
	{
		HAL_GPIO_WritePin(BRD_D18_GPIO_PORT, BRD_D18_PIN, segment_value);
	}
	else if (segment == 3)
	{
		HAL_GPIO_WritePin(BRD_D19_GPIO_PORT, BRD_D19_PIN, segment_value);
	}
	else if (segment == 4)
	{
		HAL_GPIO_WritePin(BRD_D20_GPIO_PORT, BRD_D20_PIN, segment_value);
	}
	else if (segment == 5)
	{
		HAL_GPIO_WritePin(BRD_D21_GPIO_PORT, BRD_D21_PIN, segment_value);
	}
	else if (segment == 6)
	{
		HAL_GPIO_WritePin(BRD_D22_GPIO_PORT, BRD_D22_PIN, segment_value);
	}
	else if (segment == 7)
	{
		HAL_GPIO_WritePin(BRD_D23_GPIO_PORT, BRD_D23_PIN, segment_value);
	}
	else if (segment == 8)
	{
		HAL_GPIO_WritePin(BRD_D24_GPIO_PORT, BRD_D24_PIN, segment_value);
	}
	else if (segment == 9)
	{
		HAL_GPIO_WritePin(BRD_D25_GPIO_PORT, BRD_D25_PIN, segment_value);
	}
}

/*
 * @brief Write the LED Bar segments high or low
 */
void s443396_reg_lta1000g_write(unsigned short value)
{
	int slider = 1;
	for (int i = 0; i <= 9; i++)
	{
		if (value & slider)
		{
			lta1000g_seg_set(i, 1);
		}
		else
		{
			lta1000g_seg_set(i, 0);
		}
		slider = (slider << 1);
	}
}
