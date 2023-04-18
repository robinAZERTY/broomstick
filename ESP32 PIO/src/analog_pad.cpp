#include "analog_pad.hpp"
#include <Arduino.h>

analPad::analPad(const int thres[], int nbButton)
{
    this->nbButton = nbButton;
    this->thres = new int[2 << nbButton];
    this->button = new bool[nbButton];
    this->combinations = 1;
    for (int i = 0; i < nbButton; i++)
    {
        this->button[i] = 0;
        combinations *= 2;
    }
    for (int i = 0; i < 2 << nbButton; i++)
    {
        this->thres[i] = thres[i];
    }
}

analPad::~analPad()
{
    delete[] thres;
    delete[] button;
}

void analPad::update(int analogicValue)
{
    for (int combination = 0; combination < combinations; combination++)
    {
        if (analogicValue < thres[combination + 1] || combination == combinations - 1)
        {
            for (int button_index = 0; button_index < nbButton; button_index++)
            {
                button[button_index] = ((combination) & (1 << button_index)) ? 1 : 0;
            }
            return;
        }
    }
}

bool analPad::getButton(int index)
{
    return button[index];
}