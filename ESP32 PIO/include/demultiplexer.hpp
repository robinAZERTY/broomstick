#ifndef DEMU_HPP
#define DEMU_HPP

#include <Arduino.h>

class demultiplexer
{
public:
    demultiplexer(uint8_t complexity, uint8_t selector_pins[], uint8_t value_pin);
    ~demultiplexer();
    void read();
    bool get_value(uint8_t index);

private:
    int complexity;
    uint8_t *selector_pins;
    uint8_t value_pin;
    int number_of_value;
    bool *value;
};
#endif