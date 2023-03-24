#include "broomstick.hpp"

int Broomstick::communicate()
{
    if (time - last_send_time >= 1000000 / SENDING_RATE)
    {
        last_send_time = time;
        readPannel();
// print data on serial if PRINT_ON_SERIAL is defined
#ifdef PRINT_ON_SERIAL
        printData();
#endif
// send data over bluetooth if BLUETOOTH is defined
#ifdef USE_BLE
        if (sendBluetooth() < 0)
            return -1; // error during bluetooth transmission
#endif

        return 1; // data sent
    }
    return 0; // data not sent because not enough time elapsed
}

int Broomstick::beginBluetooth()
{   
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_MULTI_AXIS); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepadConfig.setIncludeStart(true);
    bleGamepadConfig.setIncludeSelect(true);
    bleGamepadConfig.setWhichAxes(enableX, enableY, enableZ, enableRX, enableRY, enableRZ, enableSlider1, enableSlider2);      // Can also be done per-axis individually. All are true by default
    bleGamepadConfig.setWhichSimulationControls(enableRudder, enableThrottle, enableAccelerator, enableBrake, enableSteering); // Can also be done per-control individually. All are false by default
    bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);      

    
    
    // you can initialize bluetooth here
    // return a negative value if an error occured
    bleGamepad = BleGamepad(BLE_NAME, BLE_MANUFACTURER);
    bleGamepad.begin(&bleGamepadConfig);
    unsigned long t0 = millis();
#ifdef WAIT_UNTIL_CONNECTED
    while (!bleGamepad.isConnected())
    {
        if (millis() - t0 > CONNECTION_TIMEOUT)
            return -1; // timeout
    }
#endif
    return 1;
}

int Broomstick::sendBluetooth()
{
    // you can access to data to send with the struct data of the Broomstick object. Example : data.main_joy_x
    // return a negative value if an error occured
    if (!bleGamepad.isConnected())
        return -1; // not connected
    /*
PIN CONNECTIONS
Button || channel
pilot auto = 8 (hat switch)
battery = 13 (hat switch)
engine = 3 (hat switch)
Eject = 14 (button)
fumigène  = 15 (button)
feux de position = 1 (button->hat switch soft)
Bp1 = 10 (button)
Bp2 = ? (button)
Bp3 = 7 (button)
trains d'atterrissage = 12 (hat switch)
Aérofrein up = 11 (button)
Aérofrein middle = 21 (button)
Aérofrein down = 19 (button)
volet up = 5 (button)
volet middle = 23 (button)
volet down = 17 (button)

libres = 2,4,6,9,16,18,20,22
*/
    if (data.main_joy_x != previous_data.main_joy_x)
        bleGamepad.setX(data.main_joy_x);
    if (data.main_joy_y != previous_data.main_joy_y)
        bleGamepad.setY(data.main_joy_y);
    if (data.throttle!=previous_data.throttle)
        bleGamepad.setThrottle(data.throttle);
    
    if (data.pilot_auto!=previous_data.pilot_auto||data.battery!=previous_data.battery||data.engine!=previous_data.engine||data.trains_datterrissage!=previous_data.trains_datterrissage)
        bleGamepad.setHats(data.pilot_auto,data.battery,data.engine,data.trains_datterrissage);

    if (data.eject!=previous_data.eject)
        if (data.eject)
            bleGamepad.press(1);
        else
            bleGamepad.release(1);
    
    if (data.fumigene!=previous_data.fumigene)
        if (data.fumigene)
            bleGamepad.press(2);
        else
            bleGamepad.release(2);
    
    if (data.feux_de_position!=previous_data.feux_de_position)
        if (data.feux_de_position)
            bleGamepad.press(3);
        else
            bleGamepad.release(3);
    
    if (data.bp1!=previous_data.bp1)
        if (data.bp1)
            bleGamepad.press(4);
        else
            bleGamepad.release(4);
    
    if (data.bp2!=previous_data.bp2)
        if (data.bp2)
            bleGamepad.press(5);
        else
            bleGamepad.release(5);
    
    if (data.bp3!=previous_data.bp3)
        if (data.bp3)
            bleGamepad.press(6);
        else
            bleGamepad.release(6);
    
    if (data.aerofrein_up!=previous_data.aerofrein_up)
        if (data.aerofrein_up)
            bleGamepad.press(7);
        else
            bleGamepad.release(7);
    
    if (data.aerofrein_middle!=previous_data.aerofrein_middle)
        if (data.aerofrein_middle)
            bleGamepad.press(8);
        else
            bleGamepad.release(8);
    
    if (data.aerofrein_down!=previous_data.aerofrein_down)
        if (data.aerofrein_down)
            bleGamepad.press(9);
        else
            bleGamepad.release(9);
    
    if (data.volet_up!=previous_data.volet_up)
        if (data.volet_up)
            bleGamepad.press(10);
        else
            bleGamepad.release(10);
    
    if (data.volet_middle!=previous_data.volet_middle)
        if (data.volet_middle)
            bleGamepad.press(11);
        else
            bleGamepad.release(11);
        
    if (data.volet_down!=previous_data.volet_down)
        if (data.volet_down)
            bleGamepad.press(12);
        else
            bleGamepad.release(12);
        
    if (data.fire!=previous_data.fire)
        if (data.fire)
            bleGamepad.press(13);
        else
            bleGamepad.release(13);

    
    if (data.brake_!=previous_data.brake_)
        bleGamepad.setBrake(data.brake_);
            
    
    bleGamepad.sendReport();
    previous_data = data;
    return 1;
}