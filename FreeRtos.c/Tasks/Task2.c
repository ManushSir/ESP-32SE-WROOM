/*In this task,one task is running and is being called from second task */
#include <stdio.h>
#include <FreeRTOSConfig.h>
#include "freertos/idf_additions.h"
#include <stdbool.h>
#include <unistd.h>

void vTasktwo(void * pvParameters)
{
	volatile unsigned long Jia;
	
while (1)
{
	printf("\nTask two Jia is running\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
}
	
}
void vTaskone(void * pvParameters)
{
	volatile unsigned long Jinal;
	xTaskCreate(vTasktwo,"Task Jia",4096,NULL,1,NULL);
	
while (1)
{
	printf("\nTask One Jinal is running\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	
}
	
}


void app_main(void)
{
    xTaskCreate(vTaskone,"Task Jinal",4096,NULL,1,NULL);
}
