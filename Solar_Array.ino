//constants
unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong =1000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

//variables 
int lightSense =1; // adjustable light sensitivity based on ambient light at night
int multiplier =1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
int rightLDR = A4;
int leftLDR = A5;
const int ignitionSwitch = 4;


//output pins
int laRelayDown = 12; // linear actuatorHIGH2v relay to extend
int laRelayUp = 13; // linear actuatorHIGH2v relay to retract 
int leftSLND = 7; // solenoid controlHIGH2v relay left hinge
int rightSLND = 8; // solenoid controlHIGH2v relay right hinge

// Values
int rightLDRVal;
int leftLDRVal;
int ignitionSwitchVal;



void setup()
{

Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  
  pinMode(laRelayDown, OUTPUT);
  pinMode(laRelayUp, OUTPUT);
  pinMode(leftSLND, OUTPUT);
  pinMode(rightSLND, OUTPUT);
  pinMode(leftLDR, INPUT);
  pinMode(rightLDR, INPUT);
  pinMode(ignitionSwitch, INPUT);
  sensorRead();
}

void loop(){
  sensorRead();
  if(ignitionSwitchVal == 0){
    
  

unsigned long currentMillis = millis(); 


  }

else
   Serial.println("Lay Flat");
   sensorRead();
   layFlat();
  
  }



//-------Functions below-----------
void sensorRead()
{
  
rightLDRVal = analogRead(rightLDR); // right LDR sensor input pin
leftLDRVal = analogRead(leftLDR); // left LDR sensor input pin
ignitionSwitchVal = digitalRead(ignitionSwitch); // ignition or kill switch
analogRead(rightLDR);
analogRead(leftLDR);
digitalRead(ignitionSwitch);
printOut();

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
delay(2000);
}



void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
  while(ignitionSwitchVal == 0){
  digitalWrite(laRelayDown,HIGH); //linear actuator lowered to lowest point
  digitalWrite(leftSLND,HIGH); //lock right hinge solenoid after panel is lowered
  digitalWrite(rightSLND,HIGH); //lock right hinge solenoid after panel is lowered
  Serial.println(digitalRead(rightSLND));
  Serial.println(digitalRead(leftSLND));
  Serial.println(digitalRead(laRelayDown));
sensorRead();
  }
}

void trackLeftHigh() //function to lift and track if the sun is to the relative left of the panel
{
  digitalWrite(leftSLND,HIGH); //lock left hinge solenoid
  digitalWrite(rightSLND,LOW); //unlock right hinge solenoid
  while (leftLDRVal > rightLDRVal)
  {
    digitalWrite(laRelayUp,HIGH); // raise linear actuator until light sensors even one another
    sensorRead();
    
  }
  digitalWrite(laRelayUp,LOW); // stop movement of panel
}
void lowerLeftPanel() {
  while (leftLDRVal < rightLDRVal)
  {
    digitalWrite(laRelayDown,HIGH);
    sensorRead();
  }
  digitalWrite(laRelayDown,LOW);
}

void trackRightHigh() // function to lift and track if the sun is to the relative right of the panel
{
  digitalWrite(rightSLND,HIGH); //lock right hinge solenoid
  digitalWrite(leftSLND,LOW); //unlock left hinge solenoid
  while (rightLDRVal > leftLDRVal) {
    digitalWrite(laRelayUp,HIGH);
    sensorRead();
  }
    digitalWrite(laRelayUp,LOW); // stop movement of panel
}

void lowerRightPanel() {
  while (rightLDRVal < leftLDRVal)
  {
    digitalWrite(laRelayDown,HIGH);
        sensorRead();
  }

  digitalWrite(laRelayDown,LOW);

}
