//constants
unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long currentCount = 0;
const long intervalLong = 1000; // delay period in between lowering steps.
const long intervalShort = 50000; // delay period in between lowering steps.

enum LA{up,down,off};
enum LA LAstate;
enum magLock {pull, noPull};
enum magLock leftState;
enum magLock rightState;

//variables 
int lightSense = 10; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
const int rightLDR = A4;
const int leftLDR = A5;
const int ignitionSwitch = 4;
const int magLockleft = 6;
const int magLockright = 5;
const int LASense = 3;


//output pins
const int laRelayDown = 10; // linear actuator 12v relay to extend
const int laRelayUp = 11; // linear actuator 12v relay to retract 
const int leftSLND = 7; // solenoid control 12v relay left hinge
const int rightSLND = 8; // solenoid control 12v relay right hinge

// Values
int rightLDRVal;
int leftLDRVal;
int ignitionSwitchVal;
int LASenseCount;



void setup()
{
Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(laRelayDown, OUTPUT);
  pinMode(laRelayUp, OUTPUT);
  pinMode(leftSLND, OUTPUT);
  pinMode(rightSLND, OUTPUT);
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
layFlat();
  if(leftLDRVal+rightLDRVal>lightSense){ //only works anything if there is sunlight
     sensorRead();

      if(leftLDRVal>rightLDRVal){
          sensorRead();
          trackLeftHigh();
       }
     lowerLeftPanel();
  }
else if (leftLDRVal<rightLDRVal){
  sensorRead();
  trackRightHigh();
}
lowerRightPanel();
  }
  


else {
LAstate=off;
LASwitch();
}


}


//-------Functions below-----------

void LASenseReset()
{
  if(LASenseCount > 0){
    currentCount = currentMillis;
  }
}

void sensorRead()
{
  
rightLDRVal = analogRead(rightLDR); // right LDR sensor input pin
leftLDRVal = analogRead(leftLDR); // left LDR sensor input pin
ignitionSwitchVal = digitalRead(ignitionSwitchVal); // ignition or kill switch
LASenseCount = digitalRead(LASense);
LASenseReset();
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
    digitalWrite(leftSLND,HIGH);
    break;
    case noPull:
    digitalWrite(leftSLND,LOW);
    break;
    default:
    digitalWrite(leftSLND,HIGH);
    break;

  switch(rightState){
    case pull:
    digitalWrite(rightSLND,HIGH);
    break;
    case noPull:
    digitalWrite(rightSLND,LOW);
    break;
    default:
    digitalWrite(rightSLND,HIGH);
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
  if(LASenseCount<intervalLong && leftLDRVal<=rightLDRVal){
    LAstate=down;
    LASwitch();
    sensorRead();
  }
  else {
    trackRightHigh();
  }

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
  if(LASenseCount<intervalLong && leftLDRVal>=rightLDRVal){
    LAstate=down;
    LASwitch();
    sensorRead();
  }
  else {
    trackLeftHigh();
  }

}




