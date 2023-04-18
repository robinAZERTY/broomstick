
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
#include "analog_pad.hpp"

#define COMPLEXITY 3
int SELECTOR_PINS[COMPLEXITY] = {15, 2, 4};
#define NUMBER_OF_DEMU 1
int VALUE_PIN[NUMBER_OF_DEMU] = {32};

#define ANAL_PAD_NUMBER_OF_BUTTONS 3
int ANAL_PAD_THRES[8] = {1500, 2068 , 2225, 2405, 2698, 3048, 3460, 3908};

demultiplexer demu = demultiplexer(SELECTOR_PINS, VALUE_PIN, COMPLEXITY, NUMBER_OF_DEMU);
analPad pad = analPad(ANAL_PAD_THRES, ANAL_PAD_NUMBER_OF_BUTTONS);

#define analogPadINDEX 4
#define PAD_CHANNEL 2

void setup()
{
  Serial.begin(115200);
  demu.set_analog(2);
  demu.set_analog(0);
  demu.set_analog(6);
}
void loop()
{
  
  demu.update();
  for (int i = 0; i < pow(2, COMPLEXITY) * NUMBER_OF_DEMU; i++)
  {
    Serial.print(String(i) + ":" + String(demu.get_value(i)) + ",");
  }
  
  pad.update(demu.get_value(PAD_CHANNEL));
  Serial.print("pad():");
  for(int i = 0; i < ANAL_PAD_NUMBER_OF_BUTTONS; i++)
  {
    Serial.print(String(i) + ":" + String(pad.getButton(i)) + ",");
  }
  
  Serial.println();
  delay(50);
}
*/
/*
PIN CONNECTIONS
Button || channel
pilot auto = 10
battery = 15
engine = 21
Eject = 14
fumigène  = 11
feux de position = 17
Bp1 = 8
Bp2 = 9
Bp3 = 19
trains d'atterrissage = 12
Aérofrein up = 13
Aérofrein middle = 7
Aérofrein down = 5
volet up = 23
volet middle = 3
volet down = 1

libres =
*/

/*
for the analogic value of the buttons

  each value correspond to a combination of the buttons
  we count in binary begining by 00..0 to 11..1 (2^n)
  the analogic value increase when the index of the combination increase

  In my case, there are 3 buttons, so there are 8 combinations
  so anal(001) < anal(010) < anal(011) < anal(100) < anal(101) < anal(110) < anal(111)

  Vcc	3,3
  R0	4700
  R1	560
  R2	1200
  R3	2700

  about the simulation
b3	b2	b1	Vout	DAC	diff	thres
0	0	0	1,686184211	2092	2092	
0	0	1	1,796495327	2229	137	2160,5
0	1	0	1,941666667	2409	180	2319
0	1	1	2,089402174	2593	184	2501
1	0	0	2,395327103	2972	379	2782,5
1	0	1	2,624232082	3256	284	3114
1	1	0	2,945977011	3656	400	3456
1	1	1	3,3	4095	439	3875,5

experiemental values
b3	b2	b1	DAC TRESHOLD
0	0	0	1995 
0	0	1	2140  (2140+1995)/2=2067,5
0	1	0	2310  (2310+2140)/2=2225
0	1	1	2500  (2500+2310)/2=2405
1	0	0	2895  (2895+2500)/2=2697,5
1	0	1	3200  (3200+2895)/2=3047,5
1	1	0	3720  (3720+3200)/2=3460
1	1	1	4095 (4095+3720)/2=3907,5

*/
/*
#include <Arduino.h>
#include "demultiplexer.hpp"
#include "analog_pad.hpp"

#define COMPLEXITY 3
int SELECTOR_PINS[COMPLEXITY] = {15, 2, 4};
#define NUMBER_OF_DEMU 4
#define VALUE_PIN new int[NUMBER_OF_DEMU]{5,18,19,32}

#define ANAL_PAD_NUMBER_OF_BUTTONS 3
int ANAL_PAD_THRES[8] = {1500, 2068 , 2225, 2405, 2698, 3048, 3460, 3908};

demultiplexer demu = demultiplexer(SELECTOR_PINS, VALUE_PIN, COMPLEXITY, NUMBER_OF_DEMU);
analPad pad = analPad(ANAL_PAD_THRES, ANAL_PAD_NUMBER_OF_BUTTONS);

#define analogPadINDEX 4
#define PAD_CHANNEL 26

void setup()
{
  Serial.begin(115200);
  demu.set_analog(30);
  demu.set_analog(24);
  demu.set_analog(26);
}
void loop()
{
  
  demu.update();
  for (int i = 0; i < pow(2, COMPLEXITY) * NUMBER_OF_DEMU; i++)
  {
    Serial.print(String(i) + ":" + String(demu.get_value(i)) + ",");
  }
  
  pad.update(demu.get_value(PAD_CHANNEL));
  Serial.print("pad():");
  for(int i = 0; i < ANAL_PAD_NUMBER_OF_BUTTONS; i++)
  {
    Serial.print(String(i) + ":" + String(pad.getButton(i)) + ",");
  }
  
  Serial.println();
  delay(50);
}*/