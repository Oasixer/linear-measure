/* 

| home |{object being measured}   /[lsw]------screw-----------(motor)
0
 */
#include "measure.h"

void setup() {
  // Sets the two pins as Outputs
  Serial.begin(9600);
  Serial.println("Setup");
  pinMode(STEP_PIN,OUTPUT); 
  pinMode(DIRECTION_PIN,OUTPUT);
  pinMode(MOTOR_SPEED_PIN,OUTPUT);
  pinMode(SWITCH_PIN,INPUT_PULLUP);
}

char getNextCharInput(){
  /* Serial.println("Waiting for input"); */
  char c;
  /* while(c != '\n') { */
  while(Serial.available() == 0) {
  }
  c = Serial.read();
  return c;
}

void loop() {
  char input = 'h';
  while(input != 'q'){
    input = getNextCharInput();
    Serial.print("Input = ");
    Serial.println(input);
    if (input == 't'){
      Serial.println("Starting test mode");
      for(int i=0; i<=5000; i++){
        /* digitalWrite(STEP_PIN, HIGH); */
        /* delayMicroseconds(500); */
        /* digitalWrite(STEP_PIN, LOW); */
        delayMicroseconds(500);
        Serial.println(digitalRead(SWITCH_PIN));
      }
      Serial.println("Done");
    }

    else if (input == 'h'){
      Serial.println("Starting home mode");
      home();
      int numMStepsBack = backUp(numMStepsToBackUpALot);
      mSteps += numMStepsBack;
    }

    else if (input == 'm'){
      Serial.println("Starting measurement mode");
      int measurement = measure();
      Serial.print("Made a measurement of ");
      Serial.print(measurement);
      Serial.print(" microsteps which is ");
      Serial.print(measurement*MM_PER_MICROSTEP);
      Serial.println(" mm.");
      Serial.println("Backing up a little then waiting for next input");
      backUp(numMStepsToBackUpALittle);
    }
    delay(200);
  }
}
