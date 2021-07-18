/**
 **************************************************************
 * @file stages/pf/main.c
 * @author Joseph Twin - 44339638
 * @date 13052021
 * @brief project main code
 ***************************************************************
 * INTERNAL FUNCTIONS
 ***************************************************************
 * Hardware_init() - Initialise base mylib register and task libraries
 * push_button_tsk_init() - Create the push button task for toggling keypad mode
 * push_button_tsk() - Handle incoming push button presses
 * vApplicationTickHook() - Used to measure the time between RTOS tasks executions
 * vApplicationIdleHook() - (Unused) Used for the Idle Task
 * vApplicationStackOverflowHook() - Called when a task overflows the stack
 ***************************************************************
 */

#include "main.h"

int main(void)
{
    /* Initialise HAL in the system */
    HAL_Init();

    /* Initialise all base task and register libraries that are needed by CAG tasks */
    Hardware_init();

    /* Initiliase the CAG tasks for Design Tasks 1 - 5 */
    s443396_tsk_CAG_simulator_init();
    s443396_tsk_CAG_display_init();
    s443396_tsk_CAG_keypad_grid_init();
    s443396_tsk_CAG_keypad_mnemonic_init();  /* Initialise mnemonic mode even though not default keypad mode */
    vTaskSuspend(s443396keypadMnemonicTask); /* Suspend it immediately, so that is may only be actvated by future push button presses */
    s443396_tsk_CAG_joystick_init();

    /* Start the task scheduler */
    vTaskStartScheduler();

    return 0;
}

/**
  * @brief  Initiliases the task and register libraries that the CAG tasks rely on
  * @param  None
  * @retval None
  */
void Hardware_init(void)
{
    portDISABLE_INTERRUPTS();

    /* Initialise debug_printf for debugging purposes */
    BRD_debuguart_init();

    s4433963_reg_joystick_init();
    s443396_reg_keypad_init();
    s443396_reg_lta1000g_init();

    portENABLE_INTERRUPTS();

    /* Initialise tasks for base keypad, joystick input and onboard pushbutton */
    s443396_tsk_keypad_init();
    s443396_tsk_joystick_init();
    s443396_push_button_tsk_init();
}

/**
  * @brief  vApplicationTickHook
  * @param  None
  * @retval None
  */
void vApplicationTickHook(void)
{
    /* Placeholder function */
}

/**
  * @brief  Idle Application Task (Disabled)
  * @param  None
  * @retval None
  */
void vApplicationIdleHook(void)
{
    static portTickType xLastTx = 0;

    for (;;)
    {
        /* The idle hook simply prints the idle tick count */
        if ((xTaskGetTickCount() - xLastTx) > (1000 / portTICK_RATE_MS))
        {
            xLastTx = xTaskGetTickCount();
        }
    }
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  Task Handler and Task Name
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
    /* This function will get called if a task overflows its stack. If the 
    parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

    (void)pxTask;
    (void)pcTaskName;

    for (;;)
    {
    }
}