enum LinearActuator{up,down,off};
LinearActuator linearActuatorState;
enum MagLock {pull, noPull};
MagLock leftState;
MagLock rightState; 

//variables 
int lightSensitivity = 10; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.

//input pins
const int rightLightSensor = A4; // left light sensor
const int leftLightSensor = A5; //right light sensor
const int ignitionSwitch = 4; // 12v ignition switch pin, high when vechile ignition is in "run"

//output pins
const int linearActuatorDown = 10; // linear actuator 12v relay to extend
const int linearActuatorUp = 11; // linear actuator 12v relay to retract 
const int magLockLeft = 7; // solenoid control 12v relay left hinge
const int magLockRight = 8; // solenoid control 12v relay right hinge
const int standByPin = 12; 
int standBy = HIGH; //standby pin turn off motor controller when not in use to reduce parasitic drain of battery after interval period.

// Values
int rightLightValue;
int leftLightValue;
int ignitionSwitchVal;
unsigned long currentMillis = millis();
int previousMillis;
long interval = 100000; // time to wait till turning off parasitic MagLocks at night. Vehicle must be off to disengage.


void setup() {
  attachInterrupt(digitalPinToInterrupt(4), layFlat, RISING);
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(linearActuatorDown, OUTPUT);
  pinMode(linearActuatorUp, OUTPUT);
  pinMode(magLockLeft, OUTPUT);
  pinMode(magLockRight, OUTPUT);
  pinMode(leftLightSensor, INPUT);
  pinMode(rightLightSensor, INPUT);
  pinMode(ignitionSwitch, INPUT);
  pinMode(standByPin, INPUT);
}

void loop(){

  sensorRead();

  if(ignitionSwitchVal == LOW){ // only works if the ignition is off
    if(leftLightValue+rightLightValue>lightSensitivity){ //only works if there is sunlight
      if(leftLightValue>rightLightValue){
          trackLeftHigh();
          lowerLeftPanel();
         }
      else if (leftLightValue<rightLightValue){
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
  rightLightValue = analogRead(rightLightSensor); // right LDR sensor input pin
  leftLightValue = analogRead(leftLightSensor); // left LDR sensor input pin
  }

void printOut()
  {
  Serial.print("Ignition on: ");
  Serial.println(ignitionSwitchVal);
  Serial.print("Left Analog reading: ");
  Serial.println(leftLightValue);
  Serial.print("Right Analog reading: ");
  Serial.println(rightLightValue);
  Serial.println();
  Serial.println();
  }
  
void magLockSwitch(int leftState, int rightState){
  switch(leftState){
    case pull:
      digitalWrite(magLockLeft,HIGH);
      break;
    case noPull:
      digitalWrite(magLockLeft,LOW);
      break;
    default:
      digitalWrite(magLockLeft,HIGH);
      break;

  switch(rightState){
    case pull:
      digitalWrite(magLockRight,HIGH);
      break;
    case noPull:
      digitalWrite(magLockRight,LOW);
      break;
    default:
      digitalWrite(magLockRight,HIGH);
      break;
    }
  }
  }
 
void LinearActuatorSwitch(int linearActuatorState){
  
  switch (linearActuatorState){
  case up:
    digitalWrite(linearActuatorDown,LOW);
    digitalWrite(linearActuatorUp,HIGH);
    break;

  case down:
    digitalWrite(linearActuatorUp,LOW);
    digitalWrite(linearActuatorDown,HIGH);
    break;

  case off:
    digitalWrite(linearActuatorUp,LOW);
    digitalWrite(linearActuatorDown,LOW);
    break;

  default:
    digitalWrite(linearActuatorUp,LOW);
    digitalWrite(linearActuatorDown,LOW);
    break;
    }
  }
void standBySwitch(int standBy)
  {
     if (standBy=HIGH){
      digitalWrite(standByPin,HIGH);
      }
     else if (standBy=LOW){
      digitalWrite(standByPin,LOW);
      }
  }

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
  {
    LinearActuatorSwitch(down);
    magLockSwitch(pull,pull);
    if(currentMillis - previousMillis > interval && ignitionSwitchVal == LOW) {  // turn off parasitic drain on batteries at night when panels are lowered.
      previousMillis = currentMillis; 
      magLockSwitch(noPull,noPull); 
      standBySwitch(LOW);
      }
  }

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
  {
    standBySwitch(HIGH);
    magLockSwitch(pull,noPull);
    LinearActuatorSwitch(up);
    while (leftLightValue>=rightLightValue){
      sensorRead();
      }
    LinearActuatorSwitch(off);
  }
void lowerLeftPanel() {
  magLockSwitch(pull,pull);
    if (leftLightValue<multiplier*rightLightValue){
       trackRightHigh();
     }
    else if(leftLightValue<rightLightValue){
     LinearActuatorSwitch(down);
     }
    else {
     }
  LinearActuatorSwitch(off);
  }

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
  {
    standBySwitch(HIGH);
    magLockSwitch(noPull,pull);
    LinearActuatorSwitch(up);
    while (leftLightValue<=rightLightValue){
      sensorRead();
    }
    LinearActuatorSwitch(off);
  }

void lowerRightPanel()
  {
  magLockSwitch(pull,pull);
    if (leftLightValue<multiplier*rightLightValue){
      trackRightHigh();
    }
    else if(leftLightValue<rightLightValue){
      LinearActuatorSwitch(down);
     }
  LinearActuatorSwitch(off);
}
