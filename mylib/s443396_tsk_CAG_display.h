/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_display.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG display Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_CAG_DISPLAY_H
#define S443396_TSK_CAG_DISPLAY_H

/* Include mylib task files where necessary */
#include "s443396_reg_oled.h"
#include "s443396_reg_lta1000g.h"
#include "s443396_tsk_CAG_simulator.h"
#include "s443396_tsk_CAG_keypad_grid.h"
#include "s443396_tsk_CAG_joystick.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Define task handle for the CAG display */
TaskHandle_t s443396displayTask;

/* Define the CAG display stack size and priority */
#define CAG_DISPLAY_PRIORITY (tskIDLE_PRIORITY + 2)
#define CAG_DISPLAY_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)

/* Define mapping of grid size to actual cell size in pixels on display */
#define DISPLAY_CELL_SIZE 2

/* Define the size of subgrid */
#define SUBGRID_SIZE 3

/* Internal functions define explicitly */
void draw_dislay(void);
void set_contrast(int value);
void s443396_tsk_CAG_display(void);

/* External functions defined explicitly */
extern void s443396_tsk_CAG_display_init(void);

#endif