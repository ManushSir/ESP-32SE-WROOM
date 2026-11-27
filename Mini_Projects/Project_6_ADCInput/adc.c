#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include <FreeRTOSConfig.h>
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/idf_additions.h"
#include "driver/gpio.h"
#include "hal/adc_types.h"
#include <stdbool.h>
#include <unistd.h>
# define LED 48
# define GPIO_OUTPUTone GPIO_NUM_17
# define GPIO_OUTPUTtwo GPIO_NUM_12
# define GPIO_PWM GPIO_NUM_18
# define ADC_ONE ADC_CHANNEL_0

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

}

void vTaskReadPin(void *pvParameters)
{
	adc_oneshot_unit_handle_t Pinread;
	adc_oneshot_unit_init_cfg_t config =
	{
		.unit_id = ADC_UNIT_1, 
	};
	
	adc_oneshot_new_unit( &config,&Pinread);
	
	adc_oneshot_chan_cfg_t channel_config={
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12,
	};
	
	adc_oneshot_config_channel(Pinread, ADC_ONE, &channel_config);
	
	while(1)
	{
		int read_pot;
		int width = 4095;
		adc_oneshot_read(Pinread,ADC_ONE,&read_pot);
		float read_volt = (read_pot*3.3)/width;
		printf("Potentiometer = %d \t The potentiometer Voltage = %0.1f\n", read_pot,read_volt);
        vTaskDelay(pdMS_TO_TICKS(500));
		
	}
}

void app_main(void)
{
    xTaskCreatePinnedToCore(vTaskone,"Task One Core One",4096,NULL,4,NULL,1);
	static int duty_cycle = 500;
	//xTaskCreatePinnedToCore( vTaskPwm, "PWM_Task",3072,&duty_cycle, 5,NULL, 0);
	xTaskCreatePinnedToCore(vTaskReadPin,"Task to read potentiometer",4096,NULL,3,NULL,1);
}
