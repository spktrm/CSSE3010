/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_display.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG display Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_CAG_display() - Task for driving the CAG display
 * draw_dislay() - Draw current grid arrangement onto OLED display
 * set_contrast() - Set the contrast value for the display
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_CAG_display_init(void) - Initialise the CAG Display
 **************************************************************
 */

/* Globally define default value for display contrast */
int contrast = 127;

#include "s443396_tsk_CAG_display.h"

/**
  * @brief  Task for driving the CAG display
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_display(void)
{
    /* Initialise the mylib OLED register */
    s443396_reg_oled_init();

    /* Initialise the value of the LED that shows when a game iteration is performed to be 0 */
    int iteration = 0;

    /* Initialise the default value to write the segment display (e.g. 000000000) */
    int seg = 0;

    /* Store the current selection of subgrid coordinates for writing to the segment display */
    int subgrid_x = 0;
    int subgrid_y = 0;

    /* Initialise the recieve message from CAG simulator for getting which subgrid is selected */
    struct subgridMessage subgridRecvMessage;

    for (;;)
    {
        /* Queue for selecting subgrid on lta1000g */
        if (s443396keypadSubgridDisplayQueue != NULL) /* If subgrid queue exists ... */
        {
            if (xQueueReceive(s443396keypadSubgridDisplayQueue, &subgridRecvMessage, 10)) /* Get message from top of the queue */
            {
                subgrid_x = subgridRecvMessage.subgridX; /* Store the current x value of the subgrid selected */
                subgrid_y = subgridRecvMessage.subgridY; /* Store the current y value of the subgrid selected */
            }
        }

        /* Semaphore to update display every time CAG simulator performs a game iteration */
        if (s443396displayUpdateSemaphore != NULL)
        {
            if (xSemaphoreTake(s443396displayUpdateSemaphore, 10) == pdTRUE) /* Take semaphore */
            {
                iteration ^= 1; /* Toggle the value of iteration LED (lta1000g segement[9]) */

                ssd1306_Fill(Black);    /* Fill the display with black (erase old display) */
                draw_dislay();          /* Draw the grid onto the current display */
                ssd1306_UpdateScreen(); /* Update the display */
            }
        }

        /* Logically OR the iteration toggle and value of the subgrid to the segment value */
        seg = iteration << 9 | subgrid_x << 3 | subgrid_y; /* bitshift iteration value 9 for LED location, likwise for subgrid x LED location */

        s443396_reg_lta1000g_write(seg); /* Write total segment display value to the segment display */
        ssd1306_SetContrast(contrast);   /* Set the contrast of the display to the contrast value (updated by the joystick X axis) */
        vTaskDelay(1);
    }
}

/**
  * @brief  Draw current grid arrangement onto OLED display
  * @param  None
  * @retval None
  */
void draw_dislay(void)
{
    for (int y = 0; y < GRID_HEIGHT; y++) /* For every row in the grid */
    {
        for (int x = 0; x < GRID_WIDTH; x++) /* For every column in the grid */
        {
            if (grid[y][x] == ALIVE) /* If the value of that cell is alive */
            {
                for (int dy = 0; dy < DISPLAY_CELL_SIZE; dy++) /* For x size of DISPLAY_CELL_SIZE (2) */
                {
                    for (int dx = 0; dx < DISPLAY_CELL_SIZE; dx++) /* For y size of DISPLAY_CELL_SIZE (2) */
                    {
                        ssd1306_DrawPixel(2 * x + dx, 2 * y + dy, SSD1306_WHITE); /* Draw White on display at x an y */
                    }
                }
            }
        }
    }
}

/**
  * @brief  Set the contrast value for the display
  * @param  Integer value between 0 and 255 
  * @retval None
  */
void set_contrast(int value)
{
    contrast = value; /* Updat the current contrast value (called by CAG joystick) */
}

/**
  * @brief  Initialise the CAG Display
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_display_init(void)
{
    /* Create the CAG display Task */
    xTaskCreate((void *)&s443396_tsk_CAG_display, (const signed char *)"CAG_DISPLAY_TSK", CAG_DISPLAY_STACK_SIZE, NULL, CAG_DISPLAY_PRIORITY, &s443396displayTask);
}
