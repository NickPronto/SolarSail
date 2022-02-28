unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong = 300000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

int leftLDR = 13; // left LDR sensor input pin
int rightLDR = 12; // right LDR sensor input pin
int ignitionSwitch = A0; // ignition or kill switch
int lightSense = 200; // adjustable light sensitivity based on ambient light at night
int count = 50; // Max lowering countdown sessions based off 2x delayShort's time.
int multiplier = 1.2; // multiplier for LDR differences

int laRelay = 11; // linear actuator 12v relay to retract/extend (HIGH - Lower solar panel, LOW - Lift solar panel)
int leftSLND = 9; // solenoid control 12v relay left
int rightSLND = 8; // solenoid control 12v relay right


void setup()
{
  // put your setup code here, to run once:
  pinMode(laRelay, OUTPUT);
  pinMode(leftSLND, OUTPUT);
  pinMode(rightSLND, OUTPUT);
  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
  pinMode(ignitionSwitch, INPUT);

}
void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  digitalWrite(laRelay, HIGH); //linear actuator lowered to lowest point
  if (leftSLND == LOW) {
  digitalWrite(leftSLND, HIGH); //lock right hinge solenoid after panel is lowered
  }
  if (rightSLND == LOW) {
    digitalWrite(rightSLND, HIGH); //lock right hinge solenoid after panel is lowered
  }
}


void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
  digitalWrite(leftSLND, HIGH); //lock left hinge solenoid
  digitalWrite(rightSLND, LOW); //unlock right hinge solenoid
  while (analogRead(leftLDR) > analogRead(rightLDR))
  {
    digitalWrite(laRelay, HIGH); // raise linear actuator until light sensors even one another
  }
  digitalWrite(laRelay, LOW);
}
void lowerLeftPanel() {
  if (analogRead(leftLDR) < analogRead(rightLDR) )
  {
    digitalWrite(laRelay, HIGH);
  }
  digitalWrite(laRelay, LOW);
}

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
  digitalWrite(rightSLND, HIGH); //lock right hinge solenoid
  digitalWrite(leftSLND, LOW); //unlock left hinge solenoid
  while (analogRead(rightLDR) > analogRead(leftLDR)) {
    digitalWrite(laRelay, HIGH);
  }
  digitalWrite(laRelay, LOW);
}

void lowerRightPanel() {
  if (analogRead(rightLDR) < analogRead(leftLDR))
  {
    digitalWrite(laRelay, HIGH);
  }

  digitalWrite(laRelay, LOW);

}

void loop(){
unsigned long currentMillis = millis();
if(analogRead(ignitionSwitch) <= 0 || analogRead(leftLDR) + analogRead(rightLDR) > lightSense){

if(currentMillis - previousMillis >= intervalShort){
  previousMillis = currentMillis;
  if (multiplier*analogRead(leftLDR) >= analogRead(rightLDR))
  {
    trackLeftHigh();
    lowerLeftPanel();

  }
  else if (analogRead(leftLDR) <= multiplier*analogRead(rightLDR))
  {
    trackRightHigh();
    lowerRightPanel();

  }
}
  else {
    layFlat();
  }
}
  }
