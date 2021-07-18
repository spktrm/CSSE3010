/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_grid.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad grid Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_CAG_KEYPAD_GRID_H
#define S443396_TSK_CAG_KEYPAD_GRID_H

/* Include mylib task files where necessary */
#include "s443396_reg_keypad.h"
#include "s443396_tsk_keypad.h"
#include "s443396_tsk_CAG_simulator.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* Define task handle for the keypad in grid mode */
TaskHandle_t s443396keypadGridTask;

/* Define queue handles for sending the selected subgrid to the CAG simulator and CAG display */
QueueHandle_t s443396keypadSubgridSimulatorQueue;
QueueHandle_t s443396keypadSubgridDisplayQueue;

/* Define structure for sending subgrid x and y values to the CAG simulator */
struct subgridMessage
{
    int subgridX;
    int subgridY;
};

/* Define the keypad grid stack size and priority */
#define CAG_KEYPAD_GRID_PRIORITY (tskIDLE_PRIORITY + 2)
#define CAG_KEYPAD_GRID_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)

/* Internal functions define explicitly */
void keypad_grid_task(void);

/* External functions defined explicitly */
extern void s443396_tsk_CAG_keypad_grid_init(void);

#endif