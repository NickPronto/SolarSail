enum LinearActuator {up, down, off};
LinearActuator linearActuatorState;
enum MagLock {lock, unlock};
MagLock leftState;
MagLock rightState;

//variables
int lightSensitivity = 10; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge magLocks.

//input pins
const int rightLightSensor = A4; // left light sensor
const int leftLightSensor = A5; //right light sensor
const int ignitionSwitch = 4; // 12v ignition switch pin, high when vechile ignition is in "run"
const int windSpeed = 3;

//output pins
const int linearActuatorRelay1 = 9; // linear actuator 12v relay to extend
const int linearActuatorRelay2 = 10; // linear actuator 12v relay to retract
const int magLockLeft = 7; // solenoid control 12v relay left hinge
const int magLockRight = 8; // solenoid control 12v relay right hinge

// Values
int rightLightValue;
int leftLightValue;
int ignitionSwitchVal;
unsigned long currentMillis = millis();
int previousMillis;
long interval = 1000000; // time to wait till turning off parasitic MagLocks off at night. Vehicle must be off to disengage.


void setup() {
  attachInterrupt(digitalPinToInterrupt(ignitionSwitch), layFlat, RISING);
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(linearActuatorRelay1, OUTPUT);
  pinMode(linearActuatorRelay2, OUTPUT);
  pinMode(magLockLeft, OUTPUT);
  pinMode(magLockRight, OUTPUT);
  pinMode(leftLightSensor, INPUT);
  pinMode(rightLightSensor, INPUT);
  pinMode(ignitionSwitch, INPUT);
  pinMode(windSpeed, INPUT);
}

void loop() {
  sensorRead();
  if (ignitionSwitchVal < HIGH) { // only works if the ignition is off
    if (leftLightValue + rightLightValue > lightSensitivity*multiplier) { //only works if there is sunlight
      if (leftLightValue > rightLightValue) {
        trackLeftHigh();
        lowerLeftPanel();
      }
      else if (leftLightValue < rightLightValue) {
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
  ignitionSwitchVal = digitalRead(ignitionSwitch); // ignition or kill switch
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

void magLockSwitch(int leftState, int rightState) {
  switch (leftState) {
    case lock:
      digitalWrite(magLockLeft, HIGH);
      break;
    case unlock:
      digitalWrite(magLockLeft, LOW);
      break;
    default:
      digitalWrite(magLockLeft, HIGH);
      break;

      switch (rightState) {
        case lock:
          digitalWrite(magLockRight, HIGH);
          break;
        case unlock:
          digitalWrite(magLockRight, LOW);
          break;
        default:
          digitalWrite(magLockRight, HIGH);
          break;
      }
  }
}

void LinearActuatorSwitch(int linearActuatorState) {
  switch (linearActuatorState) {
    case up:
      digitalWrite(linearActuatorRelay1, HIGH);
      digitalWrite(linearActuatorRelay2, HIGH);
      break;

    case down:
      digitalWrite(linearActuatorRelay1, LOW);
      digitalWrite(linearActuatorRelay2, LOW);
      break;

    case off:
      digitalWrite(linearActuatorRelay1, HIGH);
      digitalWrite(linearActuatorRelay2, LOW);
      break;

    default:
      digitalWrite(linearActuatorRelay1, HIGH);
      digitalWrite(linearActuatorRelay2, LOW);
      break;
  }
}

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  LinearActuatorSwitch(down);
  magLockSwitch(lock, lock);
  if (currentMillis - previousMillis > interval && ignitionSwitchVal == LOW) { // turn off parasitic drain from MagLocks on batteries at night when panels are lowered.
    previousMillis = currentMillis;
    magLockSwitch(unlock, unlock);
  }
}

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
  magLockSwitch(lock, unlock);
  LinearActuatorSwitch(up);
  while (leftLightValue >= rightLightValue) {
    sensorRead();
  }
  LinearActuatorSwitch(off);
}

void lowerLeftPanel() {
  magLockSwitch(lock, lock);
  if (leftLightValue < multiplier * rightLightValue) {
    trackRightHigh();
  }
  while (leftLightValue <= rightLightValue) {
    LinearActuatorSwitch(down);
  }
  LinearActuatorSwitch(off);
}

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
  magLockSwitch(unlock, lock);
  LinearActuatorSwitch(up);
  while (leftLightValue <= rightLightValue) {
    sensorRead();
  }
  LinearActuatorSwitch(off);
}

void lowerRightPanel()
{
  magLockSwitch(lock, lock);
  if (leftLightValue < multiplier * rightLightValue) {
    trackRightHigh();
  }
  while (leftLightValue <= rightLightValue) {
    LinearActuatorSwitch(down);
  }
  LinearActuatorSwitch(off);
}
