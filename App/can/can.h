#ifndef CAN_H
#define CAN_H

#include <stdint.h>

// CAN0 on PA11 (RX) / PA12 (TX), 500 kbit/s, normal mode.

// Frame layout (standard ID 0x100, DLC 8):
//   [0-1] NTC1 temperature, int16 little-endian, tenths of degC
//   [2]   NTC1 valid: 1 = valid, 0 = invalid
//   [3-4] NTC2 temperature, int16 little-endian, tenths of degC
//   [5]   NTC2 valid: 1 = valid, 0 = invalid
//   [6-7] reserved (0)

#define CAN_TEMP_FRAME_ID   0x100


void can_setup(void);

void can_send_temperatures(int16_t temp1, uint8_t valid1,
                           int16_t temp2, uint8_t valid2);

#endif /* CAN_H */