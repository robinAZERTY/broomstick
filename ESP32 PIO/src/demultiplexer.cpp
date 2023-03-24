// Implementation in .cpp file

#include "demultiplexer.hpp"
#include "Arduino.h"

demultiplexer::demultiplexer(int selector_pins[], int value_pin[], int complexity, int number_of_demu)
{
    this->complexity = complexity;
    this->selector_pins = new int[complexity];
    this->value_pin = new int[number_of_demu];
    this->number_of_demu = number_of_demu;

    this->number_of_value = pow(2, complexity) * number_of_demu;
    this->value = new int[number_of_value];
    this->value_old = new int[number_of_value];
    this->change_indexes = new bool[number_of_value];
    this->change_indicator = 0;

    for (int i = 0; i < complexity; i++)
        this->selector_pins[i] = selector_pins[i];

    for (int i = 0; i < number_of_demu; i++)
    {
        this->value_pin[i] = value_pin[i];
        pinMode(value_pin[i], INPUT);
    }

    for (int i = 0; i < number_of_value; i++)
    {
        this->value[i] = 0;
        this->value_old[i] = 0;
        this->change_indexes[i] = 1;
    }


    for (int i = 0; i < complexity; i++)
        pinMode(selector_pins[i], OUTPUT);
}

demultiplexer::~demultiplexer()
{
    delete[] selector_pins;
    delete[] value;
    delete[] value_old;
    delete[] change_indexes;
    delete[] value_pin;
}

void demultiplexer::update()
{
    change_indicator = 0;
    for (int subChannel = 0; subChannel < number_of_value / number_of_demu; subChannel++)
    {
        bool A,B,C;
        C = subChannel & 0b001;
        B = subChannel & 0b010;
        A = subChannel & 0b100;
        digitalWrite(selector_pins[0], A);
        digitalWrite(selector_pins[1], B);
        digitalWrite(selector_pins[2], C);
        //Serial.println("A: " + String(A) + " B: " + String(B) + " C: " + String(C));

        delayMicroseconds(2);
        for (int demu_index = 0; demu_index < number_of_demu; demu_index++)
        {
            int channel = subChannel + demu_index * (number_of_value / number_of_demu);
            value_old[channel] = value[channel];
            value[channel] = digitalRead(value_pin[demu_index]);

            if (value[channel] != value_old[channel])
            {
                change_indexes[channel] = 1;
                change_indicator = 1;
            }
            else
            {
                change_indexes[channel] = 0;
            }
        }
    }
}


int demultiplexer::get_value(int index)
{
    return value[index];
}

bool demultiplexer::get_change_indicator()
{
    return change_indicator;
}

bool demultiplexer::get_change_index(int index)
{
    return change_indexes[index];
}