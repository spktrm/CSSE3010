/**
 **************************************************************
 * @file mylib/s4433963_reg_keypad.c
 * @author Joseph Twin - 44339638
 * @date 10042021
 * @brief Keypad Mylib Register Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s443396_reg_keypad_init() - Set the state of the keypadFSM to INIT state.
 * s443396_reg_keypad_fsmprocessing() - Keypad FSM processing loop
 * keypad_gpio_init() - Initalise all GPIO pins
 * keypad_writecol() - Internal function to activate a column.
 * s443396_reg_keypad_read_status() - Internal function to activate a column
 * s443396_reg_keypad_read_key(void) - Return the current hexadecimal value of the keypad
 * s443396_reg_keypad_read_ascii(void) - Return the current ASCII value of the keypad
 * s443396_reg_keypad_iss_isr(void) - ISS pin source Interrupt service routine.
 *************************************************************** 
 * INTERNAL FUNCTIONS
 ***************************************************************
 * keypad_readrow() - Internal function for reading a row. 
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s443396_reg_keypad.h"
#include "s443396_reg_iss.h"
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
/* ---------------------------------------------------------------------------*/

static int KeypadFsmCurrentState = INIT_STATE;
int KeypadStatus;
unsigned char KeypadValue;

uint32_t state_change_time;

char value_lookup[] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'};

void keypad_writecol(int colval);

/*
 * @brief Set the state of the keypadFSM to INIT state.
 */
void s443396_reg_keypad_init(void)
{
    keypad_gpio_init();

    // GPIOE->ODR |= (0x01 << 13);
    // GPIOF->ODR |= (0x01 << 14);
    // GPIOF->ODR |= (0x01 << 13);
    // GPIOF->ODR |= (0x01 << 12);

    s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_4, 50);
    s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_5, 50);
    s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_6, 50);
    s443396_reg_iss_delaytimer_ctrl(S443396_REG_ISS_SOURCE_7, 50);
}

/*
 * @brief Keypad FSM processing loop (internal and external function) Called from main() and mylib files, whenever the Keypad FSM must change state
 */
void s443396_reg_keypad_fsmprocessing(void)
{
    uint32_t current_tick = HAL_GetTick();

    if ((current_tick - state_change_time) >= 10)
    {
        if ((KeypadFsmCurrentState == INIT_STATE) || (KeypadFsmCurrentState == RSCAN4_STATE))
        {
            keypad_col1();
            KeypadFsmCurrentState = RSCAN1_STATE;
        }
        else if (KeypadFsmCurrentState == RSCAN1_STATE)
        {
            keypad_col2();
            KeypadFsmCurrentState = RSCAN2_STATE;
        }
        else if (KeypadFsmCurrentState == RSCAN2_STATE)
        {
            keypad_col3();
            KeypadFsmCurrentState = RSCAN3_STATE;
        }
        else if (KeypadFsmCurrentState == RSCAN3_STATE)
        {
            keypad_col4();
            KeypadFsmCurrentState = RSCAN4_STATE;
        }
        else
        {
            KeypadFsmCurrentState = INIT_STATE;
        }
        state_change_time = current_tick;
    }
}

/*
 * @brief Initalise all GPIO pins.
 */
void keypad_gpio_init(void)
{
    // Enable the GPIO F and E Clock
    __GPIOF_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();

    // Enable EXTI clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // A3
    GPIOF->OSPEEDR |= (GPIO_SPEED_FAST << 3); //Set fast speed.
    GPIOF->PUPDR &= ~(0x03 << (3 * 2));       //Clear bits for no push/pull
    GPIOF->MODER &= ~(0x03 << (3 * 2));       //Clear bits for input mode
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PF;
    EXTI->RTSR &= ~EXTI_RTSR_TR3; //disable rising dedge
    EXTI->FTSR |= EXTI_FTSR_TR3;  //enable falling edge
    EXTI->IMR |= EXTI_IMR_IM3;    //Enable external interrupt
    HAL_NVIC_SetPriority(BRD_A3_EXTI_IRQ, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_A3_EXTI_IRQ);

    // A4
    GPIOF->OSPEEDR |= (GPIO_SPEED_FAST << 5); //Set fast speed.
    GPIOF->PUPDR &= ~(0x03 << (5 * 2));       //Clear bits for no push/pull
    GPIOF->MODER &= ~(0x03 << (5 * 2));       //Clear bits for input mode
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PF;
    EXTI->RTSR &= ~EXTI_RTSR_TR5; //disable rising dedge
    EXTI->FTSR |= EXTI_FTSR_TR5;  //enable falling edge
    EXTI->IMR |= EXTI_IMR_IM5;    //Enable external interrupt
    HAL_NVIC_SetPriority(BRD_A4_EXTI_IRQ, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_A4_EXTI_IRQ);

    // A5
    GPIOF->OSPEEDR |= (GPIO_SPEED_FAST << 10); //Set fast speed.
    GPIOF->PUPDR &= ~(0x03 << (10 * 2));       //Clear bits for no push/pull
    GPIOF->MODER &= ~(0x03 << (10 * 2));       //Clear bits for input mode
    SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI10;
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PF;
    EXTI->RTSR &= ~EXTI_RTSR_TR10; //disable rising dedge
    EXTI->FTSR |= EXTI_FTSR_TR10;  //enable falling edge
    EXTI->IMR |= EXTI_IMR_IM10;    //Enable external interrupt

    // D2
    GPIOF->OSPEEDR |= (GPIO_SPEED_FAST << 15); //Set fast speed.
    GPIOF->PUPDR &= ~(0x03 << (15 * 2));       //Clear bits for no push/pull
    GPIOF->MODER &= ~(0x03 << (15 * 2));       //Clear bits for input mode
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI15;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PF;
    EXTI->RTSR &= ~EXTI_RTSR_TR15; //disable rising dedge
    EXTI->FTSR |= EXTI_FTSR_TR15;  //enable falling edge
    EXTI->IMR |= EXTI_IMR_IM15;    //Enable external interrupt
    HAL_NVIC_SetPriority(BRD_D2_EXTI_IRQ, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_D2_EXTI_IRQ);

    // init col pins as outputs
    // D8, PF12
    GPIOF->MODER &= ~(0x03 << (12 * 2)); //clear bits
    GPIOF->MODER |= (0x01 << (12 * 2));  //Set for push pull
    GPIOF->OSPEEDR &= ~(0x03 << (12 * 2));
    GPIOF->OSPEEDR |= 0x02 << (12 * 2);   // Set for Fast speed
    GPIOF->OTYPER &= ~(0x01 << 12);       //Clear Bit for Push/Pull utput
    GPIOF->PUPDR &= ~(0x03 << (12 * 2));  //Clear Bits
    GPIOF->PUPDR |= ((0x01) << (12 * 2)); //Set for Pull down output

    // D7, PF13
    GPIOF->MODER &= ~(0x03 << (13 * 2)); //clear bits
    GPIOF->MODER |= (0x01 << (13 * 2));  //Set for push pull
    GPIOF->OSPEEDR &= ~(0x03 << (13 * 2));
    GPIOF->OSPEEDR |= 0x02 << (13 * 2);   // Set for Fast speed
    GPIOF->OTYPER &= ~(0x01 << 13);       //Clear Bit for Push/Pull utput
    GPIOF->PUPDR &= ~(0x03 << (13 * 2));  //Clear Bits
    GPIOF->PUPDR |= ((0x01) << (13 * 2)); //Set for Pull down output

    // D4, PF4
    GPIOF->MODER &= ~(0x03 << (14 * 2)); //clear bits
    GPIOF->MODER |= (0x01 << (14 * 2));  //Set for push pull
    GPIOF->OSPEEDR &= ~(0x03 << (14 * 2));
    GPIOF->OSPEEDR |= 0x02 << (14 * 2);   // Set for Fast speed
    GPIOF->OTYPER &= ~(0x01 << 14);       //Clear Bit for Push/Pull utput
    GPIOF->PUPDR &= ~(0x03 << (14 * 2));  //Clear Bits
    GPIOF->PUPDR |= ((0x01) << (14 * 2)); //Set for Pull down output

    // D3, PE13
    GPIOE->MODER &= ~(0x03 << (13 * 2)); //clear bits
    GPIOE->MODER |= (0x01 << (13 * 2));  //Set for push pull
    GPIOE->OSPEEDR &= ~(0x03 << (13 * 2));
    GPIOE->OSPEEDR |= 0x02 << (13 * 2);   // Set for Fast speed
    GPIOE->OTYPER &= ~(0x01 << 13);       //Clear Bit for Push/Pull utput
    GPIOE->PUPDR &= ~(0x03 << (13 * 2));  //Clear Bits
    GPIOE->PUPDR |= ((0x01) << (13 * 2)); //Set for Pull down output
}

/*
 * @brief Internal function to activate a column. Lower 4 bits of colval, corresponds to columns [1,2,3,4]
 */
void keypad_writecol(int colval)
{
    if (colval == 1)
    {
        GPIOE->ODR &= ~(0x01 << 13);
        GPIOF->ODR |= (0x01 << 14);
        GPIOF->ODR |= (0x01 << 13);
        GPIOF->ODR |= (0x01 << 12);
    }
    else if (colval == 2)
    {
        GPIOE->ODR |= (0x01 << 13);
        GPIOF->ODR &= ~(0x01 << 14);
        GPIOF->ODR |= (0x01 << 13);
        GPIOF->ODR |= (0x01 << 12);
    }
    else if (colval == 3)
    {
        GPIOE->ODR |= (0x01 << 13);
        GPIOF->ODR |= (0x01 << 14);
        GPIOF->ODR &= ~(0x01 << 13);
        GPIOF->ODR |= (0x01 << 12);
    }
    else if (colval == 4)
    {
        GPIOE->ODR |= (0x01 << 13);
        GPIOF->ODR |= (0x01 << 14);
        GPIOF->ODR |= (0x01 << 13);
        GPIOF->ODR &= ~(0x01 << 12);
    }
}

/*
 * @brief Internal function for reading a row.
 */
int keypad_readrow(void)
{
    if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_4) > 0)
    {
        s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_4);
        return (1 << 0);
    }
    else if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_5) > 0)
    {
        s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_5);
        return (1 << 1);
    }
    else if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_6) > 0)
    {
        s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_6);
        return (1 << 2);
    }
    else if (s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_7) > 0)
    {
        s443396_reg_iss_eventcounter_reset(S443396_REG_ISS_SOURCE_7);
        return (1 << 3);
    }
}

/*
 * @brief Return the KeypadStatus variable.
 */
int s443396_reg_keypad_read_status(void)
{
    int row1_pressed = s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_4) > 0;
    int row2_pressed = s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_5) > 0;
    int row3_pressed = s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_6) > 0;
    int row4_pressed = s443396_reg_iss_eventcounter_read(S443396_REG_ISS_SOURCE_7) > 0;

    if (row1_pressed || row2_pressed || row3_pressed || row4_pressed)
    {
        KeypadStatus = 1;
    }
    else
    {
        KeypadStatus = 0;
    }
    return KeypadStatus;
}

/*
 * @brief Return the current hexadecimal value of the keypad. (0x00 to 0x0F).
 */
int s443396_reg_keypad_read_key(void)
{
    int row = keypad_readrow();

    if (KeypadFsmCurrentState == 1)
    {
        if (row == (1 << 0))
        {
            KeypadValue = 0x01;
        }
        else if (row == (1 << 1))
        {
            KeypadValue = 0x04;
        }
        else if (row == (1 << 2))
        {
            KeypadValue = 0x07;
        }
        else if (row == (1 << 3))
        {
            KeypadValue = 0x00;
        }
    }
    else if (KeypadFsmCurrentState == 2)
    {
        if (row == (1 << 0))
        {
            KeypadValue = 0x02;
        }
        else if (row == (1 << 1))
        {
            KeypadValue = 0x05;
        }
        else if (row == (1 << 2))
        {
            KeypadValue = 0x08;
        }
        else if (row == (1 << 3))
        {
            KeypadValue = 0x0F;
        }
    }
    else if (KeypadFsmCurrentState == 3)
    {
        if (row == (1 << 0))
        {
            KeypadValue = 0x03;
        }
        else if (row == (1 << 1))
        {
            KeypadValue = 0x06;
        }
        else if (row == (1 << 2))
        {
            KeypadValue = 0x09;
        }
        else if (row == (1 << 3))
        {
            KeypadValue = 0x0E;
        }
    }
    else if (KeypadFsmCurrentState == 4)
    {
        if (row == (1 << 0))
        {
            KeypadValue = 0x0A;
        }
        else if (row == (1 << 1))
        {
            KeypadValue = 0x0B;
        }
        else if (row == (1 << 2))
        {
            KeypadValue = 0x0C;
        }
        else if (row == (1 << 3))
        {
            KeypadValue = 0x0D;
        }
    }
    return KeypadValue;
}

/*
 * @brief Return the current ASCII value of the keypad.
 */
char s443396_reg_keypad_read_ascii(void)
{
    int key_index = s443396_reg_keypad_read_key();

    return value_lookup[key_index];
}

/*
 * @brief ISS pin source Interrupt service routine. 
 */
void s443396_reg_keypad_iss_isr(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == (1 << 3))
    {
        s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_4);
    }
    else if (GPIO_Pin == (1 << 5))
    {
        s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_5);
    }
    else if (GPIO_Pin == (1 << 10))
    {
        s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_6);
    }
    else if (GPIO_Pin == (1 << 15))
    {
        s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_7);
    }
}