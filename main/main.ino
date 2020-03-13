/* 

| home |{object being measured}   /[lsw]------screw-----------(motor)
0
 */
#include "measure.h"

void setup() {
  // Sets the two pins as Outputs
  pinMode(STEP_PIN,OUTPUT); 
  pinMode(DIRECTION_PIN,OUTPUT);
  pinMode(MOTOR_SPEED_PIN,OUTPUT);
  pinMode(SWITCH_PIN,INPUT);
}

char getNextCharInput(){
  char c;
  while(c != endOfPacketMarker) {
     if(Serial.available() > 0) {
        c = Serial.read();
     }
  }
  return c;
}

void loop() {
  char input = 'h';
  while(input != 'q'){
    input = getNextCharInput();
    if (input == 'h'){
      home();
      int numMStepsBack = backUpABit(numMStepsToBackUpALot);
      mSteps += numMStepsBack;
    }

    else if (input == 'm'){
      int measurement = measure();
      Serial.print("Made a measurement of ");
      Serial.print(measurement);
      Serial.print(" microsteps which is ");
      Serial.print(measurement*MM_PER_MICROSTEP);
      Serial.println(" mm.")
      Serial.println("Backing up a little then waiting for next input");
      backUpABit(numMStepsToBackUpALittle);
    }
    delay(200);
  }
  return;
}
