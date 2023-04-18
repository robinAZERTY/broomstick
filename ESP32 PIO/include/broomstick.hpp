#ifndef BROOMSTICK_HPP
#define BROOMSTICK_HPP
#include <Arduino.h>
#include "calibration.hpp"
#include "low_pass_filter.hpp"
#include "demultiplexer.hpp"
#include <EEPROM.h>
#include <BleGamepad.h> 
#include "analog_pad.hpp"

//hardware configuration
#define GPIO_MAIN_JOY_X 13
#define GPIO_MAIN_JOY_Y 14
#define GPIO_FIRE 21
#define GPIO_THROTTLE 27
#define GPIO_BREAK 26

//software configuration
#define COMPUTING_RATE 200
#define SENDING_RATE 50
#define ANALOG_READ_RESOLUTION 12
#define APPLY_FILTER
#define FILTER_CUTOFF_FREQUENCY 2

//demultiplexer configuration
#define COMPLEXITY 3
#define SELECTOR_PIN new int[COMPLEXITY]{15,2,4}
#define NUMBER_OF_DEMU 4
#define VALUE_PIN new int[NUMBER_OF_DEMU]{5,18,19,32}


//for DEBUG OR TEST
#define PRINT_ON_SERIAL
#define PRINT_FREQ


//EEPROM configuration
#define CALIBRATION_EEPROM_ADDRESS 0
#define CALIBRATION_EEPROM_FLAG "broomstick_calibration"
#define CALIBRATION_EEPROM_SIZE CALIBRATION_EEPROM_ADDRESS+sizeof(CALIBRATION_EEPROM_FLAG)+sizeof(Broomstick::calib)


//BLE CONFIGURATION
#define USE_BLE
#define BLE_NAME "Broomstick"
#define BLE_MANUFACTURER "Tech'ISEN"
#define WAIT_UNTIL_CONNECTED
#define CONNECTION_TIMEOUT 10000

//GAMEPAD CONFIGURATION
#define numOfButtons 28
#define numOfHatSwitches 0
#define enableX true
#define enableY true
#define enableZ false
#define enableRZ false
#define enableRX true
#define enableRY true
#define enableSlider1 false
#define enableSlider2 false
#define enableRudder false
#define enableThrottle true
#define enableAccelerator false
#define enableBrake false
#define enableSteering false

//analogicPad confiduration
#define ANAL_PAD_NUMBER_OF_BUTTONS 3
const int ANAL_PAD_THRES[8] = {1500, 2068, 2225, 2405, 2698, 3048, 3460, 3908};
#define PAD_CHANNEL 26

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
    int readPannel();
    int applyCorrection();
    int compute();
    int sendBluetooth();
    int beginBluetooth();
    int communicate();

    struct Data
    {
        int main_joy_x;
        int main_joy_y;
        int throttle;
        bool brake_;
        bool enable_pilot_auto;
        bool enable_battery;
        bool enable_engine;
        bool disable_pilot_auto;
        bool disable_battery;
        bool eject;
        bool fumigene;
        bool feux_de_position;
        bool bp1;
        bool bp2;
        bool bp3;
        bool enable_trains_datterrissage;
        bool disable_trains_datterrissage;
        bool aerofrein_up;
        bool aerofrein_middle;
        bool aerofrein_down;
        bool volet_up;
        bool volet_middle;
        bool volet_down;
        bool parachute;
        bool fire;//25
        int rotation_x;//30
        int rotation_y;//24
        bool cycle_arm;//29
        bool cycle_mode;//31
        bool verouillage_tele;//27
        bool contre_mesure;//pad-1
        bool bp4;//pad-2
        bool bp5;//pad-0
    };

    struct Data data,previous_data;
    struct Calib calib;
    low_pass_filter main_joy_x_filter,main_joy_y_filter,throttle_filter;
    bool correction_done=false;
    unsigned long last_update_time=0;
    unsigned long last_send_time=0;
    unsigned long time=0;
    unsigned long dt=0;

    BleGamepad bleGamepad;
    BleGamepadConfiguration bleGamepadConfig;
    demultiplexer demu=demultiplexer(SELECTOR_PIN, VALUE_PIN, COMPLEXITY, NUMBER_OF_DEMU);
    analPad pad=analPad(ANAL_PAD_THRES, ANAL_PAD_NUMBER_OF_BUTTONS);
};
#endif