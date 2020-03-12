/* 

| home |{object being measured}   /[lsw]------screw-----------(motor)
0
 */
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int msPin = 5;
const int limSwitchPin = 6;

int mSteps=0;

const int numMStepsToBackUpALittle = 200*16;
const int numMStepsToBackUpALot = 2000*16;
const float MM_PER_MICROSTEP = 1234567; // TODO update

char getNextCharInput(){
  char c = '';
  while(c != endOfPacketMarker)
  {
     if(Serial.available() > 0)
     {
        c = Serial.read();
     }
  }
  return c;
}
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(msPin,OUTPUT);
  pinMode(limSwitchPin,INPUT);
}

int forwardUntilHit(bool slow){
  digitalWrite(dirPin,LOW);
  if (slow){
    digitalWrite(msPin,HIGH);
  }
  else{
    digitalWrite(msPin,LOW);
  }
  while(digitalRead(limSwitchPin)==LOW){
    digitalWrite(stepPin, HIGH);
    if (slow){
      delayMicroseconds(100);
      mSteps--;
    }
    else{
      delayMicroseconds(500);
      mSteps-=16;
    }
  }
  return mSteps;
}

int backUp(numMStepsToBackUp){
  int mStepsBackwards = 0;
  digitalWrite(msPin,LOW);
  digitalWrite(dirPin,HIGH);
  while(mStepsBackwards < numMStepsToBackUp){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    mStepsBackwards+=16;
  }
  return mStepsBackwards;
}

void home(){
  // secretly just uses measure
  Serial.println("starting home");
  int howFarFromHomeWereWe = measure();
  Serial.print("We were ");
  Serial.print(howFarFromHomeWereWe);
  Serial.print(" mSteps from home");
  mSteps=0;
}

int measure(){
  Serial.println("Starting measure()");
  Serial.print("mSteps = ");
  Serial.println(mSteps);

  int mStepsToFirstTouch = forwardUntilHit(false);

  mSteps-=mStepsToFirstTouch;

  Serial.print("mStepsToFirstTouch = ");
  Serial.println(mStepsToFirstTouch);
  
  Serial.print("mSteps = ");
  Serial.println(mSteps);

  int mStepsBackwards = backUpABit(numMStepsToBackUpALittle);

  Serial.print("mStepsBackwards = ");
  Serial.println(mStepsBackwards);

  mSteps+=mStepsBackwards;

  Serial.print("total mSteps (after backing up) = ");
  Serial.println(mSteps);

  int mStepsToSecondTouch = forwardUntilHit(true);

  Serial.print("mStepsToSecondTouch = ");
  Serial.println(mStepsToSecondTouch);

  mSteps -= mStepsToSecondTouch;

  Serial.print("total mSteps (final) = ");
  Serial.println(mSteps);
  
  return mSteps;
}

void loop() {
  char input == '';
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
  }
  return;
}
