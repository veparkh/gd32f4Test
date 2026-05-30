#ifndef NTC_H
#define NTC_H

#include <stdint.h>

// Returned when the reading is outside the measurable range
#define NTC_INVALID  INT16_MIN

int16_t ntc_convert(uint16_t adc_code);

#endif /* NTC_H */