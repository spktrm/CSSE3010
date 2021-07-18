/**
 **************************************************************
 * @file mylib/s4433963_reg_pb.h
 * @author Joseph Twin - 44339638
 * @date 03032021
 * @brief Pushbutton Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s443396_reg_pb_on_init() - enables GPIO input and interrupt
 * s443396_reg_pb_on_isr() - Onboard Pushbutton Interrupt service routine
 * s443396_reg_pb_iss_init() - enables GPIO input and interrupt
 * s443396_reg_pb_iss_isr() - ISS pin source Interrupt service routine
 ***************************************************************
 */

#ifndef S443396_REG_PB_H
#define S443396_REG_PB_H

#include "board.h"
#include "processor_hal.h"

extern void s443396_reg_pb_on_init(void);
extern void s443396_reg_pb_on_isr(void);
extern void s443396_reg_pb_iss_init(void);
extern void s443396_reg_pb_iss_isr(void);

#endif
