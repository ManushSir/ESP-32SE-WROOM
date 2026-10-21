#include <stdio.h>
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig_arch.h"
#include "freertos/task.h"
#include<sdkconfig.h>
#include <stdbool.h>
#include <unistd.h>

# define LED 48

static led_strip_handle_t led_strip;

void blinking_task(void *pvParameter)
	{
		// 1. Setup the RGB 
led_strip_config_t strip_config = {
	.strip_gpio_num = LED, 
			.max_leds = 1,	
};
led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz standard
    };
	
	// Initialize the LED strip
	    led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
		
		while (1) {
		       printf("LED turning ON (White)\n");
		       led_strip_set_pixel(led_strip, 0, 255, 255, 255); // Red=255, Green=255, Blue=255
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(1000));                  // Wait 1 second

		       printf("LED turning OFF\n");
		       led_strip_clear(led_strip);                       // Turn off all colors
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(1000));                  // Wait 1 second
		   }

	}

void app_main(void)
{
	{
		xTaskCreate(&blinking_task, "blinking_task", 3072, NULL , 5, NULL);
    }
	
}

