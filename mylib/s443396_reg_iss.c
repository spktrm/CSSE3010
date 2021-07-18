/**
**************************************************************
* @file mylib/s4433963_reg_pb.c
* @author Joseph Twin - 44339638
* @date 03032021
* @brief Input Signal Synchroniser Mylib Register Driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s443396_reg_iss_init() - Initialise the input signal synchroniser counters
* s443396_reg_iss_synchroniser() - Synchronises the input signal
* s443396_reg_iss_eventcounter_read() - Returns the specified event counter
*value s443396_reg_iss_lasttimer_read() - Returns the specified last time value
* s443396_reg_iss_eventcounter_reset() - Reset the specified event counter value
* s443396_reg_iss_lasttimer_reset() - Reset the specified last time value
* s443396_reg_iss_delaytimer_ctrl() - Set the delay value for the specified
*source
***************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "s443396_reg_iss.h"
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
/* ---------------------------------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/
uint32_t iss_eventcounter_val[7];
uint32_t iss_lasttimer_val[7];
uint32_t iss_delay_val[7];
/* ---------------------------------------------------------------------------*/

/*
 * @brief Initialise the input signal synchroniser counters
 */
void s443396_reg_iss_init(void)
{
	for (int i = 0; i < 7; i++)
	{
		iss_eventcounter_val[i] = 0;
		iss_lasttimer_val[i] = 0;
		iss_delay_val[i] = 10;
	}
}

/*
 * @brief Synchronises the input signal
 */
void s443396_reg_iss_synchroniser(unsigned char signal_source_index)
{
	uint32_t timer_val = iss_lasttimer_val[signal_source_index];
	uint32_t timer_delay = iss_delay_val[signal_source_index];

	uint32_t tick = HAL_GetTick();

	if ((tick - timer_val) > timer_delay)
	{
		iss_eventcounter_val[signal_source_index] += 1;
		// debug_printf("event on signal source %u, total: %d\n\r", signal_source_index + 1, iss_eventcounter_val[signal_source_index]);
	}
	iss_lasttimer_val[signal_source_index] = tick;
}

/*
 * @brief Returns the specified event counter value
 */
uint32_t s443396_reg_iss_lasttimer_read(unsigned char signal_source_index)
{
	return iss_lasttimer_val[signal_source_index];
}

/*
 * @brief Returns the specified last time value
 */
uint32_t s443396_reg_iss_eventcounter_read(unsigned char signal_source_index)
{
	return iss_eventcounter_val[signal_source_index];
}

/*
 * @brief Reset the specified last time value
 */
void s443396_reg_iss_lasttimer_reset(unsigned char signal_source_index)
{
	iss_lasttimer_val[signal_source_index] = 0;
}

/*
 * @brief Reset the specified event counter value
 */
void s443396_reg_iss_eventcounter_reset(unsigned char signal_source_index)
{
	iss_eventcounter_val[signal_source_index] = 0;
}

/*
 * @brief Set the delay value for the specified source
 */
void s443396_reg_iss_delaytimer_ctrl(unsigned char signal_source_index, int delay_value)
{
	iss_delay_val[signal_source_index] = delay_value;
}
