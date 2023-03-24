
#include <Arduino.h>
#include "broomstick.hpp"
/*
A changer dans la pcb
NE PAS UTILISER LE PIN 12 , on décale*/
//#define FORCE_CALIBRATION
Broomstick broomstick;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  switch (broomstick.begin())
  {
  case -1:
    Serial.println("EEPROM error");
    break;
  case 0:
    Serial.println("No calibration found in EEPROM");
    broomstick.centerCalibration();
    broomstick.extremumCalibration();
    if (broomstick.saveCalibration()<0)
    {
      Serial.println("error saving calibration");
      while(1);
    };
    break;
  case 1:
    Serial.println("Calibration found");
    break;
  }
#ifdef FORCE_CALIBRATION
  broomstick.centerCalibration();
  broomstick.extremumCalibration();
  broomstick.saveCalibration();
#endif
  broomstick.printCalibration();
}

void loop()
{
 broomstick.run();
}

/*
#include <Arduino.h>
#include "demultiplexer.hpp"

#define COMPLEXITY 3
int SELECTOR_PINS[COMPLEXITY] = {15,2,4};
#define NUMBER_OF_DEMU 3
int VALUE_PIN[NUMBER_OF_DEMU] = {5,18,19};



demultiplexer demu=demultiplexer(SELECTOR_PINS, VALUE_PIN, COMPLEXITY, NUMBER_OF_DEMU);

void setup()
{
Serial.begin(115200);
}
void loop()
{
  demu.update();
  for (int i = 0; i < pow(2, COMPLEXITY)*NUMBER_OF_DEMU; i++)
  {
    Serial.print(String(i) + ":" + String(demu.get_value(i)) + ",");
  }
  Serial.println();
delay(100);

}
*/
/*
PIN CONNECTIONS
Button || channel
pilot auto = 8
battery = 13
engine = 3
Eject = 14
fumigène  = 15
feux de position = 1
Bp1 = 10
Bp2 = ?
Bp3 = 7
trains d'atterrissage = 12
Aérofrein up = 11
Aérofrein middle = 21
Aérofrein down = 19
volet up = 5
volet middle = 23
volet down = 17

libres = 2,4,6,9,16,18,20,22
*/