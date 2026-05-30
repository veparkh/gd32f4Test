#include "can.h"
#include "gd32f4xx.h"

void can_setup(void)
{
    can_parameter_struct can_param;
    can_filter_parameter_struct can_filter;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_CAN0);

    gpio_af_set(GPIOA, GPIO_AF_9, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12);

    // CAN timing: APB1 = 50 MHz, target 500 kbit/s
    // bit time = (1 + BS1 + BS2) TQ = 10 TQ, prescaler = 10
    // 50 MHz / (10 * 10) = 500 kbit/s, sample point = (1+7)/10 = 80%
    can_struct_para_init(CAN_INIT_STRUCT, &can_param);
    can_param.working_mode      = CAN_NORMAL_MODE;
    can_param.resync_jump_width = CAN_BT_SJW_1TQ;
    can_param.time_segment_1    = CAN_BT_BS1_7TQ;
    can_param.time_segment_2    = CAN_BT_BS2_2TQ;
    can_param.prescaler         = 10;
    can_init(CAN0, &can_param);

    // accept-all filter (mask mode, mask = 0 -> no bits checked)
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);
    can_filter.filter_number      = 0U;
    can_filter.filter_mode        = CAN_FILTERMODE_MASK;
    can_filter.filter_bits        = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high   = 0x0000U;
    can_filter.filter_list_low    = 0x0000U;
    can_filter.filter_mask_high   = 0x0000U;
    can_filter.filter_mask_low    = 0x0000U;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable      = ENABLE;
    can_filter_init(&can_filter);
}

void can_send_temperatures(int16_t temp1, uint8_t valid1,
                           int16_t temp2, uint8_t valid2)
{
    can_transmit_message_struct tx;

    can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &tx);
    tx.tx_sfid = CAN_TEMP_FRAME_ID;
    tx.tx_ff   = CAN_FF_STANDARD;
    tx.tx_ft   = CAN_FT_DATA;
    tx.tx_dlen = 8;

    tx.tx_data[0] = (uint8_t)(temp1 & 0xFFU);
    tx.tx_data[1] = (uint8_t)((temp1 >> 8) & 0xFFU);
    tx.tx_data[2] = valid1;
    tx.tx_data[3] = (uint8_t)(temp2 & 0xFFU);
    tx.tx_data[4] = (uint8_t)((temp2 >> 8) & 0xFFU);
    tx.tx_data[5] = valid2;
    tx.tx_data[6] = 0;
    tx.tx_data[7] = 0;

    can_message_transmit(CAN0, &tx);
}