#ifndef BROOMSTICK_HPP
#define BROOMSTICK_HPP
#include <Arduino.h>
#include "calibration.hpp"
#include "low_pass_filter.hpp"
#include <EEPROM.h>

//hardware configuration
#define GPIO_MAIN_JOY_X 13
#define GPIO_MAIN_JOY_Y 15

//software configuration
#define COMPUTING_RATE 5000
#define SENDING_RATE 10
#define ANALOG_READ_RESOLUTION 12
#define APPLY_FILTER
#define FILTER_CUTOFF_FREQUENCY 2

//for DEBUG OR TEST
#define PRINT_ON_SERIAL
#ifdef PRINT_ON_SERIAL
    //#define PRINT_FREQ
#endif

//EEPROM configuration
#define CALIBRATION_EEPROM_ADDRESS 0
#define CALIBRATION_EEPROM_FLAG "broomstick_calibration"
#define CALIBRATION_EEPROM_SIZE CALIBRATION_EEPROM_ADDRESS+sizeof(CALIBRATION_EEPROM_FLAG)+sizeof(Broomstick::calib)

class Broomstick
{
public:
    Broomstick();
    ~Broomstick();
    int begin();
    int readSensors();
    int applyCorrection();
    int centerCalibration();
    int extremumCalibration();
    int saveCalibration();
    int loadCalibration();
    int compute();
    int send();
    int run();
    void printCalibration();
    void printData();

    struct Data
    {
        int main_joy_x;
        int main_joy_y;
    };
    struct Data data;

//private:

    struct Calib calib;
    low_pass_filter main_joy_x_filter,main_joy_y_filter;
    bool correction_done=false;
    unsigned long last_update_time=0;
    unsigned long last_send_time=0;
    unsigned long time=0;
    unsigned long dt=0;
};
#endif