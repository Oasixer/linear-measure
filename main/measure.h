// defines pins numbers
const int STEP_PIN = 3; 
const int DIRECTION_PIN = 4; 
const int MOTOR_SPEED_PIN = 5;
const int SWITCH_PIN = 6;

long mSteps=0;

const int STEPS_PER_MM = 200;
const int MICROSTEPS_PER_STEP = 16;
const int MICROSTEPS_PER_MM = STEPS_PER_MM * MICROSTEPS_PER_STEP;
const float MM_PER_MICROSTEP = 1.0 / MICROSTEPS_PER_MM;
const long numMStepsToBackUpALittle = 2L*MICROSTEPS_PER_MM;
const long numMStepsToBackUpALot = 50L*MICROSTEPS_PER_MM;

long forwardUntilHit(bool slow){
  long mStepsForward = 0;
  digitalWrite(DIRECTION_PIN,HIGH);
  if (slow){
    digitalWrite(MOTOR_SPEED_PIN,HIGH);
  }
  else{
    digitalWrite(MOTOR_SPEED_PIN,LOW);
  }
  delay(100);
  while(digitalRead(SWITCH_PIN)==0){
    digitalWrite(STEP_PIN, HIGH);
    if (slow){
      delayMicroseconds(300);
      mStepsForward++;
    }
    else{
      delayMicroseconds(500);
      mStepsForward+=16;
    }
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(slow?300:500);
  }
  return mStepsForward;
}

long backUp(long numMStepsToBackUp){
  long mStepsBackwards = 0;
  delay(100);
  digitalWrite(MOTOR_SPEED_PIN, LOW);
  delay(100);
  digitalWrite(DIRECTION_PIN, LOW);
  delay(100);
  Serial.print("Backing up for: ");
  Serial.println(numMStepsToBackUp);
  delay(100);
  while(mStepsBackwards < numMStepsToBackUp){
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
    mStepsBackwards+=16;
    // Dont print here, print statements are so heavy computationally that it slows everything down
    // if (mStepsBackwards % 1024 == 0){
      // Serial.print("Backing up: cur=");
      // Serial.println(mStepsBackwards);
      // Serial.print(" of ");
      // Serial.println(numMStepsToBackUp);
    // }
  }
  return mStepsBackwards;
}

long measure(){
  Serial.println("Starting measure()");
  Serial.print("mSteps = ");
  Serial.println(mSteps);
  long mStepsToFirstTouch = forwardUntilHit(false);

  mSteps -= mStepsToFirstTouch;

  Serial.print("mStepsToFirstTouch = ");
  Serial.println(mStepsToFirstTouch);
  
  Serial.print("mSteps = ");
  Serial.println(mSteps);
  
  delay(500);

  long mStepsBackwards = backUp(numMStepsToBackUpALittle);

  Serial.print("mStepsBackwards = ");
  Serial.println(mStepsBackwards);

  mSteps+=mStepsBackwards;

  Serial.print("total mSteps (after backing up) = ");
  Serial.println(mSteps);

  delay(500);
  long mStepsToSecondTouch = forwardUntilHit(true);

  Serial.print("mStepsToSecondTouch = ");
  Serial.println(mStepsToSecondTouch);

  mSteps -= mStepsToSecondTouch;

  Serial.print("total mSteps (as of pushing limit switch in) = ");
  Serial.println(mSteps);
  
  delay(200);

  return mSteps;
}

void home(){
  // secretly just uses measure
  Serial.println("starting home");
  long howFarFromHomeWereWe = measure(); // Should be some negative value
  Serial.print("We were ");
  Serial.print(howFarFromHomeWereWe);
  Serial.println(" mSteps from home");
  mSteps=0;
  Serial.println("Resetting mSteps to 0, THEN backing up a lot");
  delay(500);
  backUp(numMStepsToBackUpALot);
  mSteps += numMStepsToBackUpALot;
  Serial.print("Finished home procedure. mSteps is now ");
  Serial.println(mSteps);
}

