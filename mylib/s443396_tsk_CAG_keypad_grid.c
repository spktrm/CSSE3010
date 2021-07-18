/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_grid.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad grid Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * keypad_grid_task() - Recieves input from the keypad task and translates it to keypad grid functionality
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_keypad_init() - Task for driving the keypad in grid mode
 ************************************************************** 
 */

#include "s443396_tsk_CAG_keypad_grid.h"

/**
  * @brief  Recieves input from the keypad task and translates it to keypad grid functionality
  * @param  None
  * @retval None
  */
void keypad_grid_task(void)
{
    int offset_x = 0;
    int offset_y = 0;

    /* Initialise Event group for keypad and keypad grid bits for the buttons */
    EventBits_t keypadBits;
    s443396keypadEventGroup = xEventGroupCreate();

    /* Initialise the subgrid message to be sent to the CAG display */
    struct subgridMessage subgridSendMessage;

    /* Init starting subgrid numbers */
    int subgrid_x = 0;
    int subgrid_y = 0;

    /* Initialise the subgrid message sent to CAG display */
    subgridSendMessage.subgridX = subgrid_x;
    subgridSendMessage.subgridY = subgrid_y;

    /* Create queue for sending subgrid selection to simulator */
    s443396keypadSubgridSimulatorQueue = xQueueCreate(10, sizeof(subgridSendMessage));

    for (;;)
    {
        /* Wait on event group bits from keypad */
        keypadBits = xEventGroupWaitBits(s443396keypadEventGroup, KEYPAD_GRID_STATE, pdTRUE, pdFALSE, 10);

        /* If keypad press is 0, increment the subgrid counter for X axis */
        if ((keypadBits & EVT_BTN_0) != 0)
        {
            subgrid_x += 1; /* Increment subgrid axis */
            if (subgrid_x > GRID_WIDTH / SUBGRID_SIZE)
            {
                subgrid_x = 0; /* If new value is greater than no. subgrids on axis, reset to 1 */
            }

            subgridSendMessage.subgridX = subgrid_x;

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_0);
        }
        else if ((keypadBits & EVT_BTN_F) != 0) /* If keypad press is F, increment the subgrid counter for Y axis */
        {
            subgrid_y += 1; /* Increment subgrid axis */
            if (subgrid_y > GRID_HEIGHT / SUBGRID_SIZE)
            {
                subgrid_y = 0; /* If new value is greater than no. subgrids on axis, reset to 1 */
            }

            subgridSendMessage.subgridY = subgrid_y;

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_F);
        }
        else if ((keypadBits & EVT_BTN_1) != 0) /* If keypad press is 1, toggle cell at that position */
        {
            offset_x = 0;
            offset_y = 0;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_1);
        }
        else if ((keypadBits & EVT_BTN_2) != 0) /* If keypad press is 2, toggle cell at that position */
        {
            offset_x = 1;
            offset_y = 0;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_2);
        }
        else if ((keypadBits & EVT_BTN_3) != 0) /* If keypad press is 3, toggle cell at that position */
        {
            offset_x = 2;
            offset_y = 0;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_3);
        }
        else if ((keypadBits & EVT_BTN_4) != 0) /* If keypad press is 4, toggle cell at that position */
        {
            offset_x = 0;
            offset_y = 1;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_4);
        }
        else if ((keypadBits & EVT_BTN_5) != 0) /* If keypad press is 5, toggle cell at that position */
        {
            offset_x = 1;
            offset_y = 1;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_5);
        }
        else if ((keypadBits & EVT_BTN_6) != 0) /* If keypad press is 6, toggle cell at that position */
        {
            offset_x = 2;
            offset_y = 1;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_6);
        }
        else if ((keypadBits & EVT_BTN_7) != 0) /* If keypad press is 7, toggle cell at that position */
        {
            offset_x = 0;
            offset_y = 2;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_7);
        }
        else if ((keypadBits & EVT_BTN_8) != 0) /* If keypad press is 8, toggle cell at that position */
        {
            offset_x = 1;
            offset_y = 2;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_8);
        }
        else if ((keypadBits & EVT_BTN_9) != 0) /* If keypad press is 9, toggle cell at that position */
        {
            offset_x = 2;
            offset_y = 2;

            toggle_cell(offset_x, offset_y);
            update_display_semaphore();

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_9);
        }
        else if ((keypadBits & EVT_BTN_A) != 0) /* If keypad press is A, start the simulation */
        {
            start_simulation();
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_A);
        }
        else if ((keypadBits & EVT_BTN_B) != 0) /* If keypad press is B, stop the simulation */
        {
            stop_simulation();
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_B);
        }
        else if ((keypadBits & EVT_BTN_C) != 0) /* If keypad press is C, clear the grid and tell CAG display to update with new empty grid */
        {
            clear_grid();
            update_display_semaphore();
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_C);
        }

        if (s443396keypadSubgridSimulatorQueue != NULL) /* Send information about selected subgrid to CAG simulator */
        {
            xQueueSendToFront(s443396keypadSubgridSimulatorQueue, (void *)&subgridSendMessage, (portTickType)10);
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Task for driving the keypad in grid mode
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_keypad_grid_init(void)
{
    xTaskCreate((void *)&keypad_grid_task, (const signed char *)"CAG_KEYPAD_GRID_TSK", CAG_KEYPAD_GRID_STACK_SIZE, NULL, CAG_KEYPAD_GRID_PRIORITY, &s443396keypadGridTask);
}
