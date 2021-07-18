/**
 **************************************************************
 * @file mylib/s4433963_tsk_pb.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief Pushbutton Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * push_button_tsk() - Recieves input from the joystick register and translates it to event bits for the CAG joystick to interpret
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_joystick_init() - Task for driving the Joystick
 ************************************************************** 
 */

#include "s443396_tsk_pb.h"

/**
  * @brief  Initialise the push button task for dictating whether the keypad is grid or mnemonic mode and toggling the blue LED
  * @param  None
  * @retval None
  */
void push_button_tsk(void)
{
    portDISABLE_INTERRUPTS();

    /* This task makes use of the regular push button register library */
    s443396_reg_pb_on_init();

    /* Initialise the board LEDS */
    BRD_LEDInit();
    BRD_LEDRedOn();   /* Start with red LED on to indicate grid mode */
    BRD_LEDBlueOff(); /* Start with blue LED off to indicate not in mnemonic mode */

    portENABLE_INTERRUPTS();

    /* Set the initial mode to be in grid mode */
    uint8_t keypad_mode = KEYPAD_GRID;

    /* Keep track of current and previous tick times to use for debouncing the push button presses */
    TickType_t current_tick = 0;
    TickType_t prev_tick = 0;

    int pbPressed = 0;

    for (;;)
    {
        if (s443396pushButtonSemaphore != NULL)
        {
            if (xSemaphoreTake(s443396pushButtonSemaphore, 10) == pdTRUE)
            {
                current_tick = xTaskGetTickCountFromISR();
                if (pbPressed)
                {
                    if (current_tick - prev_tick >= 10) /* Use a debouncing delay of 10ms to determine whether a button press has been made */
                    {
                        /* If there was a press, toggle the red and blue LED to indicate changing keypad mode */
                        BRD_LEDRedToggle();
                        BRD_LEDBlueToggle();

                        if (keypad_mode == KEYPAD_GRID)
                        {
                            /* If keypad was in grid mode, we are now transitioning to mnemonic mode */
                            vTaskSuspend(s443396keypadGridTask);    /* Suspend grid mode functionality */
                            vTaskResume(s443396keypadMnemonicTask); /* Resume mnemonic mode functionality */

                            /* Set the keypade mode to mnemonic mode */
                            keypad_mode = KEYPAD_MNEMONIC;
                        }
                        else if (keypad_mode == KEYPAD_MNEMONIC)
                        {
                            /* If keypad was in mnemonic mode, we are now transitioning back to grid mode */
                            vTaskResume(s443396keypadGridTask);      /* Resume grid mode functionality */
                            vTaskSuspend(s443396keypadMnemonicTask); /* Suspend mnemonic mode for later use */

                            /* Set the keypad mode back to grid mode */
                            keypad_mode = KEYPAD_GRID;
                        }
                    }
                    pbPressed = 0;
                }
                else
                {
                    pbPressed = 1;
                }

                /* Store the value of previous for debouncing future button presses */
                prev_tick = current_tick;
            }
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Create the task that handles button presses from the onboard push button
  * @param  None
  * @retval None
  */
void s443396_push_button_tsk_init(void)
{
    s443396pushButtonSemaphore = xSemaphoreCreateBinary();
    xTaskCreate((void *)&push_button_tsk, (const signed char *)"START_PB", PB_STACK_SIZE, NULL, PB_PRIORITY, &s443396pushButtonTask);
}