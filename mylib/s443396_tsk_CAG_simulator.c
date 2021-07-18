/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_simulator.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG simulator Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * init_simulation() - Initiliase and the CAG simulator
 * init_grid() - Initialise the grid for the simulator
 * toggle_cell() - Toggle the value of a cell alive or dead
 * update_grid() - Perform a game iteraion
 * update_display_semaphore() - Send semaphore to display to update
 * update_grid_mnemonic_life() - Draw lifeform from mnemonic keypad input
 * clear_grid() - Set all grid values to 0
 * start_simulation() - Start/Resume the simulation 
 * stop_simulation() - Stop/Pause the simulation
 * debug_print_grid() - Srint the display to terminal to visualise grid
 * set_update_delay() - Set the game update delay (dt between iterations)
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_CAG_simulator_init() - Create the task for the simulation
 * s443396_tsk_CAG_simulator_deinit() - Delete the task for the simulation
 **************************************************************
 */

#include "s443396_tsk_CAG_simulator.h"

/* Used to check whether the simulator is running or not */
int running = 1;

/* stores information about the current subgrid that has been selected */
int subgrid_x = 0;
int subgrid_y = 0;

/* stores information about the current subgrid that has been selected */
int cell_x;
int cell_y;

/* Initialise the variables to store the x and y position of life spawned using the mnemonic keypad function */
int mnemonic_cell_x = 0;
int mnemonic_cell_y = 0;
int type;

/* Initialise the base rate at which the CAG simulator will run in milliseonds */
TickType_t update_delay = 2000;

/**
  * @brief  Start the CAG simulator
  * @param  None
  * @retval None
  */
void init_simulation(void)
{
    /* Initialise the grid by clearing it */
    clear_grid();

    /* Initialise structure from which to recieve messages from keypad when in mnemonic mode */
    struct caMessage mnemonicMessage;

    /* Initialise structures from which to recieve messages about subgrid selection when keypad is in grid mode */
    struct subgridMessage subgridSendMessage; /* For sending subgrid selection to CAG dipslay */
    struct subgridMessage subgridRecvMessage; /* For recieving subgrid selection from CAG keypad in grid mode */

    /* Keep track current and previous tick times to know when to update the game simulation and how frequently */
    TickType_t current_tick = 0;
    TickType_t prev_tick = 0;

    /* Create the semaphore for telling CAG display when to update */
    s443396displayUpdateSemaphore = xSemaphoreCreateBinary();

    /* Create queue for sending keypad subgrid information to CAG display */
    s443396keypadSubgridDisplayQueue = xQueueCreate(10, sizeof(subgridRecvMessage));

    for (;;)
    {
        /* Recieve messages from the keypad when in grid mode */
        if (s443396keypadSubgridSimulatorQueue != NULL) /* If keypad grid mode subgrid selection queue for simulator exists ... */
        {
            if (xQueueReceive(s443396keypadSubgridSimulatorQueue, &subgridRecvMessage, 10)) /* Recieve message from top of queue */
            {
                subgrid_x = subgridRecvMessage.subgridX; /* Store x value of subgrid selection */
                subgrid_y = subgridRecvMessage.subgridY; /* Store y value of subgrid selection  */

                subgridSendMessage.subgridX = subgrid_x; /* Write x value of subgrid selection to new message for CAG display */
                subgridSendMessage.subgridY = subgrid_y; /* Write y value of subgrid selection to new message for CAG display */

                /* Send information about selected subgrid to CAG display */
                if (s443396keypadSubgridDisplayQueue != NULL) /* If keypad grid mode subgrid selection queue for the display exists ... */
                {
                    xQueueSendToFront(s443396keypadSubgridDisplayQueue, (void *)&subgridSendMessage, (portTickType)10); /* Send message to the front of the queue for the CAG display */
                }
            }
        }

        /* Recieve caMessages from the keypad when in  mnemonic mode */
        if (s443396mnemonicKeypadQueue != NULL) /* If mnemonic message queue exists ... */
        {
            if (xQueueReceive(s443396mnemonicKeypadQueue, &mnemonicMessage, 10)) /* Recieve message from the top of the queue */
            {
                mnemonic_cell_x = mnemonicMessage.cell_x; /* Store x value of subgrid selection from mnemonic keypad message */
                mnemonic_cell_y = mnemonicMessage.cell_y; /* Store y value of subgrid selection from mnemonic keypad message */
                type = mnemonicMessage.type;              /* Store the type of life to be spawned from the message */

                update_grid_mnemonic_life(mnemonic_cell_x, mnemonic_cell_y, type); /* Draw this new life on the display at given subgrid coordinates */
                update_display_semaphore();                                        /* Send semaphore to the CAG display to update the screen with new information */
            }
        }

        /* if Z axis on joystic has been depressed, clear display */
        if (s443396joystickZAxisButtonSemaphore != NULL) /* If the semaphore for Joystick Z Axis exists ...  */
        {
            if (xSemaphoreTake(s443396joystickZAxisButtonSemaphore, 10) == pdTRUE)
            {
                clear_grid();               /* Set every value in the grid to 0 */
                update_display_semaphore(); /* Send semaphore to the CAG display to update the screen with new information */
            }
        }

        /*  update grid every update_delay miliseconds */
        current_tick = xTaskGetTickCountFromISR(); /* Get the current tick count */
        if (running)                               /* Check if able to update the display (game has not been paused) */
        {
            if (current_tick - prev_tick >= update_delay) /* If the time between last update and current time is greater than update_delay */
            {
                update_grid();              /* Perform a game iteration  */
                update_display_semaphore(); /* Send semaphore to the CAG display to update the screen with new information */

                prev_tick = current_tick; /* Set the previous tick value to the current tick value */
            }
        }

        vTaskDelay(1);
    }
}

/**
  * @brief  Used for debugging only, to check the current state of all cells in the grid
  * @param  None
  * @retval None
  */
void debug_print_grid(void)
{
    for (int y = 0; y < GRID_HEIGHT; y++) /* For every row in the grid */
    {
        for (int x = 0; x < GRID_WIDTH; x++) /* For every column in the grid */
        {
            debug_printf("%u", grid[y][x]); /* Print the value of the cell (0 or 1) */
        }
        debug_printf("\n\r"); /* print new line and escape character for formatting on the terminal */
    }

    /* print 3 new lines between each update of the grid for clarity */
    debug_printf("\n\r");
    debug_printf("\n\r");
    debug_printf("\n\r");
}

/**
  * @brief  Set all values of the grid to be 0
  * @param  None
  * @retval None
  */
void clear_grid(void)
{
    for (int y = 0; y < GRID_HEIGHT; y++) /* For every row in the grid */
    {
        for (int x = 0; x < GRID_WIDTH; x++) /* For every column in the grid */
        {
            grid[y][x] = 0; /* Set the value of the cell to be 0 */
        }
    }
}

/**
  * @brief  Perform an iteration of Conway's Game of Life
  * @param  None
  * @retval None
  */
void update_grid(void)
{
    /* Initialise new grid, will hold the values of every cell after then next iteration */
    unsigned char new_grid[GRID_HEIGHT][GRID_WIDTH];

    /* Initialise the neighbor count to be 0 */
    int neighbor_count = 0;

    /* Loop over every cell in array */
    for (int y = 0; y < GRID_HEIGHT; y++) /* For every row in the grid */
    {
        for (int x = 0; x < GRID_WIDTH; x++) /* For every column in the grid */
        {
            new_grid[y][x] = 0; /* Initialise the value of the new grid at this position to be 0, Will only change to 1 if necesary */

            /* Reset the number of neighbors a cell has to 0 */
            neighbor_count = 0;

            for (int dy = -1; dy <= 1; dy++) /* Loop over the y offset values for a cell's neighbors */
            {
                for (int dx = -1; dx <= 1; dx++) /* Loop over the x offset values for a cell's neighbors */
                {
                    if ((y + dy >= 0) && (y + dy < GRID_HEIGHT) && (x + dx >= 0) && (x + dx < GRID_WIDTH)) /* If the current position plus the x and y offset is oustide the bounds of the grid ... */
                    {
                        if (!((x + dx == x) && (y + dy == y))) /* If neighbor to be investigated is not the current cell */
                        {
                            if (grid[y + dy][x + dx] == 1) /* AND only if that neighbor is alive */
                            {
                                neighbor_count += 1; /* Increment that cells neighbor count */
                            }
                        }
                    }
                }
            }

            if (grid[y][x] == 1) /* Check if the current grid position is an alive cell */
            {
                if ((neighbor_count == 2) || (neighbor_count == 3)) /* If the cell has exactly 2 or 3 neighbors ... */
                {
                    new_grid[y][x] = 1; /* Set the next grid iteration to keep this cell alive */
                }
            }
            else
            {                            /* If the cell was dead however ... */
                if (neighbor_count == 3) /* and it was found to have exactly 3 neighbors that were alive */
                {
                    new_grid[y][x] = 1; /* Set this cell to be alive for the next game iteration */
                }
            }
        }
    }

    /* Write the values of the new_grid arrary to old grid array */
    for (int y = 0; y < GRID_HEIGHT; y++) /* Loop over all rows in the grid */
    {
        for (int x = 0; x < GRID_WIDTH; x++) /* Loop over all columns in the grid */
        {
            if (new_grid[y][x] == 1)
            {                   /* If the new grid contains an alive cell at this location */
                grid[y][x] = 1; /* Set it to be alive */
            }
            else
            {                   /* If the new grid contains a dead cell at this location  */
                grid[y][x] = 0; /* Set it to be dead */
            }
        }
    }
}

/**
  * @brief  Draw life form given from mnemonic keypad input
  * @param  None
  * @retval None
  */
void update_grid_mnemonic_life(int cell_x, int cell_y, int type)
{
    int ox = cell_x * SUBGRID_SIZE; /* Initialise actual x coordinate for top left corner of life position */
    int oy = cell_y * SUBGRID_SIZE; /* Initialise actual y coordinate for top left corner of life position */

    if (type == (STILL_LIFE | BLOCK)) /* If lifeform is still life and is a block */
    {
        /* Hard coded cell writing */
        grid[oy][ox] = 1;         /* ---------- */
        grid[oy][ox + 1] = 1;     /* ----00---- */
        grid[oy + 1][ox] = 1;     /* ----00---- */
        grid[oy + 1][ox + 1] = 1; /* ---------- */
    }
    else if (type == (STILL_LIFE | BEEHIVE)) /* If lifeform is still life and is a beehive */
    {
        /* Hard coded cell writing */
        grid[oy][ox + 1] = 1;     /* ---------- */
        grid[oy][ox + 2] = 1;     /* ----0----- */
        grid[oy + 1][ox] = 1;     /* ---0-0---- */
        grid[oy + 1][ox + 3] = 1; /* ---0-0---- */
        grid[oy + 2][ox + 1] = 1; /* ----0----- */
        grid[oy + 2][ox + 2] = 1; /* ---------- */
    }
    else if (type == (STILL_LIFE | LOAF)) /* If lifeform is still life and is a loaf */
    {
        /* Hard coded cell writing */
        grid[oy][ox + 1] = 1;     /* ---------- */
        grid[oy][ox + 2] = 1;     /* ----00---- */
        grid[oy + 1][ox] = 1;     /* ---0--0--- */
        grid[oy + 1][ox + 3] = 1; /* ----0-0--- */
        grid[oy + 2][ox + 3] = 1; /* -----0---- */
        grid[oy + 2][ox + 1] = 1; /* ---------- */
        grid[oy + 3][ox + 2] = 1; /* ---------- */
    }
    else if (type == (OSCILLATOR | BLINKER)) /* If lifeform is an oscillator and is a blinker */
    {
        /* Hard coded cell writing */
        grid[oy + 1][ox] = 1;     /* ---------------0--- */
        grid[oy + 1][ox + 1] = 1; /* ---000---<=>---0--- */
        grid[oy + 1][ox + 2] = 1; /* ---------------0--- */
    }
    else if (type == (OSCILLATOR | TOAD)) /* If lifeform is an oscillator and is a toad */
    {
        /* Hard coded cell writing */
        grid[oy + 1][ox + 1] = 1; /* --------------------- */
        grid[oy + 1][ox + 2] = 1; /* ----00----------0---- */
        grid[oy + 1][ox + 3] = 1; /* ---0------------00--- */
        grid[oy + 2][ox] = 1;     /* ------0---<=>---00--- */
        grid[oy + 2][ox + 1] = 1; /* ----00-----------0--- */
        grid[oy + 2][ox + 2] = 1; /* --------------------- */
    }
    else if (type == (OSCILLATOR | BEACON)) /* If lifeform is an oscillator and is a beacon */
    {
        /* Hard coded cell writing */
        grid[oy][ox] = 1;         /* ----------------------- */
        grid[oy][ox + 1] = 1;     /* ----------------------- */
        grid[oy + 1][ox] = 1;     /* ---00-----------00----- */
        grid[oy + 1][ox + 1] = 1; /* ---00-----------0------ */
        grid[oy + 2][ox + 2] = 1; /* -----00---<=>------0--- */
        grid[oy + 2][ox + 3] = 1; /* -----00-----------00--- */
        grid[oy + 3][ox + 2] = 1; /* ----------------------- */
        grid[oy + 3][ox + 3] = 1; /* ----------------------- */
    }
    else if (type == (SPACE_SHIP)) /* If lifeform is an space and is a glider */
    {
        /* Hard coded cell writing */
        grid[oy][ox + 1] = 1;     /* ------------------------------------------------ */
        grid[oy + 1][ox + 2] = 1; /* ----0---------0------------0--------0-0--------- */
        grid[oy + 2][ox] = 1;     /* -----0---=>----00---=>---0-0---=>----00---=\\--- */
        grid[oy + 2][ox + 1] = 1; /* ---000--------00----------00---------0---<=//--- */
        grid[oy + 2][ox + 2] = 1; /* ------------------------------------------------ */
    }
}

/**
  * @brief  Send semaphore to the CAG display task to update the display
  * @param  None
  * @retval None
  */
void update_display_semaphore(void)
{
    if (s443396displayUpdateSemaphore != NULL) /* Check if the display update semaphore exists */
    {
        xSemaphoreGive(s443396displayUpdateSemaphore); /* Give the semaphore to the CAG display to update the OLED display */
    }
}

/**
  * @brief  Start/Resume the simulation
  * @param  None
  * @retval None
  */
void start_simulation(void)
{
    if (!running) /* If the simualator was paused */
    {
        running = 1; /* Set it running again */
    }
}

/**
  * @brief  Stop the simulation from running
  * @param  None
  * @retval None
  */
void stop_simulation(void)
{
    if (running) /* If the simualor was playing */
    {
        running = 0; /* Set it to be paused */
    }
}

/**
  * @brief  Toggle the value of cell alive or dead
  * @param  None
  * @retval None
  */
void toggle_cell(int x_offset, int y_offset)
{
    cell_y = subgrid_y * SUBGRID_SIZE + y_offset; /* Get actual x location on grid from x subgrid value and x offset */
    cell_x = subgrid_x * SUBGRID_SIZE + x_offset; /* Get actual y location on grid from y subgrid value and y offset */

    grid[cell_y][cell_x] ^= 1; /* XOR 1 with the current value of the cell at this location (effectively toggle it) */
}

/**
  * @brief  Set the update between game iterations
  * @param  None
  * @retval None
  */
void set_update_delay(TickType_t new_delay)
{
    update_delay = new_delay; /* Set the update_delay global variable to be the value specified by new_delay */
}

/**
  * @brief  Delete the CAG Simulator task
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_simulator_deinit(void)
{
    vTaskDelete(NULL);
}

/**
  * @brief  Initialise the CAG Simulator
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_simulator_init(void)
{   
    /* Create the CAG simulator task */
    xTaskCreate((void *)&init_simulation, (const signed char *)"CAG_SIMULATOR_TSK", CAG_SIMULATOR_STACK_SIZE, NULL, CAG_SIMULATOR_PRIORITY, &s443396simulatorTask);
}
