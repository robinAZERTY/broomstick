#include "demultiplexer.hpp"

demultiplexer::demultiplexer(uint8_t complexity, uint8_t selector_pins[], uint8_t value_pin)
{
    this->complexity = complexity;
    this->selector_pins = new uint8_t[complexity];
    this->value_pin = value_pin;
    this->number_of_value = pow(2, complexity);
    this->value = new bool[number_of_value];

    for (uint8_t i=0;i<complexity;i++)
        this->selector_pins[i]=selector_pins[i]; 

    for (uint8_t i=0;i<number_of_value;i++)
        this->value[i]=0; 
    
    pinMode(value_pin, INPUT);
    for (uint8_t i=0;i<complexity;i++)
        pinMode(selector_pins[i], OUTPUT);
}

demultiplexer::~demultiplexer()
{
    delete[] selector_pins;
    delete[] value;
}

void demultiplexer::read()
{
    for (uint8_t i=0;i<number_of_value;i++)
    {
        for (uint8_t j=0;j<complexity;j++)
        {
            if (i & (1 << j))
                digitalWrite(selector_pins[j], HIGH);
            else
                digitalWrite(selector_pins[j], LOW);
        }
        delayMicroseconds(10);
        value[i] = digitalRead(value_pin);
    }
}

bool demultiplexer::get_value(uint8_t index)
{
    return value[index];
}