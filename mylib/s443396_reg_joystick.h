/**
 **************************************************************
 * @file mylib/s4433963_reg_joystick.h
 * @author Joseph Twin - 44339638
 * @date 19032021
 * @brief Joystick Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s4433963_reg_joystick_init() - Initialise GPIO pins and ADC
 * joystick_readxy() - Internal generic function to read X or Y joystick value.
 ***************************************************************
 */

#ifndef S443396_REG_JOYSTICK_H
#define S443396_REG_JOYSTICK_H

#include "board.h"
#include "processor_hal.h"

ADC_HandleTypeDef AdcHandleX;
ADC_HandleTypeDef AdcHandleY;
ADC_ChannelConfTypeDef AdcChanConfigX;
ADC_ChannelConfTypeDef AdcChanConfigY;

extern void s4433963_reg_joystick_init(void);
extern int joystick_readxy(ADC_HandleTypeDef *ADC_Handler);

#define S443396_REG_JOYSTICK_X_READ() joystick_readxy(&AdcHandleX)
#define S443396_REG_JOYSTICK_Y_READ() joystick_readxy(&AdcHandleY)
#define S443396_REG_JOYSTICK_Z_READ() ((GPIOC->IDR & (0x0001 << 3)) >> 3)

#endif
