#include <stdio.h>
#include <FreeRTOSConfig.h>
#include "freertos/idf_additions.h"
#include "driver/gpio.h"
#include <stdbool.h>
#include <unistd.h>
# define LED 48
# define GPIO_OUTPUTone GPIO_NUM_17
# define GPIO_OUTPUTtwo GPIO_NUM_12

void vTaskPulse(void * pvParameters)
{

	    gpio_config_t io_conf = {
	        .pin_bit_mask = (1ULL << GPIO_OUTPUTone),
	        .mode = GPIO_MODE_OUTPUT,
	        .pull_up_en = GPIO_PULLUP_DISABLE,
	        .pull_down_en = GPIO_PULLDOWN_DISABLE,
	        .intr_type = GPIO_INTR_DISABLE,
	    };
	    gpio_config(&io_conf);
	    
	int j=0;
	    while(1){
	

	    // 2. Turn ON GPIO output (Level = 1)
	    gpio_set_level(GPIO_OUTPUTone, 1);

	    // 3. Keep it ON for 2 seconds (2000 ms)
	    vTaskDelay(pdMS_TO_TICKS(2000));

	    // 4. Turn OFF GPIO output (Level = 0)
	    gpio_set_level(GPIO_OUTPUTone, 0);
	     vTaskDelay(pdMS_TO_TICKS(2000));
		 j++;
	     printf("\n GPIO 'Core 0' Loop done : %d\n", j);
	    }

	 printf("GPIO pulse task finished. Deleting task.\n");
	    vTaskDelete(NULL);
}
void vTaskone(void * pvParameters)
{
	gpio_config_t io_conf = {
		        .pin_bit_mask = (1ULL << GPIO_OUTPUTtwo),
		        .mode = GPIO_MODE_OUTPUT,
		        .pull_up_en = GPIO_PULLUP_DISABLE,
		        .pull_down_en = GPIO_PULLDOWN_DISABLE,
		        .intr_type = GPIO_INTR_DISABLE,
		    };
		    gpio_config(&io_conf);
	xTaskCreatePinnedToCore(vTaskPulse,"Task Blinking",4096,NULL,1,NULL,0);
int k=0;	
while (1)
{
	// 2. Turn ON GPIO output (Level = 1)
	    gpio_set_level(GPIO_OUTPUTtwo, 1);

	    // 3. Keep it ON for 2 seconds (2000 ms)
	    vTaskDelay(pdMS_TO_TICKS(2000));

	    // 4. Turn OFF GPIO output (Level = 0)
	    gpio_set_level(GPIO_OUTPUTtwo, 0);
	     vTaskDelay(pdMS_TO_TICKS(2000));
		 k++;
	     printf("\n GPIO 'Core 1' Loop done : %d\n", k);
}
printf("GPIO pulse task finished. Deleting task.\n");
	    vTaskDelete(NULL);
}


void app_main(void)
{
    xTaskCreatePinnedToCore(vTaskone,"Task One Core One",4096,NULL,4,NULL,1);
}
