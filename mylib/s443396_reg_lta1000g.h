/** 
 **************************************************************
 * @file mylib/s4433963_reg_pb.h
 * @author Joseph Twin - 44339638
 * @date 03032021
 * @brief LTA1000G LED Bar Display Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s443396_reg_lta1000g_init() - enables GPIO input and interrupt
 * lta1000g_seg_set() - Set the value (0 or 1) for the corresponding LED Bar segment GPIO pin
 * s443396_reg_lta1000g_write() - Write the LED Bar segments high or low
 *************************************************************** 
 */

#ifndef S443396_REG_LTA1000G_INIT_H
#define S443396_REG_LTA1000G_INIT_H

#include "board.h"
#include "processor_hal.h"

extern void s443396_reg_lta1000g_init(void);
extern void lta1000g_seg_set(int segment, unsigned char segment_value);
extern void s443396_reg_lta1000g_write(unsigned short value);

#endif
