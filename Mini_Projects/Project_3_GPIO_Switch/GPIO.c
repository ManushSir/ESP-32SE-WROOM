#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <inttypes.h>
#include "led_strip.h"
#include "driver/gpio.h" 
#include "hal/gpio_types.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"

# define GPIO_INPUT GPIO_NUM_12
# define GPIO_OUTPUT GPIO_NUM_1
# define LED 48
SemaphoreHandle_t xBlinkSemaphore = NULL;
SemaphoreHandle_t xPulseSemaphore = NULL;

void blinking_task(void *pvParameter)
	{
    led_strip_handle_t led_strip = NULL;
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
		int i =  0;
		while(1) {
			if (xSemaphoreTake(xBlinkSemaphore, portMAX_DELAY) == pdTRUE){
		       printf("LED turning ON (White)\n");
		       led_strip_set_pixel(led_strip, 0, 255, 255, 255); // Red=255, Green=255, Blue=255
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(2000));  // Wait 2 second
           printf("LED turning ON (Red)\n");
		       led_strip_set_pixel(led_strip, 0, 255, 0, 0); // Red=255, Green=255, Blue=255
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(2000));                  // Wait 2 second
               printf("LED turning ON (Yellow)\n");
		       led_strip_set_pixel(led_strip, 0, 255, 200, 0); // Red=255, Green=255, Blue=255
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(2000)); 
		       printf("LED turning OFF\n");
		       led_strip_clear(led_strip);                       // Turn off all colors
		       led_strip_refresh(led_strip);                    // Push the change to the light
		       vTaskDelay(pdMS_TO_TICKS(1000));   
		      printf("\n LED Loop  Done : %d\n",i); 
			   vTaskDelay(pdMS_TO_TICKS(1000));               // Wait 1 second  
		   }
    }
	}

void gpio_pulse_task(void *pvParameter){

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_OUTPUT),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
int j = 0;
  while(1)
	{
    if (xSemaphoreTake(xPulseSemaphore, portMAX_DELAY) == pdTRUE){

    // 2. Turn ON GPIO output (Level = 1)
    gpio_set_level(GPIO_OUTPUT, 1);

    // 3. Keep it ON for 2 seconds (2000 ms)
    vTaskDelay(pdMS_TO_TICKS(2000));

    // 4. Turn OFF GPIO output (Level = 0)
    gpio_set_level(GPIO_OUTPUT, 0);
     vTaskDelay(pdMS_TO_TICKS(2000));
     printf("\n GPIO Loop done : %d\n",j);
	}
	            // Short yield delay to let the scheduler switch tasks smoothly
	            vTaskDelay(pdMS_TO_TICKS(1000)); 
}

  void Monitor_Switch(void *pvParameters){
	gpio_reset_pin(GPIO_INPUT);
	gpio_set_direction(GPIO_INPUT,GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_INPUT, GPIO_PULLUP_ONLY);
	int a = -1;
	while(1){
	int level = gpio_get_level(GPIO_INPUT);
	if (level !=a) {   
	if (level == 0) {
	       printf("Switch on PIN 1 is now: PRESSED (LOW)\n");
     // Wake up the GPIO pulse task
         xSemaphoreGive(xPulseSemaphore);
	   } else {
	       printf("Switch on PIN 1 is now: RELEASED (HIGH)\n");
    	 xSemaphoreGive(xBlinkSemaphore);
	   }
	           a = level;	  
}
// Delay 100ms to yield CPU time to other FreeRTOS tasks
	       vTaskDelay(pdMS_TO_TICKS(100));
		   }
}
void app_main(void)
{
  printf("\nHello Reader : Manush Here !\nIn this Programme we will select switch to switch over the tasks!\n ");
  vTaskDelay(pdMS_TO_TICKS(100));
 // 1. Create the binary semaphores BEFORE creating the tasks
    xBlinkSemaphore = xSemaphoreCreateBinary();
    xPulseSemaphore = xSemaphoreCreateBinary();
	xTaskCreate(&Monitor_Switch, "Monitor_Switch", 3072, NULL , 5, NULL);
  xTaskCreate(&gpio_pulse_task, "gpio_pulse_task", 3072, NULL , 3, NULL);
	xTaskCreate(&blinking_task, "blinking_task", 3072, NULL , 4, NULL);
}
