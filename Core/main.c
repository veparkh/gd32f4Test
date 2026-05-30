#include "gd32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

static void test_task(void *arg)
{
    (void)arg;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void)
{
    xTaskCreate(test_task, "test", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    for (;;) {
    }
}