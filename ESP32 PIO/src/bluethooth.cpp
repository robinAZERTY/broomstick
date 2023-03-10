#include "broomstick.hpp"

int Broomstick::communicate()
{
    if (time - last_send_time >= 1000000 / SENDING_RATE)
    {
        last_send_time = time;
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
    // you can initialize bluetooth here
    // return a negative value if an error occured
    bleGamepad = BleGamepad(BLE_NAME, BLE_MANUFACTURER);
    bleGamepad.begin();
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
    if (data.fire != previous_data.fire)
    {
        if (!data.fire)
        {
            bleGamepad.press(BUTTON_1);
        }
        else
        {
            bleGamepad.release(BUTTON_1);
        }
    }
    if (data.main_joy_x != previous_data.main_joy_x)
    {
        bleGamepad.setX(data.main_joy_x);
        //bleGamepad.setRX(data.main_joy_x);
    }
    if (data.main_joy_y != previous_data.main_joy_y)
    {
        bleGamepad.setY(data.main_joy_y);
        //bleGamepad.setRY(data.main_joy_y);
    }
    bleGamepad.sendReport();
    previous_data = data;
    return 1;
}