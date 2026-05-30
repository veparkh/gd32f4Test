#include "ntc.h"

#define ADC_MAX    4095.0f
#define VREF       3.3f
#define VSUPPLY    5.0f
#define R_SERIES   3300.0f   // R59

#define ADC_SATURATED  4000U

typedef struct {
    float   r;        // ohms
    int16_t temp_dC;  // tenths of a degree C
} ntc_point_t;

static const ntc_point_t table[] = {
    { 402000.0f, -400 }, { 200000.0f, -300 }, { 105000.0f, -200 },
    {  58200.0f, -100 }, {  33660.0f,    0 }, {  20200.0f,  100 },
    {  12500.0f,  200 }, {  10000.0f,  250 }, {   8040.0f,  300 },
    {   5300.0f,  400 }, {   3590.0f,  500 }, {   2490.0f,  600 },
    {   1760.0f,  700 }, {   1270.0f,  800 }, {    934.0f,  900 },
    {    638.0f, 1000 }, {    529.0f, 1100 }, {    198.0f, 1500 },
    {     74.0f, 2000 },
};

#define TABLE_LEN  (sizeof(table) / sizeof(table[0]))

int16_t ntc_convert(uint16_t adc_code)
{
    if (adc_code >= ADC_SATURATED) {
        return NTC_INVALID;
    }

    float v = ((float)adc_code / ADC_MAX) * VREF;
    float r = R_SERIES * v / (VSUPPLY - v);

    if (r > table[0].r || r < table[TABLE_LEN - 1].r) {
        return NTC_INVALID;
    }

    for (uint32_t i = 0; i < TABLE_LEN - 1; i++) {
        if (r <= table[i].r && r >= table[i + 1].r) {
            float frac = (table[i].r - r) / (table[i].r - table[i + 1].r);
            float t = table[i].temp_dC + frac * (table[i + 1].temp_dC - table[i].temp_dC);
            return (int16_t)(t >= 0.0f ? t + 0.5f : t - 0.5f);
        }
    }
    return NTC_INVALID;
}