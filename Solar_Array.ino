
enum LA{up,down,off};
enum LA LAstate;
enum magLock {pull, noPull};
enum magLock leftState;
enum magLock rightState;

//variables 
int lightSense = 10; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
const int rightLDR = A4; // left light sensor
const int leftLDR = A5; //right light sensor
const int ignitionSwitch = 4; // 12v ignition switch pin, high when vechile ignition is in "run"


//output pins
const int laRelayDown = 10; // linear actuator 12v relay to extend
const int laRelayUp = 11; // linear actuator 12v relay to retract 
const int magLockleft = 7; // solenoid control 12v relay left hinge
const int magLockright = 8; // solenoid control 12v relay right hinge

// Values
int rightLDRVal;
int leftLDRVal;
int ignitionSwitchVal;



void setup()
{
Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(laRelayDown, OUTPUT);
  pinMode(laRelayUp, OUTPUT);
  pinMode(magLockleft, OUTPUT);
  pinMode(magLockright, OUTPUT);
  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
  pinMode(ignitionSwitch, INPUT);
  pinMode(magLockleft, INPUT);
  pinMode(magLockright, INPUT);
  sensorRead();
}

void loop(){

sensorRead();

attachInterrupt(digitalPinToInterrupt(4), layFlat, RISING);

if(ignitionSwitchVal == LOW){ // only works if the ignition is off
sensorRead();

  if(leftLDRVal+rightLDRVal>lightSense){ //only works anything if there is sunlight
     sensorRead();

      if(leftLDRVal>rightLDRVal){
          sensorRead();
          trackLeftHigh();
          lowerLeftPanel();
      }
      else if (leftLDRVal<rightLDRVal){
         sensorRead();
         trackRightHigh();
         lowerRightPanel();
      }
  }
  }
  


else {
layFlat();
}


}


//-------Functions below-----------



void sensorRead()
{
ignitionSwitchVal = digitalRead(ignitionSwitchVal); // ignition or kill switch  
rightLDRVal = analogRead(rightLDR); // right LDR sensor input pin
leftLDRVal = analogRead(leftLDR); // left LDR sensor input pin

}

void printOut()
{
Serial.print("Ignition on: ");
Serial.println(ignitionSwitchVal);
Serial.print("Left Analog reading: ");
Serial.println(leftLDRVal);
Serial.print("Right Analog reading: ");
Serial.println(rightLDRVal);
Serial.println();
Serial.println();

}
void magLockSwitch(){
  switch(leftState){
    case pull:
    digitalWrite(magLockleft,HIGH);
    break;
    case noPull:
    digitalWrite(magLockleft,LOW);
    break;
    default:
    digitalWrite(magLockleft,HIGH);
    break;

  switch(rightState){
    case pull:
    digitalWrite(magLockright,HIGH);
    break;
    case noPull:
    digitalWrite(magLockright,LOW);
    break;
    default:
    digitalWrite(magLockright,HIGH);
    break;
  }
  }
  }
 
void LASwitch(){
  
  switch (LAstate){
    case up:
    digitalWrite(laRelayDown,LOW);
    digitalWrite(laRelayUp,HIGH);
    break;

    case down:
    digitalWrite(laRelayUp,LOW);
    digitalWrite(laRelayDown,HIGH);
    break;

    case off:
    digitalWrite(laRelayUp,LOW);
    digitalWrite(laRelayDown,LOW);

    break;

    default:
    digitalWrite(laRelayUp,LOW);
    digitalWrite(laRelayDown,LOW);
    break;

  }
  }

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
    LAstate=down;
    LASwitch();
    leftState=pull;
    rightState=pull;
    magLockSwitch();
    sensorRead();

}

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
    leftState=pull;
    rightState=noPull;
    magLockSwitch();
    LAstate=up;
    LASwitch();
    while (leftLDRVal>=rightLDRVal){
    sensorRead();
    }
    LAstate=off;
    LASwitch();
}
void lowerLeftPanel() {
  sensorRead();
  rightState=pull;
  magLockSwitch();
 if (leftLDRVal<multiplier*rightLDRVal){
  trackRightHigh();
 }
  else if(leftLDRVal<=rightLDRVal){
    LAstate=down;
    LASwitch();
    sensorRead();
  }
  else  {
    sensorRead();
  }
  LAstate=off;
  LASwitch();
  sensorRead();

}


void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
    leftState=noPull;
    rightState=pull;
    magLockSwitch();
    LAstate=up;
    LASwitch();
    while (leftLDRVal<=rightLDRVal){
    sensorRead();
    }
    LAstate=off;
    LASwitch();
}

void lowerRightPanel() {
  sensorRead();
  rightState=pull;
  magLockSwitch();
 if (leftLDRVal<multiplier*rightLDRVal){
  trackRightHigh();
 }
  else if(leftLDRVal<=rightLDRVal){
    LAstate=down;
    LASwitch();
    sensorRead();
  }
  else {
    sensorRead();
  }
  LAstate=off;
  LASwitch();
  sensorRead();
}




