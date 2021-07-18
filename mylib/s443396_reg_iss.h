/** 
 **************************************************************
 * @file mylib/s4433963_reg_pb.h
 * @author Joseph Twin - 44339638
 * @date 03032021
 * @brief Input Signal Synchroniser Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s443396_reg_iss_init() - Initialise the input signal synchroniser counters
 * s443396_reg_iss_synchroniser() - Synchronises the input signal
 * s443396_reg_iss_eventcounter_read() - Returns the specified event counter value
 * s443396_reg_iss_lasttimer_read() - Returns the specified last time value
 * s443396_reg_iss_eventcounter_reset() - Reset the specified event counter value
 * s443396_reg_iss_lasttimer_reset() - Reset the specified last time value
 * s443396_reg_iss_delaytimer_ctrl() - Set the delay value for the specified source
 *************************************************************** 
 */

#ifndef S443396_REG_ISS_H
#define S443396_REG_ISS_H

#include "board.h"
#include "processor_hal.h"

/* Parameters ----------------------------------------------------------------*/
#define S443396_REG_ISS_SOURCE_1 0
#define S443396_REG_ISS_SOURCE_2 1
#define S443396_REG_ISS_SOURCE_3 2
#define S443396_REG_ISS_SOURCE_4 3
#define S443396_REG_ISS_SOURCE_5 4
#define S443396_REG_ISS_SOURCE_6 5
#define S443396_REG_ISS_SOURCE_7 6
/* ---------------------------------------------------------------------------*/

extern void s443396_reg_iss_init(void);
extern void s443396_reg_iss_synchroniser(unsigned char signal_source_index);
extern uint32_t s443396_reg_iss_lasttimer_read(unsigned char signal_source_index);
extern uint32_t s443396_reg_iss_eventcounter_read(unsigned char signal_source_index);
extern void s443396_reg_iss_lasttimer_reset(unsigned char signal_source_index);
extern void s443396_reg_iss_eventcounter_reset(unsigned char signal_source_index);
extern void s443396_reg_iss_delaytimer_ctrl(unsigned char signal_source_index, int delay_value);

#endif
