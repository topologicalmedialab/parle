/***********************************************************************************
   Firgelli Linear Actuator Control
   Serial Control From Max
   Author Nima Navab
   Last Edit: May8th, 2017
   The following sketch will allow you to control a Firgelli Linear actuator in Max
   Firgelli Mini Linear Actuators http://www.robotgeek.com/store/Search.aspx?SearchTerms=firgelli
   http://www.robotgeek.com/robotgeek-small-workbench.aspx
   Wiring
   30mm Linear Actuator - Digital Pin 9
   Control Behavior:
   0-9 in max will move the linear actuators keeping absolute position.
 ***********************************************************************************/

//Includes
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

//Defines
Adafruit_MCP4725 dac;
#define LINEARPIN 9        //Linear Actuator Digital Pin
//#define HORN1 4    //Horn 1  Digital Pin
#define HORN1 6    //Horn 2  Digital Pin
#define HORN2 7    //Horn 3  Digital Pin
#define HORN3 8   //Horn 3  Digital Pin
//max/min pulse values in microseconds for the linear actuators
#define LINEAR_MIN      1180    // original range 30mm shaft 1050-2000 (bottom flush = 1220) was 1180
#define LINEAR_MAX      1700    // original range 30mm shaft 1050-2000 (top flush = 1700)
Servo LINEAR;  // create servo objects to control the linear actuators
int linearValue = LINEAR_MIN;   //current positional value being sent to the linear actuator.
int speed = 20;
int _header = 0;    // open symbol=4
int _ID = 0;        // which component
int _Val1 = 0;   // incoming value 1050-2000 v1
int _Val2 = 0;   // incoming value 1050-2000 v2
int _Val3 = 0;   // incoming value 1050-2000 v3
int _Val4 = 0;   // incoming value 1050-2000 v5
int _footer = 0;    // closing symbol=44

void setup()
{
  dac.begin(0x62);
  // initialize the serial communication:
  Serial.begin (115200); // was
  //initialize servos
  LINEAR.attach(LINEARPIN, LINEAR_MIN, LINEAR_MAX);      // attaches/activates the linear actuator as a servo object
  pinMode(LINEARPIN, OUTPUT);
  //Digital pin initialization\
  //pinMode(HORN1, OUTPUT);
  pinMode(HORN1, OUTPUT);
  pinMode(HORN2, OUTPUT);
  pinMode(HORN3, OUTPUT);
  //Analog pins do not need to be initialized
  //use the writeMicroseconds to set the linear actuators to their default positions
  LINEAR.writeMicroseconds(linearValue);
}
void loop()
{
  if (Serial.available()) {
    while (Serial.available()) {
      // 4 x 44
      _header = Serial.read();    // header
      if (_header == 4) {
        _ID = Serial.read ();     // ID
        _Val1 = Serial.read(); // Linear Value 1 (1050-2000)
        _Val2 = Serial.read(); // Linear Value 2 (1050-2000)
        _Val3 = Serial.read(); // Linear Value 3 (1050-2000)
        _Val4 = Serial.read(); // Linear Value 4 (1050-2000)
        _footer = Serial.read();
        //FOR Solenoids
        byte bit0 = bitRead(_Val1, 7);
        byte bit1 = bitRead(_Val1, 6);
        byte bit2 = bitRead(_Val1, 5);
        byte bit3 = bitRead(_Val1, 4);
        byte bit4 = bitRead(_Val1, 3);
        byte bit5 = bitRead(_Val1, 2);
        byte bit6 = bitRead(_Val1, 1);
        byte bit7 = bitRead(_Val1, 0);

        if (_footer == 44) {
          if (_ID == 1) {
            String V1 = String(_Val1);
            String V2 = String(_Val2);
            String V3 = String(_Val3);
            String V4 = String(_Val4);
            String stringConcat = V1;
            stringConcat +=  V2;
            stringConcat +=  V3;
            stringConcat +=  V4;
            linearValue = stringConcat.toInt();
            if (linearValue >= LINEAR_MIN && linearValue <= LINEAR_MAX) {
              //use the writeMicroseconds to set the servos to their new positions
              LINEAR.writeMicroseconds(linearValue);
            }   //_linearValue == linearValue
          }//end if ID =1

          //SOLENOIDS
          if (_ID == 2) {
            //if (bit0 == 1)digitalWrite(HORN1, LOW);
            //if (bit0 == 0)digitalWrite(HORN1, HIGH);
            if (bit1 == 1)digitalWrite(HORN1, LOW);
            if (bit1 == 0)digitalWrite(HORN1, HIGH);
            if (bit2 == 1)digitalWrite(HORN2, LOW);
            if (bit2 == 0)digitalWrite(HORN2, HIGH);
            if (bit3 == 1)digitalWrite(HORN3, LOW);
            if (bit3 == 0)digitalWrite(HORN3, HIGH);

          }
          //DAC
          if (_ID == 3) {
            //PPC STUFF
            //assemble all digits back into a long number
            String V1 = String(_Val1);
            String V2 = String(_Val2);
            String V3 = String(_Val3);
            String V4 = String(_Val4);
            String PPC_StringConcat = V1;
            PPC_StringConcat +=  V2;
            PPC_StringConcat +=  V3;
            PPC_StringConcat +=  V4;
            int PPCVAL_1 = PPC_StringConcat.toInt();
            dac.setVoltage(PPCVAL_1, true);
          }
        }     //end if _footer == closing char 44
      }       //end if _header == opening char 4
    }
  }
  delay(30);//IMPORTANT
}



