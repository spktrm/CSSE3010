/**
 **************************************************************
 * @file stages/s3/main.c
 * @author Joseph Twin - 44339638
 * @date 10042021
 * @brief stage 3 main code
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "s443396_reg_iss.h"
#include "s443396_lib_hamming.h"
#include "s443396_reg_keypad.h"
#include "s443396_reg_lta1000g.h"
#include "s443396_reg_pb.h"
/* ---------------------------------------------------------------------------*/

int main(void)
{

    int key;
    char ascii;

    unsigned short seg;

    uint16_t encoded, decoded;

    uint32_t current_tick, prev_tick;

    HAL_Init();
    BRD_debuguart_init();
    s443396_reg_keypad_init();
    s443396_reg_lta1000g_init();

    while (1)
    {      
        // call fsmprocessing every 50ms
        current_tick = HAL_GetTick();
        if ((current_tick - prev_tick) >= 50) {
            s443396_reg_keypad_fsmprocessing();
            prev_tick =  current_tick;
        }

        // read key only if pressed
        if (s443396_reg_keypad_read_status()) {
            key = s443396_reg_keypad_read_key();

            // clear bits and set them
            seg &= 0x00;
            seg |= key;

            encoded = s443396_lib_hamming_byte_encode(key);
            decoded = s443396_lib_hamming_byte_decode(encoded);
            
            ascii = s443396_reg_keypad_read_ascii();
            debug_printf("key: %c, encoded: %4x, decoded: %4x\n\r", ascii, encoded, decoded);
        }

        // write to display
        s443396_reg_lta1000g_write(seg);
    }

    return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
    s443396_reg_keypad_iss_isr(GPIO_Pin);
}

void EXTI3_IRQHandler(void)
{
    // A3,
    // debug_printf("row1\r\n");
    HAL_GPIO_EXTI_IRQHandler(BRD_A3_PIN);
}

void EXTI9_5_IRQHandler(void)
{
    // A4
    // debug_printf("row2\r\n");
    HAL_GPIO_EXTI_IRQHandler(BRD_A4_PIN);
}

void EXTI15_10_IRQHandler(void)
{   
    // debug_printf("row3 or 4\r\n");
    if (EXTI->PR == (1 << 10))
    { // A5
        HAL_GPIO_EXTI_IRQHandler(BRD_A5_PIN);
    }
    else if (EXTI->PR == (1 << 15))
    { // D2
        HAL_GPIO_EXTI_IRQHandler(BRD_D2_PIN);
    }
}
