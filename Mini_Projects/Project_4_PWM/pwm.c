#include <stdio.h>
#include <FreeRTOSConfig.h>
#include "freertos/idf_additions.h"
#include "driver/gpio.h"
#include <stdbool.h>
#include <unistd.h>
# define LED 48
# define GPIO_OUTPUT GPIO_NUM_17

void vTaskPulse(void * pvParameters)
{
	{

	    gpio_config_t io_conf = {
	        .pin_bit_mask = (1ULL << GPIO_OUTPUT),
	        .mode = GPIO_MODE_OUTPUT,
	        .pull_up_en = GPIO_PULLUP_DISABLE,
	        .pull_down_en = GPIO_PULLDOWN_DISABLE,
	        .intr_type = GPIO_INTR_DISABLE,
	    };
	    gpio_config(&io_conf);
	    
	    for(int j=0;j<=50;j++){

	    // 2. Turn ON GPIO output (Level = 1)
	    gpio_set_level(GPIO_OUTPUT, 1);

	    // 3. Keep it ON for 2 seconds (2000 ms)
	    vTaskDelay(pdMS_TO_TICKS(2000));

	    // 4. Turn OFF GPIO output (Level = 0)
	    gpio_set_level(GPIO_OUTPUT, 0);
	     vTaskDelay(pdMS_TO_TICKS(2000));
	     printf("\n GPIO Loop done : %d\n",j);
	    }

	}
	 printf("GPIO pulse task finished. Deleting task.\n");
	    vTaskDelete(NULL);
	
}
void vTaskone(void * pvParameters)
{
	volatile unsigned long Jinal;
	xTaskCreate(vTaskPulse,"Task Blinking",4096,NULL,1,NULL);
	
while (1)
{
	printf("\nTask One Jinal is running\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
}
	
}


void app_main(void)
{
    xTaskCreate(vTaskone,"Task Jinal",4096,NULL,4,NULL);
}
