/*In this task, we have created task one and two with same priority. They execute togaether  simulatneously. 
However the speed is high ao you cannot see the significant delay. */

#include <stdio.h>
#include <FreeRTOSConfig.h>
#include "freertos/idf_additions.h"
#include <stdbool.h>
#include <unistd.h>

void vTaskone(void * pvParameters)
{
	volatile unsigned long Jinal;
	
while (1)
{
	printf("\nTask One Jinal is running\n");
	vTaskDelay(pdMS_TO_TICKS(1000));
}	
}

void vTasktwo(void * pvParameters)
{
volatile unsigned long Jia;	
while (1)
{
	printf("\nTask two Jia is running\n");
	vTaskDelay(pdMS_TO_TICKS(1000));	
}
}
void app_main(void)
{
  xTaskCreate(vTaskone,"Task Jinal",4096,NULL,1,NULL);
	xTaskCreate(vTasktwo,"Task Jia",4096,NULL,1,NULL);
}
