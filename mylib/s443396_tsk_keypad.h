/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_grid.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad grid Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_KEYPAD_H
#define S443396_TSK_KEYPAD_H

/* Include mylib task files where necessary */
#include "s443396_reg_keypad.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* Define task handle for the keypad */
TaskHandle_t s443396keypadTask;

/* Define event group handle for the keypad buttons */
EventGroupHandle_t s443396keypadEventGroup;

/* Define event bit positions for keypad keys */
#define EVT_BTN_0 1 << 0
#define EVT_BTN_1 1 << 1
#define EVT_BTN_2 1 << 2
#define EVT_BTN_3 1 << 3
#define EVT_BTN_4 1 << 4
#define EVT_BTN_5 1 << 5
#define EVT_BTN_6 1 << 6
#define EVT_BTN_7 1 << 7
#define EVT_BTN_8 1 << 8
#define EVT_BTN_9 1 << 9
#define EVT_BTN_A 1 << 10
#define EVT_BTN_B 1 << 11
#define EVT_BTN_C 1 << 12
#define EVT_BTN_D 1 << 13
#define EVT_BTN_E 1 << 14
#define EVT_BTN_F 1 << 15

/* Define keypad state as logical OR of event bit positions */
#define KEYPAD_GRID_STATE EVT_BTN_0 | EVT_BTN_1 | EVT_BTN_2 | EVT_BTN_3 | EVT_BTN_4 | EVT_BTN_5 | EVT_BTN_6 | EVT_BTN_7 | EVT_BTN_8 | EVT_BTN_9 | EVT_BTN_A | EVT_BTN_B | EVT_BTN_C | EVT_BTN_D | EVT_BTN_E | EVT_BTN_F

/* Define the keypad stack size and priority */
#define KEYPAD_PRIORITY (tskIDLE_PRIORITY + 2)
#define KEYPAD_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)

/* Internal functions define explicitly */
void keypad_task(void);

/* External functions defined explicitly */
extern void s443396_tsk_keypad_init(void);

#endif