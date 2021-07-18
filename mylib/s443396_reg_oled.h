/**
 **************************************************************
 * @file mylib/s4433963_reg_oled.h
 * @author Joseph Twin - 44339638
 * @date 26042021
 * @brief OLED Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s443396_reg_oled_init() - Initialise the OLED SSD1306 device with the I2C interface.
 ***************************************************************
 */

#ifndef S443396_REG_OLED_H
#define S443396_REG_OLED_H

#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"

#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

extern void s443396_reg_oled_init(void);

#endif