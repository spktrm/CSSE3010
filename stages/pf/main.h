/**
 **************************************************************
 * @file stages/pf/main.h
 * @author Joseph Twin - 44339638
 * @date 24052021
 * @brief project header file includes and explicit function dec
 ***************************************************************
 */

#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
/* mylib tsk files -----------------------------------------------------------*/
#include "s443396_tsk_CAG_simulator.h"
#include "s443396_tsk_CAG_display.h"
#include "s443396_tsk_CAG_keypad_grid.h"
#include "s443396_tsk_CAG_keypad_mnemonic.h"
#include "s443396_tsk_CAG_joystick.h"
/* mylib reg files -----------------------------------------------------------*/
#include "s443396_tsk_pb.h"
/* FreeRTOS libraries --------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ---------------------------------------------------------------------------*/

/* Hardware init */
static void Hardware_init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#endif
