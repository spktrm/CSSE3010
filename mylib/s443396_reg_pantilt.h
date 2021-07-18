/**
 **************************************************************
 * @file mylib/s4433963_reg_pantilt.h
 * @author Joseph Twin - 44339638
 * @date 19032021
 * @brief PanTilt Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s443396_reg_pan_tilt_init() - initialise servo (GPIO, PWM, etc)
 * s4433963_reg_pantilt_write() - Generic function for writing an angle
 * s443396_pantilt_angle_read() - Read the current pan servo angle
 ***************************************************************
 */

#ifndef S443396_REG_PANTILT_H
#define S443396_REG_PANTILT_H

#define S443396_REG_PANTILT_PAN_ZERO_CAL_OFFSET 0;
#define S443396_REG_PANTILT_TILT_ZERO_CAL_OFFSET 0;

#include "board.h"
#include "processor_hal.h"

extern void s443396_reg_pantilt_init(void);
extern void s443396_pantilt_angle_write(int type, int angle);
extern uint16_t s443396_pantilt_angle_read(int type);

#define S443396_REG_PANTILT_PAN_WRITE(value) s443396_pantilt_angle_write(0, value)
#define S443396_REG_PANTILT_PAN_READ() s443396_pantilt_angle_read(0)
#define S443396_REG_PANTILT_TILT_WRITE(value) s443396_pantilt_angle_write(1, value)
#define S443396_REG_PANTILT_TILT_READ() s443396_pantilt_angle_read(1)

#endif
