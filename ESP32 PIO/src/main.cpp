// hello wolrd
#include <Arduino.h>
#include "broomstick.hpp"
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
    broomstick.saveCalibration();
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
  // put your main code here, to run repeatedly:
 broomstick.run();
}