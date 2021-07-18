/**
 **************************************************************
 * @file mylib/s4433963_tsk_joystick.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief Joystick Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_JOYSTICK_H
#define S443396_TSK_JOYSTICK_H

/* Include mylib task files where necessary */
#include "s443396_reg_joystick.h"
#include "s443396_reg_iss.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* Define event bit positions for Joystick X Axis */
#define EVT_JOYSTICK_X_0 1 << 0
#define EVT_JOYSTICK_X_1 1 << 1
#define EVT_JOYSTICK_X_2 1 << 2
#define EVT_JOYSTICK_X_3 1 << 3
#define EVT_JOYSTICK_X_4 1 << 4

/* Define Joystick X Axis state as logical OR of event bit positions */
#define JOYSTICK_X_STATE EVT_JOYSTICK_X_0 | EVT_JOYSTICK_X_1 | EVT_JOYSTICK_X_2 | EVT_JOYSTICK_X_3 | EVT_JOYSTICK_X_4

/* Define event bit positions for Joystick Y Axis */
#define EVT_JOYSTICK_Y_0 1 << 0
#define EVT_JOYSTICK_Y_1 1 << 1
#define EVT_JOYSTICK_Y_2 1 << 2
#define EVT_JOYSTICK_Y_3 1 << 3
#define EVT_JOYSTICK_Y_4 1 << 4

/* Define Joystick Y Axis state as logical OR of event bit positions */
#define JOYSTICK_Y_STATE EVT_JOYSTICK_Y_0 | EVT_JOYSTICK_Y_1 | EVT_JOYSTICK_Y_2 | EVT_JOYSTICK_Y_3 | EVT_JOYSTICK_Y_4

/* Define event bit positions for Joystick Z Axis */
#define EVT_JOYSTICK_Z 1 << 0

/* Define Joystick Z Axis state as the only event bit that exists for it */
#define JOYSTICK_Z_STATE EVT_JOYSTICK_Z

/* Define the joystick stack size and priority */
#define JOYSTICK_PRIORITY (tskIDLE_PRIORITY + 2)
#define JOYSTICK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

/* Define Semaphore handle for the joystick Z axis */
SemaphoreHandle_t s443396joystickZAxisButtonSemaphore;

/* Define event group handles for the joystick axis'*/
EventGroupHandle_t s443396joystickXEventGroup;
EventGroupHandle_t s443396joystickYEventGroup;
EventGroupHandle_t s443396joystickZEventGroup;

/* Define task handle for the joystick */
TaskHandle_t s443396joystickTask;

/* Internal functions define explicitly */
void joystick_task(void);

/* External functions defined explicitly */
extern void s443396_tsk_joystick_init(void);

#endif
