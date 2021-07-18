/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_joystick.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG joystick Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_CAG_JOYSTICK_H
#define S443396_TSK_CAG_JOYSTICK_H

/* Include mylib task files where necessary */
#include "s443396_reg_joystick.h"
#include "s443396_tsk_joystick.h"
#include "s443396_tsk_CAG_display.h"
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

/* Define task handle for the CAG joystick */
TaskHandle_t s443396joystickCAGTask;

/* Define the CAG joystick stack size and priority */
#define CAG_JOYSTICK_PRIORITY (tskIDLE_PRIORITY + 2)
#define CAG_JOYSTICK_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)

/* Internal functions define explicitly */
void CAG_joystick_task(void);

/* External functions defined explicitly */
extern void s443396_tsk_CAG_joystick_deinit(void);
extern void s443396_tsk_CAG_joystick_init(void);

#endif