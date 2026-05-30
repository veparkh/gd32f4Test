#ifndef NTC_H
#define NTC_H

#include <stdint.h>

// Returned when the reading is outside the measurable range
#define NTC_INVALID  INT16_MIN

// ADC code (0..4095) -> temperature in tenths of a degree C (367 = 36.7 C) or NTC_INVALID if out of range.
int16_t ntc_convert(uint16_t adc_code);

#endif /* NTC_H */