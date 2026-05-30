#include "adc.h"
#include "gd32f4xx.h"

#define ADC_IRQ_PRIORITY  6U

static TaskHandle_t notify_task;

void adc_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8); 

    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1);


    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

    // inserted group: 2 channels, PA0 -> rank 0, PA1 -> rank 1
    adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 2U);
    adc_inserted_channel_config(ADC0, ADC_NTC1_RANK, ADC_CHANNEL_0, ADC_SAMPLETIME_56);
    adc_inserted_channel_config(ADC0, ADC_NTC2_RANK, ADC_CHANNEL_1, ADC_SAMPLETIME_56);


    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    // end-of-inserted-group interrupt
    adc_interrupt_enable(ADC0, ADC_INT_EOIC);
    nvic_irq_enable(ADC_IRQn, ADC_IRQ_PRIORITY, 0U);

    adc_enable(ADC0);
     for (volatile uint32_t i = 0; i < 1000U; i++) {
    }
    adc_calibration_enable(ADC0);
}

void adc_set_notify_task(TaskHandle_t task)
{
    notify_task = task;
}

void adc_start(void)
{
    adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
}

uint16_t adc_read(uint8_t rank)
{
    return adc_inserted_data_read(ADC0, rank);
}

void adc_irq_handler(void)
{
    if (adc_interrupt_flag_get(ADC0, ADC_INT_FLAG_EOIC) == SET) {
        adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);

        if (notify_task != NULL) {
            BaseType_t higher_woken = pdFALSE;
            vTaskNotifyGiveFromISR(notify_task, &higher_woken);
            portYIELD_FROM_ISR(higher_woken);
        }
    }
}