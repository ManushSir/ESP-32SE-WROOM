#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define PWM_GPIO        17
#define PWM_FREQ_HZ     5000
#define PWM_DUTY        2048      // 50% duty for 12-bit resolution

void app_main(void)
{
    // Timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_12_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = PWM_FREQ_HZ,
        .clk_cfg          = LEDC_AUTO_CLK
    };

    ledc_timer_config(&ledc_timer);

    // Channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = PWM_GPIO,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = PWM_DUTY,
        .hpoint         = 0
    };

    ledc_channel_config(&ledc_channel);

    printf("PWM started on GPIO17\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
