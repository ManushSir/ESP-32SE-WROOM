/*The previous code executes as expected. However, when the switch changes it's state the task stop abruptly.
The code below is designed to execute in the desired way   */

#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"

#include "led_strip.h"

#define GPIO_INPUT   GPIO_NUM_14
#define GPIO_OUTPUT  GPIO_NUM_1
#define LED          48

TaskHandle_t blinkHandle = NULL;
TaskHandle_t pulseHandle = NULL;

/* Task IDs */
typedef enum
{
    TASK_BLINK = 0,
    TASK_PULSE = 1
} task_mode_t;

/*
 * This variable stores the task that SHOULD be running.
 * Monitor_Switch updates it whenever the switch changes.
 */
volatile task_mode_t requestedTask = TASK_BLINK;


/************************************************************
 * BLINK TASK
 ************************************************************/
void blinking_task(void *pvParameter)
{
    led_strip_handle_t led_strip = NULL;

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED,
        .max_leds = 1,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
    };

    led_strip_new_rmt_device(&strip_config,
                             &rmt_config,
                             &led_strip);

    int i = 0;

    while (1)
    {
        printf("LED turning ON (White)\n");
        led_strip_set_pixel(led_strip, 0, 255, 255, 255);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("LED turning ON (Red)\n");
        led_strip_set_pixel(led_strip, 0, 255, 0, 0);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("LED turning ON (Yellow)\n");
        led_strip_set_pixel(led_strip, 0, 255, 200, 0);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("LED turning OFF\n");
        led_strip_clear(led_strip);
        led_strip_refresh(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000));

        i++;

        printf("\nLED Loop Done : %d\n", i);

        /*
         * Check only AFTER the complete sequence.
         */
        if (requestedTask == TASK_PULSE)
        {
            printf("Blink task completed cycle. Switching to Pulse Task...\n");

            vTaskResume(pulseHandle);

            /* Suspend myself */
            vTaskSuspend(NULL);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


/************************************************************
 * GPIO PULSE TASK
 ************************************************************/
void gpio_pulse_task(void *pvParameter)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_OUTPUT),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);

    int j = 0;

    while (1)
    {
        gpio_set_level(GPIO_OUTPUT, 1);

        vTaskDelay(pdMS_TO_TICKS(2000));

        gpio_set_level(GPIO_OUTPUT, 0);

        vTaskDelay(pdMS_TO_TICKS(2000));

        j++;

        printf("\nGPIO Loop Done : %d\n", j);

        /*
         * Check only AFTER the complete pulse cycle.
         */
        if (requestedTask == TASK_BLINK)
        {
            printf("Pulse task completed cycle. Switching to Blink Task...\n");

            vTaskResume(blinkHandle);

            /* Suspend myself */
            vTaskSuspend(NULL);
        }
    }
}


/************************************************************
 * MONITOR SWITCH
 ************************************************************/
void Monitor_Switch(void *pvParameters)
{
    gpio_reset_pin(GPIO_INPUT);
    gpio_set_direction(GPIO_INPUT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_INPUT, GPIO_PULLUP_ONLY);

    int previousLevel = -1;

    while (1)
    {
        int level = gpio_get_level(GPIO_INPUT);

        if (level != previousLevel)
        {
            if (level == 0)
            {
                printf("Switch PRESSED (LOW)\n");

                /*
                 * Request Pulse Task.
                 * Do NOT suspend Blink immediately.
                 */
                requestedTask = TASK_PULSE;
            }
            else
            {
                printf("Switch RELEASED (HIGH)\n");

                /*
                 * Request Blink Task.
                 * Do NOT suspend Pulse immediately.
                 */
                requestedTask = TASK_BLINK;
            }

            previousLevel = level;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


/************************************************************
 * APP MAIN
 ************************************************************/
void app_main(void)
{
    printf("\nHello Reader : Manush Here !\n");
    printf("Current task completes before switching.\n\n");

    xTaskCreate(
        gpio_pulse_task,
        "gpio_pulse_task",
        3072,
        NULL,
        3,
        &pulseHandle);

    xTaskCreate(
        blinking_task,
        "blinking_task",
        3072,
        NULL,
        4,
        &blinkHandle);

    /*
     * Start with Blink Task active.
     */
    vTaskSuspend(pulseHandle);

    requestedTask = TASK_BLINK;

    xTaskCreate(
        Monitor_Switch,
        "Monitor_Switch",
        3072,
        NULL,
        5,
        NULL);
}
