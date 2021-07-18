/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_mnemonic.h
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad mnemonic Task Mylib Driver
 ***************************************************************
 */

#ifndef S443396_TSK_CAG_KEYPAD_MNEMONIC_H
#define S443396_TSK_CAG_KEYPAD_MNEMONIC_H

/* Include mylib task files where necessary */
#include "s443396_tsk_CAG_simulator.h"
#include "s443396_reg_keypad.h"
#include "s443396_tsk_keypad.h"

/* Include necessary sourcelib header files */
#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/* Define task handle for the keypad in grid mode */
TaskHandle_t s443396keypadMnemonicTask;

/* Define queue handles for sending the selected lifeform and position to the CAG simulator */
QueueHandle_t s443396mnemonicKeypadQueue;

/* Define types of cellular life */
#define CELL 0b00010000
#define DEAD 0b00000000
#define ALIVE 0b00000001

/* Define types of still life */
#define STILL_LIFE 0b00100000
#define BLOCK 0b00000000
#define BEEHIVE 0b00000001
#define LOAF 0b00000010

/* Define types of oscillator life */
#define OSCILLATOR 0b01000000
#define BLINKER 0b00000000
#define TOAD 0b00000010
#define BEACON 0b00000100

/* Define types of space ship life */
#define SPACE_SHIP 0x10000000

/* Define different command modes */
#define CMD_MODE 0
#define STL_MODE 1
#define OSC_MODE 2
#define GLD_MODE 3
#define DLT_MODE 4
#define CRE_MODE 5

/* Define the keypad mnemonic stack size and priority */
#define CAG_KEYPAD_MNEMONIC_PRIORITY (tskIDLE_PRIORITY + 2)
#define CAG_KEYPAD_MNEMONIC_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)

/* Internal functions define explicitly */
void keypad_mnemonic_task(void);
void insert_char_in_command(char command_char);
void check_command(char *command);
void execute_command(char *command, int *argument_buffer);
void handle_mode(char *array, int evt_btn);

/* External functions defined explicitly */
extern void s443396_tsk_CAG_keypad_mnemonic_init(void);

#endif
