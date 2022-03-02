//constants
unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong = 1000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

//variables 
int lightSense = 1; // adjustable light sensitivity based on ambient light at night
int multiplier = 1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
int rightLDR;
int leftLDR;
int ignitionSwitch = 4;

//output pins
int laRelayDown = 13; // linear actuator 12v relay to extend
int laRelayUp = 12; // linear actuator 12v relay to retract 
int leftSLND = 7; // solenoid control 12v relay left hinge
int rightSLND = 8; // solenoid control 12v relay right hinge

int ignitionSwitchVal;


void setup()
{


Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(laRelayDown, OUTPUT);
  pinMode(laRelayUp, OUTPUT);
  pinMode(leftSLND, OUTPUT);
  pinMode(rightSLND, OUTPUT);
  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
  pinMode(ignitionSwitch, INPUT);

  


}

void loop(){
unsigned long currentMillis = millis(); 
void sensorRead();

Serial.println("Left Analog reading: ");
Serial.println(leftLDR);
Serial.write( '\r' ); //  Carriage Return
Serial.println("Right Analog reading: ");
Serial.println(rightLDR);
  Serial.write( '\r' ); //  Carriage Return
  delay(1000);
  
if(ignitionSwitchVal == LOW || leftLDR + rightLDR > lightSense){ //only works when there is ample light and the ignition is off.

if(currentMillis - previousMillis >= intervalShort){ // delay in panel movements to avoid bouncing/jittering
  previousMillis = currentMillis;
  if (multiplier*leftLDR >= rightLDR) // "multiplier" here gives buffer to LDR differences while transitioning over from left to right tracking, avoiding a situation where both hinges are disconnected.
  {
    Serial.println("Stuck Left High Panel");
    sensorRead();
    trackLeftHigh();
    lowerLeftPanel();

  }
  else if (leftLDR <= multiplier*rightLDR)
  {
    Serial.println("Stuck Right High Panel");
    sensorRead();
    trackRightHigh();
    lowerRightPanel();

  }
}
}
  else {
    Serial.println("Stuck Lay Flat");
    sensorRead();
    layFlat();
  }

  }


//-------Functions below-----------

void sensorRead()
{
rightLDR = analogRead(A4); // right LDR sensor input pin
leftLDR = analogRead(A5); // left LDR sensor input pin
ignitionSwitchVal=digitalRead(ignitionSwitch); // ignition or kill switch
}

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  digitalWrite(laRelayDown, HIGH); //linear actuator lowered to lowest point
  digitalWrite(leftSLND, HIGH); //lock right hinge solenoid after panel is lowered
  digitalWrite(rightSLND, HIGH); //lock right hinge solenoid after panel is lowered
 if(currentMillis - previousMillis >= intervalLong){ // delay to confirm panel to be completely lowered. 
  previousMillis = currentMillis;
  digitalWrite(laRelayDown, LOW); 
 }
}

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
  digitalWrite(leftSLND, HIGH); //lock left hinge solenoid
  digitalWrite(rightSLND, LOW); //unlock right hinge solenoid
  while (leftLDR > rightLDR)
  {
    digitalWrite(laRelayUp, HIGH); // raise linear actuator until light sensors even one another
    sensorRead();
    
  }
  digitalWrite(laRelayUp, LOW); // stop movement of panel
}
void lowerLeftPanel() {
  while (leftLDR < rightLDR )
  {
    digitalWrite(laRelayDown, HIGH);
    sensorRead();
  }
  digitalWrite(laRelayDown, LOW);
}

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
  digitalWrite(rightSLND, HIGH); //lock right hinge solenoid
  digitalWrite(leftSLND, LOW); //unlock left hinge solenoid
  while (rightLDR > leftLDR) {
    digitalWrite(laRelayUp, HIGH);
    sensorRead();
  }
    digitalWrite(laRelayUp, LOW); // stop movement of panel
}

void lowerRightPanel() {
  while (rightLDR < leftLDR)
  {
    digitalWrite(laRelayDown, HIGH);
        sensorRead();
  }

  digitalWrite(laRelayDown, LOW);

}
