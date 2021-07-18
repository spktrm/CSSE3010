/**
 **************************************************************
 * @file mylib/s4433963_tsk_oled.h
 * @author Joseph Twin - 44339638
 * @date 26042021
 * @brief OLED Task MyLib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_OLED_H
#define S443396_TSK_OLED_H

#include "s443396_reg_oled.h"
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define OLED_INIT_PRIORITY      (tskIDLE_PRIORITY + 2)
#define OLED_INIT_STACK_SIZE    (configMINIMAL_STACK_SIZE * 6)

struct oledTextMsg
{
    int startX;
    int startY;
    char displayText[20];
};

QueueHandle_t s443396QueueOled; // Queue used

extern void s443396_tsk_oled_init(void);
void s443396_tsk_oled(void);

#endif