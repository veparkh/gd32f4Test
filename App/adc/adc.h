#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#define ADC_NTC1_RANK   0   // PA0
#define ADC_NTC2_RANK   1   // PA1

void adc_init(void);
void adc_set_notify_task(TaskHandle_t task);
void adc_start(void);
uint16_t adc_read(uint8_t rank);
void adc_irq_handler(void);

#endif /* ADC_H */