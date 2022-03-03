//constants
unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong = 1000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

enum LA{up,down,off};
enum LA LAstate;
enum magLock {pull, noPull};
enum magLock leftState;
enum magLock rightState;

//variables 
int lightSense =1; // adjustable light sensitivity based on ambient light at night
int multiplier =1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
int rightLDR = A4;
int leftLDR = A5;
const int ignitionSwitch = 4;


//output pins
int laRelayDown = 10; // linear actuator 12v relay to extend
int laRelayUp = 11; // linear actuator 12v relay to retract 
int leftSLND = 7; // solenoid control 12v relay left hinge
int rightSLND = 8; // solenoid control 12v relay right hinge

// Values
int rightLDRVal;
int leftLDRVal;
int ignitionSwitchVal;



void setup()
{
LAstate=off;
Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
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
currentMillis;
sensorRead();
if(ignitionSwitchVal == LOW){
    LAstate=down;
    leftState=pull;
    rightState=pull;




  }

else {
   Serial.println("Lay Flat");
   sensorRead();
   layFlat();
  
  }

}

//-------Functions below-----------
void sensorRead()
{
  
rightLDRVal = analogRead(rightLDR); // right LDR sensor input pin
leftLDRVal = analogRead(leftLDR); // left LDR sensor input pin
ignitionSwitchVal = digitalRead(ignitionSwitchVal); // ignition or kill switch

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
    break;//constants
unsigned long currentMillis = millis(); // delay period to lower or raise Linear Actuator fully
unsigned long previousMillis = 0;
const long intervalLong = 1000; // delay period in between lowering steps.
const long intervalShort = 5000; // delay period in between lowering steps.

enum LA{up,down,off};
enum LA LAstate;
enum magLock {pull, noPull};
enum magLock leftState;
enum magLock rightState;

//variables 
int lightSense =1; // adjustable light sensitivity based on ambient light at night
int multiplier =1.2; // multiplier for LDR differences to confirm panel is fully down before switching hinge solenoids.


//input pins
int rightLDR = A4;
int leftLDR = A5;
const int ignitionSwitch = 4;


//output pins
int laRelayDown = 10; // linear actuator 12v relay to extend
int laRelayUp = 11; // linear actuator 12v relay to retract 
int leftSLND = 7; // solenoid control 12v relay left hinge
int rightSLND = 8; // solenoid control 12v relay right hinge

// Values
int rightLDRVal;
int leftLDRVal;
int ignitionSwitchVal;



void setup()
{
LAstate=off;
Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
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
currentMillis;
sensorRead();
if(ignitionSwitchVal == LOW){
    LAstate=down;
    leftState=pull;
    rightState=pull;




  }

else {
   Serial.println("Lay Flat");
   sensorRead();
   layFlat();
  
  }

}

//-------Functions below-----------
void sensorRead()
{
  
rightLDRVal = analogRead(rightLDR); // right LDR sensor input pin
leftLDRVal = analogRead(leftLDR); // left LDR sensor input pin
ignitionSwitchVal = digitalRead(ignitionSwitchVal); // ignition or kill switch

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
    digitalWrite(1,LOW);
    digitalWrite(2,HIGH);
    Serial.println("Switch is Up");
    Serial.println();
    break;

    case down:
    digitalWrite(2,LOW);
    digitalWrite(1,HIGH);
    Serial.println("Switch is Down");
    Serial.println();
    break;

    case off:
    digitalWrite(2,LOW);
    digitalWrite(1,LOW);
    Serial.println("Switch is Off");
    Serial.println();
    break;

    default:
    digitalWrite(2,LOW);
    digitalWrite(1,LOW);
    Serial.println("Switch is Off");
    Serial.println();
    break;

  }
  }

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
    LAstate=down;
    LASwitch();
    sensorRead();

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
    digitalWrite(1,HIGH);
    digitalWrite(2,LOW);
    Serial.println("Switch is Up");
    Serial.println();
    break;

    case down:
    digitalWrite(2,HIGH);
    digitalWrite(1,LOW);
    Serial.println("Switch is Down");
    Serial.println();
    break;

    case off:
    digitalWrite(2,LOW);
    digitalWrite(1,LOW);
    Serial.println("Switch is Off");
    Serial.println();
    break;

    default:
    digitalWrite(2,LOW);
    digitalWrite(1,LOW);
    Serial.println("Switch is Off");
    Serial.println();
    break;

  }
  }

void layFlat() // function to drop the panel to its lowest point and lock both hinge solenoids in place for travel.
{
    LAstate=down;
    LASwitch();
    sensorRead();

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
