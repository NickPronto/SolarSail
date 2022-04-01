#include <Wire.h>
#include <BH1750FVI.h>

BH1750FVI LightSensor;

enum LinearActuator {up, down, off};
LinearActuator linearActuatorState;
enum MagLock {lock, unlock};
MagLock leftState;
MagLock rightState;
enum appSignal {left, right, wait, flat};
appSignal panelMove;

//variables
int lightSensitivity = 100; // adjustable light sensitivity based on ambient light at night
int manualToggle; //app input for manual mode: 1 = "Automatic", 2 = "Manual", default automatic, for non-app users.

//input pins
const int rightLightSensor_pin = 22; // left light sensor
const int leftLightSensor_pin = 21; //right light sensor
const int magLockRightSense_pin = 25;
const int magLockLeftSense_pin = 26;
const int ignitionSwitch_pin = 34; // 12v ignition switch pin, high when vechile ignition is in "run"

//output pins
const int linearActuatorRelay1_pin = 4; // linear actuator 12v relay to extend
const int linearActuatorRelay2_pin = 2; // linear actuator 12v relay to retract
const int magLockLeft_pin = 16; // solenoid control 12v relay left hinge
const int magLockRight_pin = 17; // solenoid control 12v relay right hinge

// Values
uint16_t leftLightValue;
uint16_t rightLightValue;
int magLockRightSense;
int magLockLeftSense;

int ignitionSwitchVal;
unsigned long currentMillis = millis();
int previousMillis;
long interval = 10; // Minutes to wait till turning off parasitic MagLocks off at night. Vehicle must be off to disengage.
long delayInterval = 1; //minutes to wait till checking/changing angle


void setup() {
  attachInterrupt(digitalPinToInterrupt(ignitionSwitch_pin), layFlat, RISING);
  LightSensor.begin();
  LightSensor.SetMode(OneTime_L_resolution_Mode);
  Serial.begin(115200);

  pinMode(magLockRightSense_pin, INPUT_PULLUP);
  pinMode(magLockLeftSense_pin, INPUT_PULLUP);
  pinMode(linearActuatorRelay1_pin, OUTPUT);
  pinMode(linearActuatorRelay2_pin, OUTPUT);
  pinMode(magLockLeft_pin, OUTPUT);
  pinMode(magLockRight_pin, OUTPUT);
  pinMode(leftLightSensor_pin, INPUT);
  pinMode(rightLightSensor_pin, INPUT);
  pinMode(ignitionSwitch_pin, INPUT);

  manualToggle = 1; //default to automatic

}

void loop() {
  sensorRead();
  if (currentMillis - previousMillis > delayInterval * 60000) {
    if (leftLightValue > rightLightValue) {
      trackLeft();
    }
    else {
      trackRight();
    }
    currentMillis = previousMillis;
  }

  //-------Functions below-----------
  void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
  {
    LinearActuatorSwitch(down);
    magLockSwitch(lock, lock);
    if (currentMillis - previousMillis > (interval * 60000) && ignitionSwitchVal == LOW) { // turn off parasitic drain from MagLocks on batteries at night when panels are lowered.
      magLockSwitch(unlock, unlock);
      previousMillis = currentMillis;
    }
  }


  void manualSwitch(int) {
    switch (panelMove) {
      case left:
        leftLightValue = 1 + lightSensitivity;
        rightLightValue = 0;
        break;
      case right:
        rightLightValue = 1 + lightSensitivity;
        leftLightValue = 0;
        break;
      case wait:
        rightLightValue = leftLightValue;
        break;
      case flat:
        layFlat();
        break;
      default:
        rightLightValue = leftLightValue;
        break;
    }
  }

  void sensorRead()
  {
    magLockRightSense = digitalRead(magLockRightSense_pin);
    magLockLeftSense = digitalRead(magLockLeftSense_pin);
    ignitionSwitchVal = digitalRead(ignitionSwitch_pin); // ignition or kill switch
    if (ignitionSwitchVal == LOW) { // only works if the ignition is off
      if (manualToggle = 2) { //toggle is switched to manual mode.
        manualSwitch(wait); // waiting on user to press a button. any button.
      }
      else {
        if (leftLightValue + rightLightValue > lightSensitivity) { //only works if there is sunlight
          LightSensor.SetAddress(Device_Address_H);
          leftLightValue = LightSensor.GetLightIntensity();// Get Lux value left
          LightSensor.SetAddress(Device_Address_L);
          rightLightValue = LightSensor.GetLightIntensity();// Get Lux value right
        }
      }
      else {
        layFlat();
      }
    }
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
      digitalWrite(magLockLeft_pin, HIGH);
      break;
    case unlock:
      digitalWrite(magLockLeft_pin, LOW);
      break;
    default:
      digitalWrite(magLockLeft_pin, HIGH);
      break;
  }
  switch (rightState) {
    case lock:
      digitalWrite(magLockRight_pin, HIGH);
      break;
    case unlock:
      digitalWrite(magLockRight_pin, LOW);
      break;
    default:
      digitalWrite(magLockRight_pin, HIGH);
      break;
  }

}

void LinearActuatorSwitch(int linearActuatorState) {
  switch (linearActuatorState) {
    case up:
      digitalWrite(linearActuatorRelay1_pin, HIGH);
      digitalWrite(linearActuatorRelay2_pin, HIGH);
      break;

    case down:
      digitalWrite(linearActuatorRelay1_pin, LOW);
      digitalWrite(linearActuatorRelay2_pin, LOW);
      break;

    case off:
      digitalWrite(linearActuatorRelay1_pin, HIGH);
      digitalWrite(linearActuatorRelay2_pin, LOW);
      break;

    default:
      digitalWrite(linearActuatorRelay1_pin, HIGH);
      digitalWrite(linearActuatorRelay2_pin, LOW);
      break;
  }
}

void trackLeft() { //function to lift and track if the sun is to the relative left of the panel
  magLockSwitch(lock, unlock);
  if (magLockRightSense == LOW) {
    while (leftLightValue > rightLightValue) {

      LinearActuatorSwitch(up);
    }
    LinearActuatorSwitch(off);
  }
  lowerLeftPanel();
}

void lowerLeftPanel() {
  magLockSwitch(lock, lock);
  while (leftLightValue < rightLightValue) {
    LinearActuatorSwitch(down);
  }
  LinearActuatorSwitch(off);
  if (magLockRightSense == HIGH) {
    trackRight();
  }

}

void trackRight() {// function to lift and track if the sun is to the relative right of the panel
  magLockSwitch(unlock, lock);
  if (magLockLeftSense == LOW) {
    while (leftLightValue < rightLightValue) {

      LinearActuatorSwitch(up);
    }
    LinearActuatorSwitch(off);
  }
  lowerRightPanel();
}

void lowerRightPanel() {
  magLockSwitch(lock, lock);
  while (leftLightValue > rightLightValue) {
    LinearActuatorSwitch(down);
  }
  LinearActuatorSwitch(off);
  if (magLockLeftSense == HIGH) {
    trackRight();
  }

}
