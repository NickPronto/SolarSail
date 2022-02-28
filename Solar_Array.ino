unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong = 300000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

int leftLDR = 13; // left LDR sensor input pin
int rightLDR = 12; // right LDR sensor input pin
int ignitionSwitch = A0; // ignition or kill switch
int lightSense = 200; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.

int laRelayDown = 11; // linear actuator 12v relay to extend
int laRelayUp = 10; // linear actuator 12v relay to retract 
int leftSLND = 9; // solenoid control 12v relay left
int rightSLND = 8; // solenoid control 12v relay right


void setup()
{
  // put your setup code here, to run once:
  pinMode(laRelayDown, OUTPUT);
  pinMode(laRelayUp, OUTPUT);
  pinMode(leftSLND, OUTPUT);
  pinMode(rightSLND, OUTPUT);
  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
  pinMode(ignitionSwitch, INPUT);

}
void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  digitalWrite(laRelayDown, HIGH); //linear actuator lowered to lowest point
 if(currentMillis - previousMillis >= intervalLong){ // delay to allow panel to be completely lowered before locking all sides. Could be removed if we use maglocks over electromagnetic solenoids.
  previousMillis = currentMillis;
  if (leftSLND == LOW) {
  digitalWrite(leftSLND, HIGH); //lock right hinge solenoid after panel is lowered
  }
  if (rightSLND == LOW) {
    digitalWrite(rightSLND, HIGH); //lock right hinge solenoid after panel is lowered
  }
}
  digitalWrite(laRelayDown, LOW); //turn off down relay to avoid conflicts 
}

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
  digitalWrite(leftSLND, HIGH); //lock left hinge solenoid
  digitalWrite(rightSLND, LOW); //unlock right hinge solenoid
  while (analogRead(leftLDR) > analogRead(rightLDR))
  {
    digitalWrite(laRelayUp, HIGH); // raise linear actuator until light sensors even one another
  }
  digitalWrite(laRelayUp, LOW); // stop movement of panel
}
void lowerLeftPanel() {
  while (analogRead(leftLDR) < analogRead(rightLDR) )
  {
    digitalWrite(laRelayDown, HIGH);
  }
  digitalWrite(laRelayDown, LOW);
}

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
  digitalWrite(rightSLND, HIGH); //lock right hinge solenoid
  digitalWrite(leftSLND, LOW); //unlock left hinge solenoid
  while (analogRead(rightLDR) > analogRead(leftLDR)) {
    digitalWrite(laRelayUp, HIGH);
  }
    digitalWrite(laRelayUp, LOW); // stop movement of panel
}

void lowerRightPanel() {
  while (analogRead(rightLDR) < analogRead(leftLDR))
  {
    digitalWrite(laRelayDown, HIGH);
  }

  digitalWrite(laRelayDown, LOW);

}

void loop(){
unsigned long currentMillis = millis(); // delay in panel movements to avoid bouncing/jittering
if(analogRead(ignitionSwitch) <= 0 || analogRead(leftLDR) + analogRead(rightLDR) > lightSense){

if(currentMillis - previousMillis >= intervalShort){
  previousMillis = currentMillis;
  if (multiplier*analogRead(leftLDR) >= analogRead(rightLDR)) // "multiplier" here gives buffer to LDR differences while transitioning over from left to right tracking, avoiding a situation where both hinges are disconnected.
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
