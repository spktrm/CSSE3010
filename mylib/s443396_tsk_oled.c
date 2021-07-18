/**
 **************************************************************
 * @file mylib/s4433963_tsk_oled.c
 * @author Joseph Twin - 44339638
 * @date 26042021
 * @brief OLED Task MyLib Driver
 ***************************************************************
 */

#include "s443396_tsk_oled.h"

void s443396_oled_tsk(void)
{
    s443396_reg_oled_init();

    struct oledTextMsg RecvMessage;

    struct oledTextMsg right_oledMsg;
    struct oledTextMsg left_oledMsg;

    for (;;)
    {   
        if (s443396QueueOled != NULL) 
        {
            ssd1306_Fill(Black);   

            if (xQueueReceive(s443396QueueOled, &RecvMessage, 10))
            {
                if (RecvMessage.startX == 5) 
                {
                    left_oledMsg = RecvMessage;
                    // debug_printf("%d %d %s\r\n", left_oledMsg.startX, left_oledMsg.startY, left_oledMsg.displayText);
                }
                if (RecvMessage.startX == 85) 
                {
                    right_oledMsg = RecvMessage;
                    // debug_printf("%d %d %s\r\n", right_oledMsg.startX, right_oledMsg.startY, right_oledMsg.displayText);
                }
            }

            ssd1306_SetCursor(left_oledMsg.startX, left_oledMsg.startY);
            ssd1306_WriteString(left_oledMsg.displayText, Font_6x8, SSD1306_WHITE);

            ssd1306_SetCursor(55, 0);
            ssd1306_WriteString(":", Font_6x8, SSD1306_WHITE);

            ssd1306_SetCursor(right_oledMsg.startX, right_oledMsg.startY);
            ssd1306_WriteString(right_oledMsg.displayText, Font_6x8, SSD1306_WHITE);

            ssd1306_UpdateScreen();
        }
    }
}

void s443396_tsk_oled_init(void)
{
    xTaskCreate((void *)&s443396_oled_tsk, (const signed char *)"OLED_TSK", OLED_INIT_STACK_SIZE, NULL, OLED_INIT_PRIORITY, NULL);
}