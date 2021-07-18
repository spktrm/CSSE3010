/**
 **************************************************************
 * @file stages/pf/it_handler.c
 * @author Joseph Twin - 44339638
 * @date 24052021
 * @brief project interrupt handler file
 ***************************************************************
 */

#include "main.h"

/**
  * @brief  External Interrupt Handler on External interrupt line 3
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
    /* Clears interrupt flag for the pin A3 */
    /* Used for column 1 of the keypad */
    HAL_GPIO_EXTI_IRQHandler(BRD_A3_PIN);
}

/**
  * @brief  External Interrupt Handler on External interrupt line 5
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    /* Clears interrupt flag for the pin A4 */
    /* Used for column 2 of the keypad */
    HAL_GPIO_EXTI_IRQHandler(BRD_A4_PIN);
}

/**
  * @brief  External Interrupt Handler on External interrupt line 10
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR == BRD_A5_PIN)
    {
        /* Clears interrupt flag for the pin A5 */
        /* Used for column 3 of the keypad */
        HAL_GPIO_EXTI_IRQHandler(BRD_A5_PIN);
    }
    else if (EXTI->PR == BRD_D2_PIN)
    {
        /* Clears interrupt flag for the pin D2 */
        /* Used for column 4 of the keypad */
        HAL_GPIO_EXTI_IRQHandler(BRD_D2_PIN);
    }
    else if (EXTI->PR == BRD_USER_BUTTON_PIN)
    {
        /* Clears interrupt flag for the onboard user push button */
        HAL_GPIO_EXTI_IRQHandler(BRD_USER_BUTTON_PIN);
    }
}

/**
  * @brief  HAL GPIO External Interrupt Callbacks for all Interrupt Service routines used
  * @param  None
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken;

    if (GPIO_Pin == BRD_A3_PIN)
    {
        s443396_reg_keypad_iss_isr(BRD_A3_PIN); /* Synchronises keypad column 1 button presses in the ISR */
    }
    else if (GPIO_Pin == BRD_A4_PIN)
    {
        s443396_reg_keypad_iss_isr(BRD_A4_PIN); /* Synchronises keypad column 2 button presses in the ISR */
    }
    else if (GPIO_Pin == BRD_A5_PIN)
    {
        s443396_reg_keypad_iss_isr(BRD_A5_PIN); /* Synchronises keypad column 3 button presses in the ISR */
    }
    else if (GPIO_Pin == BRD_D2_PIN)
    {
        s443396_reg_keypad_iss_isr(BRD_D2_PIN); /* Synchronises keypad column 4 button presses in the ISR */
    }
    else if (GPIO_Pin == BRD_USER_BUTTON_PIN)
    {
        /* Check if a higher priority task has arisen */
        xHigherPriorityTaskWoken = pdFALSE;

        if (s443396pushButtonSemaphore != NULL) /* If semaphore for the push button exists ... */
        {                                                                                 
            xSemaphoreGiveFromISR(s443396pushButtonSemaphore, &xHigherPriorityTaskWoken); /* Given the push button semaphore from the ISR */
        }

        /* Used to the perform context switching, if required */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
