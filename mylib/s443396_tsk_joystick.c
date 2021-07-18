/**
 **************************************************************
 * @file mylib/s4433963_tsk_joystick.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief Joystick Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * joystick_task() - Recieves input from the joystick register and translates it to event bits for the CAG joystick to interpret
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_joystick_init() - Task for driving the Joystick
 ************************************************************** 
 */

#include "s443396_tsk_joystick.h"

/**
  * @brief  Recieves input from the joystick register and translates it to event bits for the CAG joystick to interpret
  * @param  None
  * @retval None
  */
void joystick_task(void)
{
    /* Initialise the Joystick X, Y and Z Axis values to be 0 */
    int pan = 0;
    int tilt = 0;
    int btn = 0;

    /* Initialise whether the semaphore for the Joystick Z Axis has been given */
    int given = 0;

    s443396joystickXEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick X Axis */
    s443396joystickYEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick Y Axis */
    s443396joystickZEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick Z Axis  */

    EventBits_t joystickXBits; /* Declare event bits for Joystick X axis */
    EventBits_t joystickYBits; /* Declare event bits for Joystick Y axis */
    EventBits_t joystickZBits; /* Declare event bits for Joystick Z axis */

    for (;;)
    {
        /* read the values of X, Y and Z axis of Joystick */
        pan = S443396_REG_JOYSTICK_X_READ();
        tilt = S443396_REG_JOYSTICK_Y_READ();
        btn = S443396_REG_JOYSTICK_Z_READ();

        /* Send Joystick X value to control the display contrast */
        if (pan >= 0 && pan < 819)
        {
            joystickXBits = xEventGroupSetBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_0);
        }
        else if (pan >= 819 && pan < 1638)
        {
            joystickXBits = xEventGroupSetBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_1);
        }
        else if (pan >= 1638 && pan < 2457)
        {
            joystickXBits = xEventGroupSetBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_2);
        }
        else if (pan >= 2457 && pan < 3276)
        {
            joystickXBits = xEventGroupSetBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_3);
        }
        else if (pan >= 3276 && pan <= 4096)
        {
            joystickXBits = xEventGroupSetBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_4);
        }

        /* Send Joystick Y value to control the simulator update time */
        if (tilt >= 0 && tilt < 819)
        {
            joystickYBits = xEventGroupSetBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_0);
        }
        else if (tilt >= 819 && tilt < 1638)
        {
            joystickYBits = xEventGroupSetBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_1);
        }
        else if (tilt >= 1638 && tilt < 2457)
        {
            joystickYBits = xEventGroupSetBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_2);
        }
        else if (tilt >= 2457 && tilt < 3276)
        {
            joystickYBits = xEventGroupSetBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_3);
        }
        else if (tilt >= 3276 && tilt <= 4096)
        {
            joystickYBits = xEventGroupSetBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_4);
        }

        /* Send Joystick Z value to clear the display */
        if (btn == 0) /* If button has not been pressed */
        {
            if (given) /* And semaphore has not yet been given */
            {
                joystickZBits = xEventGroupSetBits(s443396joystickZEventGroup, EVT_JOYSTICK_Z);
                given = 0; /* lock if statement so that the semaphore may not be given more than once */
            }
        }
        else
        {
            given = 1; /* Else, set the value of given back to 1 so that it may be given once again */
        }

        vTaskDelay(1);
    }
}

/**
  * @brief  Task for driving the Joystick
  * @param  None
  * @retval None
  */
void s443396_tsk_joystick_init(void)
{
    xTaskCreate((void *)&joystick_task, (const signed char *)"JOYSTICK_TSK", JOYSTICK_STACK_SIZE, NULL, JOYSTICK_PRIORITY, &s443396joystickTask);
}