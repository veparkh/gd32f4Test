#include "gd32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "can.h"
#include "tasks.h"

int main(void)
{
    adc_init();
    can_setup();
    tasks_create();

    vTaskStartScheduler();
    
    for(;;) {
    }
}