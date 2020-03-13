// defines pins numbers
const int STEP_PIN = 3; 
const int DIRECTION_PIN = 4; 
const int MOTOR_SPEED_PIN = 5;
const int SWITCH_PIN = 6;

int mSteps=0;

const int numMStepsToBackUpALittle = 200*16;
const int numMStepsToBackUpALot = 2000*16;
const float MM_PER_MICROSTEP = 1234567; // TODO update

int forwardUntilHit(bool slow){
  digitalWrite(DIRECTION_PIN,LOW);
  if (slow){
    digitalWrite(MOTOR_SPEED_PIN,HIGH);
  }
  else{
    digitalWrite(MOTOR_SPEED_PIN,LOW);
  }
  while(digitalRead(SWITCH_PIN)==LOW){
    digitalWrite(STEP_PIN, HIGH);
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

int backUp(int numMStepsToBackUp){
  int mStepsBackwards = 0;
  digitalWrite(MOTOR_SPEED_PIN,LOW);
  digitalWrite(DIRECTION_PIN,HIGH);
  while(mStepsBackwards < numMStepsToBackUp){
    digitalWrite(STEP_PIN, HIGH);
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
  
  delay(500);

  int mStepsBackwards = backUpABit(numMStepsToBackUpALittle);

  Serial.print("mStepsBackwards = ");
  Serial.println(mStepsBackwards);

  mSteps+=mStepsBackwards;

  Serial.print("total mSteps (after backing up) = ");
  Serial.println(mSteps);

  delay(500);
  int mStepsToSecondTouch = forwardUntilHit(true);

  Serial.print("mStepsToSecondTouch = ");
  Serial.println(mStepsToSecondTouch);

  mSteps -= mStepsToSecondTouch;

  Serial.print("total mSteps (final) = ");
  Serial.println(mSteps);
  
  delay(200);
  return mSteps;
}
