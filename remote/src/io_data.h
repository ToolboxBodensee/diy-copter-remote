#ifndef INPUT_HPP
#define INPUT_HPP
#include "Arduino.h"

class IO_DATA
{
private:
    void     adc_init(void);
    uint16_t adc_read(uint8_t ch);

public:
    uint16_t r_x, r_y;
    bool     r_sw;
    uint16_t l_x, l_y;
    bool     l_sw;
    bool     arm_sw;
    bool     flight_mode_sw;
    uint16_t akku_power;

    IO_DATA();
    void init();
    void update();
};

#endif /* INPUT_HPP */
