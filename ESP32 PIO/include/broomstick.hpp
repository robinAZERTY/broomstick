#ifndef BROOMSTICK_HPP
#define BROOMSTICK_HPP
#include <Arduino.h>
#include "calibration.hpp"
#include "low_pass_filter.hpp"
#include <EEPROM.h>
#include <BleGamepad.h> 

//hardware configuration
#define GPIO_MAIN_JOY_X 13
#define GPIO_MAIN_JOY_Y 15
#define GPIO_FIRE 4

//software configuration
#define COMPUTING_RATE 6000
#define SENDING_RATE 50
#define ANALOG_READ_RESOLUTION 12
#define APPLY_FILTER
#define FILTER_CUTOFF_FREQUENCY 2

//for DEBUG OR TEST
#define PRINT_ON_SERIAL
//#define PRINT_FREQ


//EEPROM configuration
#define CALIBRATION_EEPROM_ADDRESS 0
#define CALIBRATION_EEPROM_FLAG "broomstick_calibration"
#define CALIBRATION_EEPROM_SIZE CALIBRATION_EEPROM_ADDRESS+sizeof(CALIBRATION_EEPROM_FLAG)+sizeof(Broomstick::calib)


//BLE CONFIGURATION
#define USE_BLE
#define BLE_NAME "Broomstick"
#define BLE_MANUFACTURER "Tech'ISEN"
#define WAIT_UNTIL_CONNECTED
#define CONNECTION_TIMEOUT 60000

class Broomstick
{
public:
    Broomstick();
    ~Broomstick();
    int begin();
    int run();

    int centerCalibration();
    int extremumCalibration();
    int saveCalibration();
    int loadCalibration();

    void printCalibration();
    void printData();

private:
    int readSensors();
    int applyCorrection();
    int compute();
    int sendBluetooth();
    int beginBluetooth();
    int communicate();

    struct Data
    {
        int main_joy_x;
        int main_joy_y;
        bool fire;
    };

    struct Data data,previous_data;
    struct Calib calib;
    low_pass_filter main_joy_x_filter,main_joy_y_filter;
    bool correction_done=false;
    unsigned long last_update_time=0;
    unsigned long last_send_time=0;
    unsigned long time=0;
    unsigned long dt=0;

    BleGamepad bleGamepad;

};
#endif