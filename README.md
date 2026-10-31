# ESP-32SE-WROOM
ESP32-S3 is the best Embedded Board for Beginners. 
Will Learn to design Embedded Projects in it. 
Pin Diagrams is uploaded inside.  
Standard header file that requires to be included : 
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <inttypes.h>
#include "led_strip/led_strip.h" // Need to make special CMake//
#include "driver/gpio.h" 
#include "hal/gpio_types.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
