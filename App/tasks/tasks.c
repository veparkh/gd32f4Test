#include "tasks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "adc.h"
#include "ntc.h"
#include "can.h"

#define SENSOR_PERIOD_MS    100U
#define ADC_WAIT_MS         10U     // max wait for the ADC end-of-conversion notify
#define QUEUE_LENGTH        4U

typedef struct {
    int16_t temp1;    // tenths of degC, or NTC_INVALID
    uint8_t valid1;   // 1 = valid, 0 = invalid
    int16_t temp2;
    uint8_t valid2;
} temp_message_t;

static QueueHandle_t temp_queue;

static void sensor_task(void *arg)
{
    (void)arg;

    adc_set_notify_task(xTaskGetCurrentTaskHandle());

    TickType_t last_wake = xTaskGetTickCount();
    for(;;) {
        adc_start();
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(ADC_WAIT_MS));

        int16_t t1 = ntc_convert(adc_read(ADC_NTC1_RANK));
        int16_t t2 = ntc_convert(adc_read(ADC_NTC2_RANK));

        temp_message_t msg;
        msg.temp1  = t1;
        msg.valid1 = (uint8_t)(t1 != NTC_INVALID);
        msg.temp2  = t2;
        msg.valid2 = (uint8_t)(t2 != NTC_INVALID);

        xQueueSend(temp_queue, &msg, 0);

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

static void can_task(void *arg)
{
    (void)arg;

    for(;;) {
        temp_message_t msg;
        if(xQueueReceive(temp_queue, &msg, portMAX_DELAY) == pdTRUE) {
            can_send_temperatures(msg.temp1, msg.valid1, msg.temp2, msg.valid2);
        }
    }
}

void tasks_create(void)
{
    temp_queue = xQueueCreate(QUEUE_LENGTH, sizeof(temp_message_t));
    configASSERT(temp_queue != NULL);

    xTaskCreate(sensor_task, "sensor", configMINIMAL_STACK_SIZE + 64,
                NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(can_task, "can", configMINIMAL_STACK_SIZE + 64,
                NULL, tskIDLE_PRIORITY + 1, NULL);
}