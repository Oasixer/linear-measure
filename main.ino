/*     Simple Stepper Motor Control Exaple Code
 */
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int msPin = 5;
const int limSwitchPin = 6;

int mSteps=0;

const int numStepsToBackUp = 200;
 
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
      mSteps++;
    }
    else{
      delayMicroseconds(500);
      mSteps+=16;
    }
  }
  return mSteps;
}

int backUpABit(){
  int mStepsBackwards = 0;
  digitalWrite(msPin,LOW);
  digitalWrite(dirPin,HIGH);
  while(mStepsBackwards < numStepsToBackUp){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    mStepsBackwards+=16;
  }
  return mStepsBackwards;
}

int measure(){
  mSteps=0;
  int mStepsToFirstTouch = forwardUntilHit(false);

  mSteps+=mStepsToFirstTouch;

  Serial.print("mStepsToFirstTouch = ");
  Serial.println(mStepsToFirstTouch);

  int mStepsBackwards = backUpABit();

  Serial.print("mStepsBackwards = ");
  Serial.println(mStepsBackwards);

  mSteps-=mStepsBackwards;

  Serial.print("total mSteps (after backing up) = ");
  Serial.println(mSteps);

  int mStepsToSecondTouch = forwardUntilHit(true);

  Serial.print("mStepsToSecondTouch = ");
  Serial.println(mStepsToSecondTouch);

  mSteps += mStepsToSecondTouch;

  Serial.print("total mSteps (total/final) = ");
  Serial.println(mSteps);
  
  return mSteps;
}

void loop() {
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
