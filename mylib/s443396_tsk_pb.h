/**
 **************************************************************
 * @file mylib/s4433963_tsk_pb.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief Pushbutton Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_PB_H
#define S443396_TSK_PB_H

/* Include mylib task files where necessary */
#include "s443396_reg_pb.h"
#include "s443396_tsk_CAG_keypad_grid.h"
#include "s443396_tsk_CAG_keypad_mnemonic.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* Define which mode the keypad is in */
#define KEYPAD_GRID 0
#define KEYPAD_MNEMONIC 1

/* Declare the semaphore handle for the push button */
SemaphoreHandle_t s443396pushButtonSemaphore;

/* Define task handle for the onboard pushbutton */
TaskHandle_t s443396pushButtonTask;

/* Define the user push button stack size and priority */
#define PB_PRIORITY (tskIDLE_PRIORITY + 2)
#define PB_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

/* Internal functions define explicitly */
void push_button_tsk(void);

/* External functions defined explicitly */
extern void s443396_push_button_tsk_init(void);

#endif