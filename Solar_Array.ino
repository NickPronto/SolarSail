#include <Wire.h>
#include <BH1750FVI.h>

BH1750FVI LightSensor;

enum LinearActuator {up, down, off};
LinearActuator linearActuatorState;
enum MagLock {lock, unlock};
MagLock leftState;
MagLock rightState;

//variables
int lightSensitivity = 100; // adjustable light sensitivity based on ambient light at night
int manualToggle = 1; //app input for manual mode: 2 "Manual", 1 "Automatic", default automatic, for non-app users.

//input pins
const int rightLightSensor = 22; // left light sensor
const int leftLightSensor = 21; //right light sensor
const int magLockRightSense = 25;
const int magLockLeftSense = 26;
const int ignitionSwitch = 34; // 12v ignition switch pin, high when vechile ignition is in "run"


//output pins
const int linearActuatorRelay1 = 4; // linear actuator 12v relay to extend
const int linearActuatorRelay2 = 2; // linear actuator 12v relay to retract
const int magLockLeft = 16; // solenoid control 12v relay left hinge
const int magLockRight = 17; // solenoid control 12v relay right hinge

// Values
uint16_t leftLightValue;
uint16_t rightLightValue;
int panelMove;

int ignitionSwitchVal;
unsigned long currentMillis = millis();
int previousMillis;
long interval = 10; // Minutes to wait till turning off parasitic MagLocks off at night. Vehicle must be off to disengage.
//long delayInterval = 1; //minutes to wait till checking/changing angle


void setup() {
  attachInterrupt(digitalPinToInterrupt(ignitionSwitch), layFlat, RISING);
  LightSensor.begin();
  LightSensor.SetMode(OneTime_L_resolution_Mode);
  Serial.begin(115200);

  pinMode(magLockRightSense, INPUT_PULLUP);
  pinMode(magLockLeftSense, INPUT_PULLUP);
  pinMode(linearActuatorRelay1, OUTPUT);
  pinMode(linearActuatorRelay2, OUTPUT);
  pinMode(magLockLeft, OUTPUT);
  pinMode(magLockRight, OUTPUT);
  pinMode(leftLightSensor, INPUT);
  pinMode(rightLightSensor, INPUT);
  pinMode(ignitionSwitch, INPUT);

}

void loop() {
  sensorRead();
  if (ignitionSwitchVal < HIGH) { // only works if the ignition is off
    if (leftLightValue + rightLightValue > lightSensitivity) { //only works if there is sunlight
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
void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  LinearActuatorSwitch(down);
  magLockSwitch(lock, lock);
  if (currentMillis - previousMillis > (interval * 60000) && ignitionSwitchVal == LOW) { // turn off parasitic drain from MagLocks on batteries at night when panels are lowered.
    previousMillis = currentMillis;
    magLockSwitch(unlock, unlock);
  }
}


void manualMode() {
  switch (panelMove) {
    case 1:
      leftLightValue = 1 + lightSensitivity;
      rightLightValue = 0;
      break;
    case 2:
      rightLightValue = 1 + lightSensitivity;
      leftLightValue = 0;
      break;
    case 3:
      rightLightValue = leftLightValue;
      break;
    case 4:
      layFlat();
      break;
    default:
      layFlat();
  }
}

void sensorRead()
{
  ignitionSwitchVal = digitalRead(ignitionSwitch); // ignition or kill switch
  if (manualToggle = 2) {
    manualMode();
  }
  else {
    LightSensor.SetAddress(Device_Address_H);
    leftLightValue = LightSensor.GetLightIntensity();// Get Lux value left
    LightSensor.SetAddress(Device_Address_L);
    rightLightValue = LightSensor.GetLightIntensity();// Get Lux value right
  }
}
void printOut()
{
  Serial.print("Ignition on: ");
  Serial.println(ignitionSwitchVal);
  Serial.print("Left light sensor reading: ");
  Serial.println(leftLightValue);
  Serial.print("Right light sensor reading: ");
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
  if (magLockRightSense == HIGH) {
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
  if (magLockLeftSense == HIGH) {
    trackLeftHigh();
  }
  while (leftLightValue <= rightLightValue) {
    LinearActuatorSwitch(down);
  }
  LinearActuatorSwitch(off);
}
