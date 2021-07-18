/**
 **************************************************************
 * @file mylib/s443396_tsk_CAG_keypad_mnemonic.c
 * @author Joseph Twin - 44339638
 * @date 27052021
 * @brief CAG keypad mnemonic Task Mylib Driver
 **************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************
 * keypad_mnemonic_task() - Recieves input from the keypad task and translates it to keypad mnemonic functionality
 * insert_char_in_command() - Place the selected alphabet character in the last 
  * index of the command_buffer and moves every other char one 
  * index forward while remove the char at index 0
 * check_command() - Check whether the current command has been taken from the command buffer is valid or not
  * if it is, change the mode of the mnemonic keypad to reflect this and collect futher information from 
  * the keyapd if necessary
 * execute_command() - Executes the current command stored in the command string and resets parameters accordingly
 * handle_mode() - Called when any event bits have been set for the keypad and the keypad is mnemonic mode
  * handles whether to execute a function or collect more data from the user
 **************************************************************
 * EXTERNAL FUNCTIONS
 **************************************************************
 * s443396_tsk_CAG_keypad_mnemonic_init() - Task for driving the keypad in mnemonic mode
 ************************************************************** 
 */

#include "s443396_tsk_CAG_keypad_mnemonic.h"

/* Stores all the previous characters that have been give by the mnemonic keypad */
char command_buffer[] = {'0', '0', '0'};

/* Stores potential arguments that mnemonic commands might need */
int argument_buffer[] = {0, 0, 0};

/* Eventually the command_buffer is concatenated into a sting and is stored her */
char command[3];

/* Variable to hold whether the type argument has been set */
int type_set = 0;

/* Variable to hold whether the subgrid X argument has been set */
int subgridX_set = 0;

/* Variable to hold whether the subgrid Y argument has been set */
int subgridY_set = 0;

/* Store the current index of the keypad button array, resets when key has been added to the command_buffer, otherwise cycles through */
int idx = 0;

/* 0 while a valid command has not been entered */
/* 1 if command is for creating still life */
/* 2 if command is for creating oscillator */
/* 3 if command is for creating space ship */
/* 4 if command is for deleting either the CAG joystick task or CAG simulator task */
/* 5 if command is for creating either the CAG joystick task or CAG simulator task */
int mode = 0;

/* Initialise caMessage for sending to the life creating with mnemonic commands to the display */
struct caMessage mnemonicMessage;

/**
  * @brief  Recieves input from the keypad task and translates it to keypad mnemonic functionality
  * @param  None
  * @retval None
  */
void keypad_mnemonic_task(void)
{
    /* Initialise Event group for keypad and keypad grid bits for the buttons */
    EventBits_t keypadBits;
    s443396keypadEventGroup = xEventGroupCreate();

    /* Intialise the characters associated with each number key */
    char array2[] = {'A', 'B', 'C'};
    char array3[] = {'D', 'E', 'F'};
    char array4[] = {'G', 'H', 'I'};
    char array5[] = {'J', 'K', 'L'};
    char array6[] = {'M', 'N', 'O'};
    char array7[] = {'P', 'Q', 'R', 'S'};
    char array8[] = {'T', 'U', 'V'};
    char array9[] = {'W', 'X', 'Y', 'Z'};

    /* Initiliase the mnemonic keypad queue for sending to the CAG simulator */
    s443396mnemonicKeypadQueue = xQueueCreate(10, sizeof(mnemonicMessage));

    for (;;)
    {
        /* Wait on event group bits from keypad */
        keypadBits = xEventGroupWaitBits(s443396keypadEventGroup, KEYPAD_GRID_STATE, pdTRUE, pdFALSE, 10);

        if ((keypadBits & EVT_BTN_0) != 0)
        {
            handle_mode(array2, EVT_BTN_0); /* handle the respecitve key press */

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_0);
        }
        else if ((keypadBits & EVT_BTN_1) != 0)
        {
            handle_mode(array2, EVT_BTN_1);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_1);
        }
        else if ((keypadBits & EVT_BTN_2) != 0)
        {
            handle_mode(array2, EVT_BTN_2);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_2);
        }
        else if ((keypadBits & EVT_BTN_3) != 0)
        {
            handle_mode(array3, EVT_BTN_3);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_3);
        }
        else if ((keypadBits & EVT_BTN_4) != 0)
        {
            handle_mode(array4, EVT_BTN_4);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_4);
        }
        else if ((keypadBits & EVT_BTN_5) != 0)
        {
            handle_mode(array5, EVT_BTN_5);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_5);
        }
        else if ((keypadBits & EVT_BTN_6) != 0)
        {
            handle_mode(array6, EVT_BTN_6);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_6);
        }
        else if ((keypadBits & EVT_BTN_7) != 0)
        {
            handle_mode(array7, EVT_BTN_7);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_7);
        }
        else if ((keypadBits & EVT_BTN_8) != 0)
        {
            handle_mode(array8, EVT_BTN_8);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_8);
        }
        else if ((keypadBits & EVT_BTN_9) != 0)
        {
            handle_mode(array9, EVT_BTN_9);

            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_9);
        }
        else if ((keypadBits & EVT_BTN_A) != 0)
        {
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_A);
        }
        else if ((keypadBits & EVT_BTN_B) != 0)
        {
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_B);
        }
        else if ((keypadBits & EVT_BTN_C) != 0)
        {
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_C);
        }
        else if ((keypadBits & EVT_BTN_D) != 0)
        {
            stop_simulation();
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_D);
        }
        else if ((keypadBits & EVT_BTN_E) != 0)
        {
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_E);
        }
        else if ((keypadBits & EVT_BTN_F) != 0)
        {
            start_simulation();
            keypadBits = xEventGroupClearBits(s443396keypadEventGroup, EVT_BTN_F);
        }

        check_command(command); /* Check if current command is valid */

        vTaskDelay(1);
    }
}

/**
  * @brief  Place the selected alphabet character in the last 
  * index of the command_buffer and moves every other char one 
  * index forward while remove the char at index 0
  * @param  char which character the appended to the end of the command buffer
  * @retval None
  */
void insert_char_in_command(char command_char)
{
    command_buffer[0] = command_buffer[1]; /* Move all other characters in buffer forward 1 */
    command_buffer[1] = command_buffer[2]; /* Move all other characters in buffer forward 1 */
    command_buffer[2] = command_char;      /* Insert character in command buffer at the end */
}

/**
  * @brief  Check whether the current command has been taken from the command buffer is valid or not
  * if it is, change the mode of the mnemonic keypad to reflect this and collect futher information from 
  * the keyapd if necessary
  * @param  char* -> the current command that is store in the command string
  * @retval None
  */
void check_command(char *command)
{
    int STL = memcmp(command, "STL", 3); /* Used for checking if the current command valid */
    int OSC = memcmp(command, "OSC", 3); /* Used for checking if the current command valid */
    int GLD = memcmp(command, "GLD", 3); /* Used for checking if the current command valid */
    int DLT = memcmp(command, "DLT", 3); /* Used for checking if the current command valid */
    int CRE = memcmp(command, "CRE", 3); /* Used for checking if the current command valid */

    if (STL == 0)
    {
        mode = STL_MODE; /* Change mode accordingly */
    }
    else if (OSC == 0)
    {
        mode = OSC_MODE;
    }
    else if (GLD == 0)
    {
        mode = GLD_MODE;
    }
    else if (DLT == 0)
    {
        mode = DLT_MODE;
    }
    else if (CRE == 0)
    {
        mode = CRE_MODE;
    }
    else
    {
        mode = 0;
    }
}

/**
  * @brief  Called when any event bits have been set for the keypad and the keypad is mnemonic mode
  * handles whether to execute a function or collect more data from the user
  * @param  char* -> the current array of characters associated with this key press if necessary
  * int -> the bit position of the current button event
  * @retval None
  */
void handle_mode(char *array, int evt_btn)
{
    unsigned int number = 0; /* Stores the index of the bit in evt_btn */
    unsigned int i = 1;      /* Used for masking the evt_btn variable to find out which place the bit is */

    int array_length; /* Initialise the the variable to store the length of array associated with keypress */

    while (!(i & evt_btn))
    {
        i = i << 1;
        ++number; /* Increment number until evt_btn bit is found */
    }

    if (number == 6 || number == 7 || number == 9)
    {
        array_length = 4; /* Specify length of character array based on which btn has been pressed */
    }
    else
    {
        array_length = 3;
    }

    if (mode == CMD_MODE)
    {
        if (evt_btn == EVT_BTN_0) /* If no commands have been selected ... */
        {
            clear_grid();               /* Clear grid if 0 key is pressed */
            update_display_semaphore(); /* Update display */
        }
        else
        {
            char array_str[array_length]; /* create string from character array of current key pressed */
            memcpy(array_str, array, array_length);

            if (strchr(array_str, command_buffer[2]) != NULL) /* Check whether the last character of the command_buffer is in the current character array */
            {
                idx += 1;
                if (idx >= array_length)
                {
                    idx = 0;
                }
                command_buffer[2] = array[idx]; /* If it is, overwrite it */
            }
            else
            {
                idx = 0; /* If it isn't, append character end of command buffer and reset character array index */
                insert_char_in_command(array[idx]);
            }

            memcpy(command, command_buffer, 3); /* Update the current list of characters in the command string with those from the comman buffer */
        }
    }
    else if (mode == STL_MODE | mode == OSC_MODE) /* If mode has transitioned to creating still life or an oscillator */
    {
        if (!type_set) /* Check if type has been set */
        {
            if (number == 0 || number == 1 || number == 2) /* Type can only be 0, 1 or 2 for these lifeforms */
            {
                argument_buffer[0] = number;
                type_set = 1; /* Update the fact that the type has been set */
            }
        }
        else if (!subgridX_set) /* If the X subgrid value has not been set ... */
        {
            argument_buffer[1] = number;
            subgridX_set = 1;
        }
        else if (!subgridY_set) /* If the Y subgrid value has not been set ... */
        {
            argument_buffer[2] = number;
            subgridY_set = 1;
        }
        if (type_set & subgridX_set & subgridY_set) /* Make sure all 3 arguments needed have been set before executing the command */
        {
            execute_command(command, argument_buffer);
        }
    }
    else if (mode == GLD_MODE) /* If mode is creating a space ship, disregard the need for a type value */
    {
        if (!subgridX_set) /* Only need subgrid X value */
        {
            argument_buffer[1] = number;
            subgridX_set = 1;
        }
        else if (!subgridY_set) /* and subgrid Y value */
        {
            argument_buffer[2] = number;
            subgridY_set = 1;
        }
        if (subgridX_set & subgridY_set) /* Make sure necessary arguments are set before executing the command */
        {
            execute_command(command, argument_buffer);
        }
    }
    else if (mode == DLT_MODE) /* If in delete mode ... */
    {
        if (!type_set) /* Check that the task index has been set */
        {
            if (number == 0 || number == 1) /* Can only except an index of 0 or 1 */
            {
                argument_buffer[0] = number;
                type_set = 1;
            }
        }
        else
        {
            execute_command(command, argument_buffer);
        }
    }
    else if (mode == CRE_MODE) /* Likewise for recreating the task from a valid index */
    {
        if (!type_set)
        {
            if (number == 0 || number == 1)
            {
                argument_buffer[0] = number;
                type_set = 1;
            }
        }
        else
        {
            execute_command(command, argument_buffer);
        }
    }
}

/**
  * @brief  Executes the current command stored in the command string and resets parameters accordingly
  * @param  char* -> the current command
  * int* -> all the potential arguments needed for the mnemonic command
  * @retval None
  */
void execute_command(char *command, int *argument_buffer)
{
    mnemonicMessage.cell_x = argument_buffer[1]; /* Set the X value of the mnemonic message for the CAG simulator */
    mnemonicMessage.cell_y = argument_buffer[2]; /* Set the Y value of the mnemonic message for the CAG simulator */

    if (memcmp(command, "STL", 3) == 0)
    {
        if (argument_buffer[0] == 0) /* Check which kind of still life to send */
        {
            mnemonicMessage.type = STILL_LIFE | BLOCK;
        }
        else if (argument_buffer[0] == 1)
        {
            mnemonicMessage.type = STILL_LIFE | BEEHIVE;
        }
        else if (argument_buffer[0] == 2)
        {
            mnemonicMessage.type = STILL_LIFE | LOAF;
        }

        if (s443396mnemonicKeypadQueue != NULL)
        {
            xQueueSendToFront(s443396mnemonicKeypadQueue, (void *)&mnemonicMessage, (portTickType)10);
        }
    }
    else if (memcmp(command, "OSC", 3) == 0)
    {
        if (argument_buffer[0] == 0) /* Check which kind of oscillator to send */
        {
            mnemonicMessage.type = OSCILLATOR | BLINKER;
        }
        else if (argument_buffer[0] == 1)
        {
            mnemonicMessage.type = OSCILLATOR | TOAD;
        }
        else if (argument_buffer[0] == 2)
        {
            mnemonicMessage.type = OSCILLATOR | BEACON;
        }

        if (s443396mnemonicKeypadQueue != NULL)
        {
            xQueueSendToFront(s443396mnemonicKeypadQueue, (void *)&mnemonicMessage, (portTickType)10);
        }
    }
    else if (memcmp(command, "GLD", 3) == 0)
    {
        mnemonicMessage.type = SPACE_SHIP; /* Space ship only has one type */
        if (s443396mnemonicKeypadQueue != NULL)
        {
            xQueueSendToFront(s443396mnemonicKeypadQueue, (void *)&mnemonicMessage, (portTickType)10);
        }
    }
    else if (memcmp(command, "DLT", 3) == 0) /* Delete appropriate task */
    {
        if (argument_buffer[1] == 0)
        {
            vTaskDelete(s443396simulatorTask);
        }
        else if (argument_buffer[1] == 1)
        {
            vTaskDelete(s443396joystickCAGTask);
        }
    }
    else if (memcmp(command, "CRE", 3) == 0) /* Create appropriate task */
    {
        if (argument_buffer[1] == 0)
        {
            s443396_tsk_CAG_simulator_init();
        }
        else if (argument_buffer[1] == 1)
        {
            s443396_tsk_CAG_joystick_init();
        }
    }

    /* Reset the command_buffer, the command string and the argument buffer */
    /* Also reset all the variables that check whether type or subgrid values have been set */
    command_buffer[0] = '0';
    command_buffer[1] = '0';
    command_buffer[2] = '0';
    memcpy(command, command_buffer, 3);

    argument_buffer[0] = 0;
    argument_buffer[1] = 0;
    argument_buffer[2] = 0;

    type_set = 0;
    subgridX_set = 0;
    subgridY_set = 0;

    /* Reset mode back to taking new commands */
    mode = CMD_MODE;
}

/**
  * @brief  Task for driving the keypad in mnemonic mode
  * @param  None
  * @retval None
  */
void s443396_tsk_CAG_keypad_mnemonic_init(void)
{
    xTaskCreate((void *)&keypad_mnemonic_task, (const signed char *)"CAG_KEYPAD_MNEMONIC_TSK", CAG_KEYPAD_MNEMONIC_STACK_SIZE, NULL, CAG_KEYPAD_MNEMONIC_PRIORITY, &s443396keypadMnemonicTask);
}
