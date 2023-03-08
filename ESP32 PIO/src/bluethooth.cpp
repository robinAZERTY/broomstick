#include "broomstick.hpp"

int Broomstick::communicate()
{
    if (time - last_send_time >= 1000000 / SENDING_RATE)
    {
        last_send_time = time;
    //print data on serial if PRINT_ON_SERIAL is defined
    #ifdef PRINT_ON_SERIAL
            printData();
    #endif
    //send data over bluetooth if BLUETOOTH is defined
    #ifdef BLUETOOTH
        if(sendBluetooth()<0)
            return -1;//error during bluetooth transmission
    #endif
        
        return 1; // data sent
    }
    return 0; // data not sent because not enough time elapsed
}

int Broomstick::beginBluetooth()
{
    //you can initialize bluetooth here
    //return a negative value if an error occured
    return -1;//not implemented yet
}

int Broomstick::sendBluetooth()
{
    //you can access to data to send with the struct data of the Broomstick object. Example : data.main_joy_x
    //return a negative value if an error occured
    return -1;//not implemented yet
}