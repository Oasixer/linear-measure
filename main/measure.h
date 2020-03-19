// defines pins numbers
const int STEP_PIN = 3; 
const int DIRECTION_PIN = 4; 
const int MOTOR_SPEED_PIN = 5;
const int SWITCH_PIN = 6;

int mSteps=0;

const int STEPS_PER_MM = 200;
const int MICROSTEPS_PER_STEP = 16;
const float MICROSTEPS_PER_MM = STEPS_PER_MM * MICROSTEPS_PER_STEP;
const float MM_PER_MICROSTEP = 1.0 / MICROSTEPS_PER_MM;
const int numMStepsToBackUpALittle = 5*MICROSTEPS_PER_MM;
const int numMStepsToBackUpALot = 150*MICROSTEPS_PER_MM;

int forwardUntilHit(bool slow){
  digitalWrite(DIRECTION_PIN,HIGH);
  if (slow){
    digitalWrite(MOTOR_SPEED_PIN,HIGH);
  }
  else{
    digitalWrite(MOTOR_SPEED_PIN,LOW);
  }
  while(digitalRead(SWITCH_PIN)==0){
    digitalWrite(STEP_PIN, HIGH);
    if (slow){
      delayMicroseconds(300);
      mSteps--;
    }
    else{
      delayMicroseconds(500);
      mSteps-=16;
    }
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(slow?300:500);
  }
  return mSteps;
}

int backUp(int numMStepsToBackUp){
  int mStepsBackwards = 0;
  digitalWrite(MOTOR_SPEED_PIN,LOW);
  digitalWrite(DIRECTION_PIN,LOW);
  while(mStepsBackwards < numMStepsToBackUp){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
    mStepsBackwards+=16;
  }
  return mStepsBackwards;
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

  int mStepsBackwards = backUp(numMStepsToBackUpALittle);

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

void home(){
  // secretly just uses measure
  Serial.println("starting home");
  int howFarFromHomeWereWe = measure();
  Serial.print("We were ");
  Serial.print(howFarFromHomeWereWe);
  Serial.print(" mSteps from home");
  mSteps=0;
}

