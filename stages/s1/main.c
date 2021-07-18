/**
 **************************************************************
 * @file stages/s1/main.c
 * @author Joseph Twin - 44339638
 * @date 03032021
 * @brief stage 1 main code
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * sxxxxxx_mylib_template_init() - intialise mylib template driver
 * sxxxxxx_mylib_template_set() - set mylib template driver
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "s443396_reg_iss.h"
#include "s443396_reg_lta1000g.h"
#include "s443396_reg_pb.h"
/* ---------------------------------------------------------------------------*/

/* Parameters ----------------------------------------------------------------*/
#define RIGHT 0
#define LEFT 1
#define MODE_MPD 0
#define MODE_EVENT_COUNTER 1
#define MAX_TIME_MODE_EVENT_COUNTER 5000
#define MIN_TIME_TWO_BTN_EVENTS 2000
/* ---------------------------------------------------------------------------*/

int mode = MODE_MPD;
int event_counter = 0;
int mpd = 768;		   // inital state of mpd
int direction = RIGHT; // init direciton left

uint32_t event_counter_display_timer_start = 0;
uint32_t last_button_press_timer_start = 0;

void check_mode_one(void);
void display_event_counter(void);
void ext_btn_event(void);
void int_btn_event(void);

/*
 * @brief  Main program - flashes onboard LEDs
 */
int main(void)
{
	HAL_Init(); // Initalise Board
	BRD_debuguart_init();

	s443396_reg_lta1000g_init(); // Initalise lta1000g
	s443396_reg_pb_on_init();	 // init onboard push button
	s443396_reg_pb_iss_init();	 // init external button source

	// init iss and delay timer to be 50ms
	s443396_reg_iss_init();
	s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_1, 50);
	s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_2, 50);

	// init prev btn event counters so to compare in main loop
	uint32_t prev_ext_btn_event_count = 0;
	uint32_t prev_int_btn_event_count = 0;

	// init seg to display the mpd
	int seg = mpd;

	// initially write the display
	s443396_reg_lta1000g_write(seg);

	// Main processing loop
	while (1)
	{
		// check if more than two on board button events have occured in the
		// last two seconds
		check_mode_one();

		if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_1) !=
			prev_ext_btn_event_count)
		{
			ext_btn_event();
		}

		if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_2) !=
			prev_int_btn_event_count)
		{
			int_btn_event();
		}

		// reset counter once greater than 10
		if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_1) >= 10)
		{
			s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_1);
		}

		// reset counter once greater than 10
		if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_2) >= 10)
		{
			s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_2);
		}

		// update the event counter even if not displayed
		event_counter =
			(s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_1) << 5) | s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_2);

		// display mpd if mode is 0, display event counter bar if mode is 1
		if (mode == MODE_MPD)
		{
			seg = mpd;
		}
		else if (mode == MODE_EVENT_COUNTER)
		{
			seg = event_counter;
		}

		// update the display
		s443396_reg_lta1000g_write(seg);

		// set previous event count to compare for next loop
		prev_ext_btn_event_count =
			s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_1);
		prev_int_btn_event_count =
			s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_2);
		HAL_Delay(.1); // Delay for 1ms
	}

	return 0;
}

/*
 * @brief external interrupt callback
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BRD_USER_BUTTON_PIN)
	{							 // Check if the pushbutton pin was pressed
		s443396_reg_pb_on_isr(); // Sync on-board button source
	}
	else if (GPIO_Pin == BRD_A0_PIN)
	{							  // Check if A0 pin was toggled
		s443396_reg_pb_iss_isr(); // Sync A0 pin
	}
}

/*
 * @brief external interrupt handler for on board push button
 */
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BRD_USER_BUTTON_PIN);
}

/*
 * @brief external interrupt handler for A0 pin
 */
void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BRD_A0_PIN);
}

/*
 * @brief check event counter time limit
 */
void check_mode_one(void)
{
	if ((mode != MODE_MPD) &&
		((HAL_GetTick() - event_counter_display_timer_start) >= MAX_TIME_MODE_EVENT_COUNTER))
	{
		debug_printf("switching to mpd mode\n\r");
		mode = MODE_MPD;
	}
}

/*
 * @brief show event counter on LED bar
 */
void display_event_counter(void)
{
	debug_printf("switching to event counter mode\n\r");

	event_counter_display_timer_start = HAL_GetTick();
	mode = MODE_EVENT_COUNTER;
}

/*
 * @brief process for if A0 pin is toggled
 */
void ext_btn_event(void)
{
	if (direction == RIGHT)
	{ // if the direction is right
		mpd = (mpd >> 1);
		if (mpd == 1)
		{ // loop around
			mpd = 768;
		}
	}
	else
	{ // if the direction is left
		mpd = (mpd << 1);
		if (mpd == 1536)
		{ // loop around
			mpd = 3;
		}
	}
}

/*
 * @brief process for if on board push button is toggled
 */
void int_btn_event(void)
{
	direction ^= 1; // toggle the direction

	// state the direction of the mpd
	if (direction == RIGHT)
	{
		debug_printf("mpd now moves to the right\n\r");
	}
	else if (direction == LEFT)
	{
		debug_printf("mpd now moves to the left\n\r");
	}

	if (mode != MODE_EVENT_COUNTER)
	{ // if display is not alread showing events and there have been more
		// than two button presses in the last 2 seconds

		if ((HAL_GetTick() - last_button_press_timer_start) <= MIN_TIME_TWO_BTN_EVENTS)
		{
			display_event_counter();
		}
	}

	// keep track of the last time the on-board button was pressed
	last_button_press_timer_start =
		s443396_reg_iss_lasttimer_read(S443396_REG_ISS_SOURCE_2);
}
