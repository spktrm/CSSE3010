/** 
 **************************************************************
 * @file mylib/s4433963_reg_keypad.h
 * @author Joseph Twin - 44339638
 * @date 10042021
 * @brief Keypad Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s443396_reg_keypad_init() - Set the state of the keypadFSM to INIT state.
 * s443396_reg_keypad_fsmprocessing() - Keypad FSM processing loop
 * keypad_gpio_init() - Initalise all GPIO pins
 * keypad_writecol() - Internal function to activate a column.
 * s443396_reg_keypad_read_status() - Internal function to activate a column
 * s443396_reg_keypad_read_key(void) - Return the current hexadecimal value of the keypad
 * s443396_reg_keypad_read_ascii(void) - Return the current ASCII value of the keypad
 * s443396_reg_keypad_iss_isr(void) - ISS pin source Interrupt service routine.
 *************************************************************** 
 */

#ifndef S443396_REG_KEYPAD_H
#define S443396_REG_KEYPAD_H

#include "board.h"
#include "processor_hal.h"

/* Defines -------------------------------------------------------------------*/
#define INIT_STATE 0
#define RSCAN1_STATE 1
#define RSCAN2_STATE 2
#define RSCAN3_STATE 3
#define RSCAN4_STATE 4
/* ---------------------------------------------------------------------------*/

extern void s443396_reg_keypad_init(void);
extern void s443396_reg_keypad_fsmprocessing(void);
extern void keypad_gpio_init(void);
extern int s443396_reg_keypad_read_status(void);
extern int s443396_reg_keypad_read_key(void);
extern char s443396_reg_keypad_read_ascii(void);
extern void s443396_reg_keypad_iss_isr(uint16_t GPIO_PIN);

#define keypad_col1() keypad_writecol(1);
#define keypad_col2() keypad_writecol(2);
#define keypad_col3() keypad_writecol(3);
#define keypad_col4() keypad_writecol(4);

#endif
