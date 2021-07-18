/**
 **************************************************************
 * @file stages/s2/main.c
 * @author Joseph Twin - 44339638
 * @date 19032021
 * @brief stage 1 main code
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "s443396_reg_iss.h"
#include "s443396_reg_pantilt.h"
#include "s443396_reg_joystick.h"
#include "s443396_reg_lta1000g.h"
/* ---------------------------------------------------------------------------*/

int main(void) {

    int dutycycle = 10;
    int angle = -90;

    int pan;
    int tilt;
    int btn;

    HAL_Init();
    BRD_debuguart_init();
    s443396_reg_pantilt_init();
    s4433963_reg_joystick_init();

    while(1) {

        // if (angle > 90) {
        //     angle = -90;
        // }

        // S443396_REG_PANTILT_PAN_WRITE(angle);
        // S443396_REG_PANTILT_TILT_WRITE(angle);
        // angle += 10;
        
        // HAL_Delay(1000);

        pan = S443396_REG_JOYSTICK_X_READ();
        tilt = S443396_REG_JOYSTICK_Y_READ();
        btn = S443396_REG_JOYSTICK_Z_READ();
        // debug_printf("x:%d\r\n", pan);
        debug_printf("x:%d y:%d, z:%d\r\n", pan, tilt);
    }

    return 0;
}