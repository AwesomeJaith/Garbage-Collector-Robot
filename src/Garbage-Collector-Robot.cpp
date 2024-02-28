#include <Servo.h>

int frontLEDPin            =  7;
int backLEDPin             =  6;

int leftMotorsPin1         = 11;
int leftMotorsPin2         = 10;

int rightMotorsPin1        = 13;
int rightMotorsPin2        = 12;

int clawServoPin           = A0;
int liftArmServoPin        = A1;
int rotateClawServoPin     = A2;
int compactorServoPin      = A4;

int forceSensorPin         = A3;

int frontUltraSonicTrigPin =  2;
int frontUltraSonicEchoPin =  3;
int backUltraSonicTrigPin  =  4;
int backUltraSonicEchoPin  =  5;
int clawUltraSonicTrigPin  =  8;
int clawUltraSonicEchoPin  =  9;

float force                =  0;
int instructionIterator    =  0;
int liftArmServoAngle      =  0;
int clawServoAngle         =  0;

Servo clawServo;
Servo liftArmServo;
Servo rotateClawServo;
Servo compactorServo;

String path; //= "FFFFRRLFHFC";

void setup()
{
  pinMode(frontLEDPin, OUTPUT);
  pinMode(backLEDPin , OUTPUT);
  
  pinMode(leftMotorsPin1 , OUTPUT);
  pinMode(leftMotorsPin2 , OUTPUT);
  
  pinMode(rightMotorsPin1, OUTPUT);
  pinMode(rightMotorsPin2, OUTPUT);
  
  pinMode(clawServoPin      , OUTPUT);
  pinMode(liftArmServoPin   , OUTPUT);
  pinMode(rotateClawServoPin, OUTPUT);
  pinMode(compactorServoPin , OUTPUT);
  
  clawServo.attach(clawServoPin);
  liftArmServo.attach(liftArmServoPin);
  rotateClawServo.attach(rotateClawServoPin);
  compactorServo.attach(compactorServoPin);
  
  pinMode(forceSensorPin, INPUT);
  
  pinMode(frontUltraSonicTrigPin, OUTPUT);
  pinMode(frontUltraSonicEchoPin, INPUT);
  
  pinMode(backUltraSonicTrigPin , OUTPUT);
  pinMode(backUltraSonicEchoPin , INPUT);
 
  pinMode(clawUltraSonicTrigPin , OUTPUT);
  pinMode(clawUltraSonicEchoPin , INPUT);
    
  Serial.begin(9600);
  
  Serial.println("Enter a path you would like the robot to travel.");
  while (Serial.available() == 0)
  {
  }
  
  path = Serial.readString();
  delay(1000);
  Serial.print("You entered: ");
  Serial.println(path);
}

void loop()
{  
  String instruction = getNextInstruction();
  
  Serial.println(instruction);
  
  if (instruction == "forward") 
  {
  	moveForward(2500);
  }
  if (instruction == "right") 
  {
    moveRight(2500);
  }
  if (instruction == "left") 
  {
    moveLeft(2500);
  }
  if (instruction == "backward") 
  {
    moveBackward(2500);
  }
  if (instruction == "collect") 
  {
    emptyTrashCanProcedure();
  }
  if (instruction == "complete") 
  {
    endProgramProcedure();
  }
  
  delay(1000);
}

String getNextInstruction() 
{
  char nextInstruction;
  
  if (instructionIterator >= path.length()) 
  {
  	nextInstruction = 'C';
  }
  else 
  {
    nextInstruction = path[instructionIterator];
    instructionIterator++;
  }
  
  switch(nextInstruction) 
  {
    case 'F':
      return "forward";
    case 'R':
      return "right";
    case 'L':
      return "left";
    case 'B':
      return "backward";
    case 'H':
      return "collect";
    default:
      return "complete";
  }
}

float distanceFromSensor(int sensorTrigPin, int sensorEchoPin) 
{
  int distance;
  int time;
  
  digitalWrite(sensorTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensorTrigPin, LOW);
  
  time = pulseIn(sensorEchoPin, HIGH);
  distance = time / 148.1; // Distance in inches
  
  return distance;
}

bool checkFrontObstructions(int distance) 
{
  if ( distanceFromSensor(frontUltraSonicTrigPin, 
                          frontUltraSonicEchoPin) <= distance ) 
  {
    Serial.println("There is an obstruction in the front.");
    return true;
  }
  else 
  {
    Serial.println("No obstruction detected in the front.");
    return false;
  }
}

bool checkBackObstructions(int distance) 
{
  if ( distanceFromSensor(backUltraSonicTrigPin, 
                          backUltraSonicEchoPin) <= distance ) 
  {
    Serial.println("There is an obstruction in the back.");
    return true;
  }
  else 
  {
    Serial.println("No obstruction detected in the back.");
    return false;
  }
}     

void moveForward(int timeInMilliseconds) 
{
  while (checkFrontObstructions(30) == true) 
  {
  	delay(100);
  }
  activateForwardMotors(timeInMilliseconds);
}

void moveRight(int timeInMilliseconds) 
{
  while ( (checkFrontObstructions(30) == true) && 
          (checkBackObstructions(30)  == true) ) 
  {
  	delay(100);
  }
  activateLeftMotor(timeInMilliseconds);
}

void moveLeft(int timeInMilliseconds) {
  while ( (checkFrontObstructions(30) == true) && 
          (checkBackObstructions(30)  == true) ) 
  {
  	delay(100);
  }
  activateRightMotor(timeInMilliseconds);
}

void moveBackward(int timeInMilliseconds) 
{
  while (checkBackObstructions(30) == true) 
  {
  	delay(100);
  }
  activateBackwardMotors(timeInMilliseconds);
}

void activateForwardMotors(int timeInMilliseconds) 
{
  digitalWrite(leftMotorsPin1 , HIGH);
  digitalWrite(leftMotorsPin2 , LOW);
  digitalWrite(rightMotorsPin1, HIGH);
  digitalWrite(rightMotorsPin2, LOW);
  delay(timeInMilliseconds);
}

void activateLeftMotor(int timeInMilliseconds) 
{
  digitalWrite(leftMotorsPin1 , HIGH);
  digitalWrite(leftMotorsPin2 , LOW);
  digitalWrite(rightMotorsPin1, LOW);
  digitalWrite(rightMotorsPin2, LOW);
  delay(timeInMilliseconds);
}

void activateRightMotor(int timeInMilliseconds) 
{
  digitalWrite(leftMotorsPin1 , LOW);
  digitalWrite(leftMotorsPin2 , LOW);
  digitalWrite(rightMotorsPin1, HIGH);
  digitalWrite(rightMotorsPin2, LOW);
  delay(timeInMilliseconds);
}

void activateBackwardMotors(int timeInMilliseconds) 
{
  digitalWrite(leftMotorsPin1 , LOW);
  digitalWrite(leftMotorsPin2 , HIGH);
  digitalWrite(rightMotorsPin1, LOW);
  digitalWrite(rightMotorsPin2, HIGH);
  delay(timeInMilliseconds);
}

void stopMotors() 
{
  digitalWrite(leftMotorsPin1 , LOW);
  digitalWrite(leftMotorsPin2 , LOW);
  digitalWrite(rightMotorsPin1, LOW);
  digitalWrite(rightMotorsPin2, LOW);
}

void flashLights(bool frontLight, bool backLight, int durationInSeconds) 
{  
  for (int i = durationInSeconds * 2; i > 0; i--) 
  {
    if (frontLight) 
    {
      digitalWrite(frontLEDPin, HIGH);
    }
    if (backLight) 
    {
      digitalWrite(backLEDPin , HIGH);
    }
    
    delay(250);
    
    if (frontLight) 
    {
      digitalWrite(frontLEDPin, LOW);
    }
    if (backLight) 
    {
      digitalWrite(backLEDPin , LOW);
    }
    
    delay(250);
  }
}

bool checkForTrashCan(int distance)
{
  if ( distanceFromSensor(clawUltraSonicTrigPin, 
                          clawUltraSonicEchoPin) <= distance ) 
  {
    return true;
  }
  else 
  {
    return false;
  }
}

void findTrashCan()
{
  Serial.println("Searching for can in general area.");
  while (!checkForTrashCan(50))
  {
    moveForward(1);
    delay(100);
  }
}

void moveArmForward(int angleIncrement)
{
  Serial.println("Moving arm forward.");
  liftArmServoAngle += angleIncrement;
  liftArmServo.write(liftArmServoAngle); 
}
    
void grabTrashCan()
{
  Serial.println("Searching for can within 5 inches.");
  while ( !checkForTrashCan(5) && (liftArmServoAngle < 45) )
  {
    moveArmForward(1); 
    delay(100);
  }
  adjustClaw(400);
}

void adjustClaw(int force) 
{
  Serial.println("Adjusting claw.");
  while (analogRead(forceSensorPin) <= force)
  {
    clawServoAngle += 1;
    clawServo.write(clawServoAngle);
    delay(100);
  }
}

void emptyTrashCan() 
{
  Serial.println("Emptying can.");
  rotateClawServo.write(180);
  delay(1000);
  rotateClawServo.write(0);
}

void resetLiftArm() 
{
  Serial.println("Resetting lift arm and claw.");
  clawServo.write(0);
  delay(1000);
  liftArmServo.write(0);
  delay(1000);
  rotateClawServo.write(0);
  delay(1000);
}

void compactTrash()
{
  Serial.println("Compacting trash");
  compactorServo.write(360);
  delay(5000);
  compactorServo.write(0);
}

void emptyTrashCanProcedure()
{
  stopMotors();
  flashLights(true, false, 5);
  findTrashCan();
  stopMotors();
  grabTrashCan();
  emptyTrashCan();
  resetLiftArm();
  compactTrash();
}

void endProgramProcedure()
{
  stopMotors();
  flashLights(true, true, 10);

  // No official solution exists to stop an Arduino program
  while(1);
}
