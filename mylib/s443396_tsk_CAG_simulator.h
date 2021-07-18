/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_simulator.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG simulator Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_CAG_SIMULATOR_H
#define S443396_TSK_CAG_SIMULATOR_H

/* Include mylib task files where necessary */
#include "s443396_tsk_CAG_display.h"
#include "s443396_tsk_CAG_joystick.h"
#include "s443396_tsk_CAG_keypad_grid.h"
#include "s443396_tsk_CAG_keypad_mnemonic.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Define the task handle for the CAG Simulator */
TaskHandle_t s443396simulatorTask;

/* Define the semaphore handle for update the CAG display */
SemaphoreHandle_t s443396displayUpdateSemaphore;

/* Define caMessage structure for recieving information from the mnemonic keypad input  */
typedef struct caMessage
{
    int type;   /* Type - Cell, or Lifeform */
    int cell_x; /* Cell/Lifeform x position */
    int cell_y; /* Cell/Lifeform y position */
} caMessage_t;

#define GRID_HEIGHT 15 /* Define maximum grid height */
#define GRID_WIDTH 30  /* Define maximum grid width */

/* Globally define the grid that contains cells, for use within the CAG simulator and CAG display */
unsigned char grid[GRID_HEIGHT][GRID_WIDTH];

/* Define the CAG simulator stack size and priority */
#define CAG_SIMULATOR_PRIORITY (tskIDLE_PRIORITY + 2)
#define CAG_SIMULATOR_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)

/* Internal functions define explicitly */
void init_simulation(void);
void init_grid(void);
void toggle_cell(int x_offset, int y_offset);
void update_grid(void);
void update_display_semaphore(void);
void update_grid_mnemonic_life(int cell_x, int cell_y, int type);
void clear_grid(void);
void start_simulation(void);
void stop_simulation(void);
void debug_print_grid(void);
void set_update_delay(TickType_t new_delay);

/* External functions defined explicitly */
extern void s443396_tsk_CAG_simulator_deinit(void);
extern void s443396_tsk_CAG_simulator_init(void);

#endif
