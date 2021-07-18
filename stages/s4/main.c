/**
 **************************************************************
 * @file stages/s4/main.c
 * @author Joseph Twin - 44339638
 * @date 26042021
 * @brief stage 4 main code
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
/* ---------------------------------------------------------------------------*/
#include "s443396_tsk_oled.h"
#include "s443396_tsk_joystick.h"
#include "s443396_reg_iss.h"
/* ---------------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ---------------------------------------------------------------------------*/

int mode = 1;

struct dualTimerMsg
{
    int type; // type is either l or r
    unsigned char timerValue;
};

QueueHandle_t s443396queueTimerDisplay; // Queue used

static void Hardware_init();
void TaskTimerLeft(void);
void TaskTimerRight(void);
void TaskTimerDisplay(void);

// Task Priorities
#define SEND_LEFT_TASK_PRIORITY (tskIDLE_PRIORITY + 4)
#define SEND_RIGHT_TASK_PRIORITY (tskIDLE_PRIORITY + 5)

#define RECEIVERTASK_PRIORITY (tskIDLE_PRIORITY + 3)

// Task Stack Allocations
#define SEND_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define RECEIVERTASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)

/*
 * Starts all the other tasks, then starts the scheduler.
 */
int main(void)
{

    HAL_Init(); //Only HAL_Init() must be called before task creation.

    Hardware_init();

    // Start RTOS tasks

    s443396_tsk_oled_init();
    // s443396_tsk_joystick_init();

    xTaskCreate((void *)&TaskTimerLeft, (const signed char *)"SEND_LEFT", SEND_TASK_STACK_SIZE, NULL, SEND_LEFT_TASK_PRIORITY, NULL);
    xTaskCreate((void *)&TaskTimerRight, (const signed char *)"SEND_RIGHT", SEND_TASK_STACK_SIZE, NULL, SEND_RIGHT_TASK_PRIORITY, NULL);
    xTaskCreate((void *)&TaskTimerDisplay, (const signed char *)"RECV", RECEIVERTASK_STACK_SIZE, NULL, RECEIVERTASK_PRIORITY, NULL);

    // Start the scheduler.
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler. */
    return 0;
}

/*
 * Sender Task.Send a message to the queue, every second.
 */
void TaskTimerLeft(void)
{
    struct dualTimerMsg SendMessage;

    SendMessage.type = 'l';
    SendMessage.timerValue = 0;

    for (;;)
    {
        if (s443396queueTimerDisplay != NULL)
        {
            // debug_printf("%d\r\n", SendMessage.timerValue);

            xQueueSendToFront(s443396queueTimerDisplay, (void *)&SendMessage, (portTickType)10);
        }

        SendMessage.timerValue++; // Increment Sequence Number
        if (SendMessage.timerValue >= 32)
        {
            SendMessage.timerValue = 0;
        }

        // Wait for 1000ms
        vTaskDelay(1000);
    }
}

void TaskTimerRight(void)
{
    struct dualTimerMsg SendMessage;

    SendMessage.type = 'r';
    SendMessage.timerValue = 0;

    for (;;)
    {
        if (s443396queueTimerDisplay != NULL)
        {
            // debug_printf("%d\r\n", SendMessage.timerValue);

            xQueueSendToFront(s443396queueTimerDisplay, (void *)&SendMessage, (portTickType)10);
        }

        SendMessage.timerValue++; // Increment Sequence Number
        if (SendMessage.timerValue >= 32)
        {
            SendMessage.timerValue = 0;
        }

        // Wait for 1000ms
        vTaskDelay(50);
    }
}

/*
 * Receiver Task. Used to receive messages.
 */
void TaskTimerDisplay(void)
{
    struct dualTimerMsg RecvMessage;
    struct oledTextMsg SendMessage;

    s443396queueTimerDisplay = xQueueCreate(10, sizeof(RecvMessage)); // Create queue of length 10 Message items
    s443396QueueOled = xQueueCreate(10, sizeof(SendMessage));         // Create queue of length 10 Message items

    for (;;)
    {
        if (s443396queueTimerDisplay != NULL)
        { // Check if queue exists
            // Check for item received - block atmost for 10 ticks
            if (xQueueReceive(s443396queueTimerDisplay, &RecvMessage, 10))
            {
                if (RecvMessage.type == 'l')
                {
                    debug_printf("left\r\n");

                    SendMessage.startX = 5;
                    SendMessage.startY = 0;
                    sprintf(SendMessage.displayText, "%02d", RecvMessage.timerValue);

                    // debug_printf("%d %d %s\r\n", SendMessage.startX, SendMessage.startY, SendMessage.displayText);

                    xQueueSendToFront(s443396QueueOled, (void *)&SendMessage, (portTickType)10);
                }
                if (RecvMessage.type == 'r')
                {
                    debug_printf("right\r\n");

                    SendMessage.startX = 85;
                    SendMessage.startY = 0;
                    sprintf(SendMessage.displayText, "%02d", RecvMessage.timerValue);

                    // debug_printf("%d %d %s\r\n", SendMessage.startX, SendMessage.startY, SendMessage.displayText);

                    xQueueSendToFront(s443396QueueOled, (void *)&SendMessage, (portTickType)10);
                }
            }
        }
    }
}

/*
 * Hardware Initialisation.
 */
void Hardware_init(void)
{
    portDISABLE_INTERRUPTS(); //Disable interrupts
    BRD_LEDInit();            //Initialise Blue LED
    BRD_debuguart_init();
    portENABLE_INTERRUPTS(); //Enable interrupts
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken;

    if (GPIO_Pin == BRD_A2_PIN)
    {
        xHigherPriorityTaskWoken = pdFALSE;

        EXTI->PR |= EXTI_PR_PR3; //Clear interrupt flag.

        s443396_reg_iss_synchroniser(S443396_REG_ISS_SOURCE_3);

        // Is it time for another Task() to run?
        xHigherPriorityTaskWoken = pdFALSE;

        if (s443396SemaphoreJoystickZ != NULL)
        {                                                                                // Check if semaphore exists
            xSemaphoreGiveFromISR(s443396SemaphoreJoystickZ, &xHigherPriorityTaskWoken); // Give PB Semaphore from ISR
        }

        // Perform context switching, if required.
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);
}