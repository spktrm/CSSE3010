/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_grid.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad grid Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * keypad_task() - Recieves input from the keypad register and translates it to event bits for the CAG keypad grid/mnemonic to interpret
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_keypad_init() - Task for driving the keypad
 ************************************************************** 
 */

#include "s443396_tsk_CAG_keypad_grid.h"

/**
  * @brief  Recieves input from the keypad register and translates it to event bits for the CAG keypad grid/mnemonic to interpret
  * @param  None
  * @retval None
  */
void keypad_task(void)
{
    /* Declare the read value from the keypad to be char */
    char key;

    /* Create the keypad event group */
    s443396keypadEventGroup = xEventGroupCreate();

    /* Declare the keypad event bits */
    EventBits_t keypadBits;

    for (;;)
    {
        /* Transition the keypad throughout columns states */
        s443396_reg_keypad_fsmprocessing();

        /* Read the key only if pressed */
        if (s443396_reg_keypad_read_status())
        {
            key = s443396_reg_keypad_read_key(); /* Get the hexadecimal value of the key that was pressed */

            BRD_LEDGreenToggle(); /* Toggle green LED whenever a key has been pressed */

            /* Set the event bits for any key that was pressed */
            if (key == 0x00)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_0);
            }
            else if (key == 0x01)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_1);
            }
            else if (key == 0x02)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_2);
            }
            else if (key == 0x03)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_3);
            }
            else if (key == 0x04)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_4);
            }
            else if (key == 0x05)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_5);
            }
            else if (key == 0x06)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_6);
            }
            else if (key == 0x07)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_7);
            }
            else if (key == 0x08)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_8);
            }
            else if (key == 0x09)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_9);
            }
            else if (key == 0x0A)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_A);
            }
            else if (key == 0x0B)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_B);
            }
            else if (key == 0x0C)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_C);
            }
            else if (key == 0x0D)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_D);
            }
            else if (key == 0x0E)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_E);
            }
            else if (key == 0x0F)
            {
                keypadBits = xEventGroupSetBits(s443396keypadEventGroup, EVT_BTN_F);
            }
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Task for driving the keypad
  * @param  None
  * @retval None
  */
void s443396_tsk_keypad_init(void)
{
    xTaskCreate((void *)&keypad_task, (const signed char *)"KEYPAD_TSK", KEYPAD_STACK_SIZE, NULL, KEYPAD_PRIORITY, &s443396keypadTask);
}
