/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_joystick.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG joystick Task Mylib Driver
 ***************************************************************
 * INTERNAL FUNCTIONS
 ***************************************************************
 * CAG_joystick_task() - Recieves input from the joystick task and translates it to CAG functionality
 ***************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************
 * s443396_tsk_CAG_joystick_deinit() - Function for deleting the CAG Joystick Task
 * s443396_tsk_CAG_joystick_init() - Task for driving the CAG Joystick
 ***************************************************************
 */

#include "s443396_tsk_CAG_joystick.h"

/**
  * @brief  Recieves input from the joystick task and translates it to CAG functionality
  * @param  None
  * @retval None
  */
void CAG_joystick_task(void)
{
    /* Create the binary for the Z Axis button sempaphore */
    s443396joystickZAxisButtonSemaphore = xSemaphoreCreateBinary();

    s443396joystickXEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick X Axis */
    s443396joystickYEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick Y Axis */
    s443396joystickZEventGroup = xEventGroupCreate(); /* Create Event Group for Joystick Z Axis  */

    EventBits_t joystickXBits; /* Declare event bits for Joystick X axis */
    EventBits_t joystickYBits; /* Declare event bits for Joystick Y axis */
    EventBits_t joystickZBits; /* Declare event bits for Joystick Z axis */

    for (;;)
    {
        /* Wait on event group bits from Joystick */
        joystickXBits = xEventGroupWaitBits(s443396joystickXEventGroup, JOYSTICK_X_STATE, pdTRUE, pdFALSE, 10);
        joystickYBits = xEventGroupWaitBits(s443396joystickYEventGroup, JOYSTICK_Y_STATE, pdTRUE, pdFALSE, 10);
        joystickZBits = xEventGroupWaitBits(s443396joystickZEventGroup, JOYSTICK_Z_STATE, pdTRUE, pdFALSE, 10);

        /* Handle event bits form Joystick X Axis */
        if ((joystickXBits & EVT_JOYSTICK_X_0) != 0)
        {
            set_contrast(0); /* If Joystick X Axis is in lowest value position, set contrast to be 0 */

            joystickXBits = xEventGroupClearBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_0); /* Clear bits */
        }
        else if ((joystickXBits & EVT_JOYSTICK_X_1) != 0)
        {
            set_contrast(63); /* Discretely and Linearly interpolate between values of 0 and 255 for max and min contrast*/

            joystickXBits = xEventGroupClearBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_1); /* Clear bits */
        }
        else if ((joystickXBits & EVT_JOYSTICK_X_2) != 0)
        {
            set_contrast(127); /* Middle value of joystick and hence "grey" value for contrast */

            joystickXBits = xEventGroupClearBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_2); /* Clear bits */
        }
        else if ((joystickXBits & EVT_JOYSTICK_X_3) != 0)
        {
            set_contrast(191);

            joystickXBits = xEventGroupClearBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_3); /* Clear bits */
        }
        else if ((joystickXBits & EVT_JOYSTICK_X_4) != 0)
        {
            set_contrast(255); /* Max contrast (display is brightest when Joystick X is maximum) */

            joystickXBits = xEventGroupClearBits(s443396joystickXEventGroup, EVT_JOYSTICK_X_4); /* Clear bits */
        }

        /* Handle event bits from /Joystick Y Axis */
        if ((joystickYBits & EVT_JOYSTICK_Y_0) != 0)
        {
            set_update_delay(500); /* Same principle for the Y axis, minimum value of Joystick Y Axis corresponds to minimum update delay */

            joystickYBits = xEventGroupClearBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_0); /* Clear bits */
        }
        else if ((joystickYBits & EVT_JOYSTICK_Y_1) != 0)
        {
            set_update_delay(1000); /* Discretely and interpolate up to maximum value */

            joystickYBits = xEventGroupClearBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_1); /* Clear bits */
        }
        else if ((joystickYBits & EVT_JOYSTICK_Y_2) != 0)
        {
            set_update_delay(2000); /* Regular update time (Joystick Y Axis is in middle position) */

            joystickYBits = xEventGroupClearBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_2); /* Clear bits */
        }
        else if ((joystickYBits & EVT_JOYSTICK_Y_3) != 0)
        {
            set_update_delay(5000);

            joystickYBits = xEventGroupClearBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_3); /* Clear bits */
        }
        else if ((joystickYBits & EVT_JOYSTICK_Y_4) != 0)
        {
            set_update_delay(10000); /* Maximum udpate time */

            joystickYBits = xEventGroupClearBits(s443396joystickYEventGroup, EVT_JOYSTICK_Y_4); /* Clear bits */
        }

        /* Handle input from the Joystick Z Axis Button */
        if ((joystickZBits & EVT_JOYSTICK_Z) != 0)
        {
            if (s443396joystickZAxisButtonSemaphore != NULL) /* If semaphore exists .... */
            {
                xSemaphoreGive(s443396joystickZAxisButtonSemaphore); /* Give semaphore to clear display */
            }

            joystickZBits = xEventGroupClearBits(s443396joystickZEventGroup, EVT_JOYSTICK_Z); /* Clear bits */
        }

        vTaskDelay(1);
    }
}

/**
  * @brief  Delete the CAG Joystick task
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_joystick_deinit(void)
{
    vTaskDelete(NULL);
}

/**
  * @brief  Task for driving the CAG Joystick
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_joystick_init(void)
{   
    xTaskCreate((void *)&CAG_joystick_task, (const signed char *)"CAG_JOYSTICK_TSK", CAG_JOYSTICK_STACK_SIZE, NULL, CAG_JOYSTICK_PRIORITY, &s443396joystickCAGTask);
}
