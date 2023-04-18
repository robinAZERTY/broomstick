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
enable_battery = 13 (hat switch)
enable_engine = 3 (hat switch)
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
    if (data.throttle != previous_data.throttle)
        bleGamepad.setThrottle(data.throttle);

    if (data.eject != previous_data.eject)
        if (data.eject)
            bleGamepad.press(1);
        else
            bleGamepad.release(1);

    if (data.fumigene != previous_data.fumigene)
        if (data.fumigene)
            bleGamepad.press(2);
        else
            bleGamepad.release(2);

    if (data.feux_de_position != previous_data.feux_de_position)
        if (data.feux_de_position)
            bleGamepad.press(3);
        else
            bleGamepad.release(3);

    if (data.bp1 != previous_data.bp1)
        if (data.bp1)
            bleGamepad.press(4);
        else
            bleGamepad.release(4);

    if (data.bp2 != previous_data.bp2)
        if (data.bp2)
            bleGamepad.press(5);
        else
            bleGamepad.release(5);

    if (data.bp3 != previous_data.bp3)
        if (data.bp3)
            bleGamepad.press(6);
        else
            bleGamepad.release(6);

    if (data.aerofrein_up != previous_data.aerofrein_up)
        if (data.aerofrein_up)
            bleGamepad.press(7);
        else
            bleGamepad.release(7);

    if (data.aerofrein_middle != previous_data.aerofrein_middle)
        if (data.aerofrein_middle)
            bleGamepad.press(8);
        else
            bleGamepad.release(8);

    if (data.aerofrein_down != previous_data.aerofrein_down)
        if (data.aerofrein_down)
            bleGamepad.press(9);
        else
            bleGamepad.release(9);

    if (data.volet_up != previous_data.volet_up)
        if (data.volet_up)
            bleGamepad.press(10);
        else
            bleGamepad.release(10);

    if (data.volet_middle != previous_data.volet_middle)
        if (data.volet_middle)
            bleGamepad.press(11);
        else
            bleGamepad.release(11);

    if (data.volet_down != previous_data.volet_down)
        if (data.volet_down)
            bleGamepad.press(12);
        else
            bleGamepad.release(12);

    if (data.fire != previous_data.fire)
        if (data.fire)
            bleGamepad.press(13);
        else
            bleGamepad.release(13);

    if (data.brake_ != previous_data.brake_)
        if (data.brake_)
            bleGamepad.press(14);
        else
            bleGamepad.release(14);

    if (data.enable_pilot_auto != previous_data.enable_pilot_auto)
        if (data.enable_pilot_auto)
        {
            bleGamepad.press(15);
            bleGamepad.release(16);
        }
        else
        {
            bleGamepad.release(15);
            bleGamepad.press(16);
        }

    if (data.enable_battery != previous_data.enable_battery)
        if (data.enable_battery)
        {
            bleGamepad.press(17);
            bleGamepad.release(18);
        }
        else
        {
            bleGamepad.release(17);
            bleGamepad.press(18);
        }
    
    if (data.enable_engine != previous_data.enable_engine)
        if (data.enable_engine)
        {
            bleGamepad.press(19);
            bleGamepad.release(20);
        }
        else
        {
            bleGamepad.release(19);
            bleGamepad.press(20);
        }

    if (data.enable_trains_datterrissage != previous_data.enable_trains_datterrissage)
        if (data.enable_trains_datterrissage)
        {
            bleGamepad.press(21);
            bleGamepad.release(22);
        }
        else
        {
            bleGamepad.release(21);
            bleGamepad.press(22);
        }

        if(data.rotation_x != previous_data.rotation_x)
            bleGamepad.setRX(data.rotation_x);

        if(data.rotation_y != previous_data.rotation_y)
            bleGamepad.setRY(data.rotation_y);
        
        if(data.cycle_arm!=previous_data.cycle_arm)
            if(data.cycle_arm)
                bleGamepad.press(23);
            else
                bleGamepad.release(23);
        if(data.cycle_mode!=previous_data.cycle_mode)
            if(data.cycle_mode)
                bleGamepad.press(24);
            else
                bleGamepad.release(24);
        if(data.verouillage_tele!=previous_data.verouillage_tele)
            if(data.verouillage_tele)
                bleGamepad.press(25);
            else
                bleGamepad.release(25);
        if(data.contre_mesure!=previous_data.contre_mesure)
            if(data.contre_mesure)
                bleGamepad.press(26);
            else
                bleGamepad.release(26);
        if(data.bp4!=previous_data.bp4)
            if(data.bp4)
                bleGamepad.press(27);
            else
                bleGamepad.release(27);
        if(data.bp5!=previous_data.bp5)
            if(data.bp5)
                bleGamepad.press(28);
            else
                bleGamepad.release(28);

    bleGamepad.sendReport();
    previous_data = data;
    return 1;
}