#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the LED built in to the Arduino on pin 13 while calibration is going on.
// During this phase, you should expose each reflectance sensor to the lightest and 
// darkest readings they will encounter.
// For example, if you are making a line follower, you should slide the sensors across the
// line during the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is.  Improper calibration will result in poor readings.
// If you want to skip the calibration phase, you can get the raw sensor readings
// (pulse times from 0 to 2500 us) by calling qtrrc.read(sensorValues) instead of
// qtrrc.readLine(sensorValues).

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance) 
// to 1000 (minimum reflectance) followed by the estimated location of the line as a number
// from 0 to 5000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.


#define NUM_SENSORS_D   1     // number of sensors used
#define NUM_SENSORS_A   8     // number of sensors used
#define TIMEOUT       2500    // waits for 2500 microseconds for sensor outputs to go low
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading


//Left Sensor
QTRSensorsRC Left((unsigned char[]) {51}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int LsensorValue[NUM_SENSORS_D];


//Right Sensor
QTRSensorsRC Right((unsigned char[]) {50}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int RsensorValue[NUM_SENSORS_D];

//Sensor Array
QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3,4,5,6,7}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int arraySensorValues[NUM_SENSORS_A];



// connect motor controller pins to Arduino digital pins
// motor one
int enR = 10;
int in1 = 9;
int in2 = 8;

// motor two
int enL = 5;
int in3 = 7;
int in4 = 6;

//MotorR speed is constant 
int MotorL = 60;
int MotorR = 60;
int baseSpeed = 60;
int lastError = 0;

int counter = 0;
int LCounter = 0;
int RCounter = 0;
int levelCounter = 1;

bool innerCounter = 0;

bool buttonFlag = 1;
bool onoff = 0;

bool first = false;

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 100;

//interval before the front IR sensors detect lines in ms
int intervalAfter = 300;
bool frontReady = false;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;

double kp = .03;
double kd = .035;
double ki = 0;
//double ki = .0001;

void setup()
{
    
 // set all the motor control pins to outputs
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //Set the left and right motor forward
  RForward();
  LForward();
  
  Serial.begin(9600);
  Serial.println("Calibrating...");
  delay(500);
  
  //calibrationMovement();

  //the functions have to be called for variables to be initialized
  Left.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  Right.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  qtra.calibrate();

  setCalibration();
  
  timer0 = 0;

  //button startstop
  pinMode(52, INPUT);
}


void loop()
{
    
  buttonFlag = digitalRead(52);
  if(!buttonFlag)
  {
    onoff = !onoff;
    delay(500);
  }
  
  if(!onoff)
  {
    hardStop();
  }
  else
  {
    
    RForward();
    LForward();
    //assign a speed for each motor
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
  
  
    //read the front IR sensor values
    Left.read(LsensorValue);
    Right.read(RsensorValue);

    readLeftSensor();

    if(counter == 1)
      {
        LeftTurnTest();
      }

    counter = 0;
    
    //calculate the error depending on which IR sensor from the array are above a line
    int position = qtra.readLine(arraySensorValues);

    int error = position-3500;
  
    Serial.println(position);
   
    
    //calculate PID values
    proportional = kp * error;
    integral = ki * (integral + proportional);
    derivative = kd * (error-lastError);
    
    
    double MotorLAdjust = proportional + integral + derivative;
  
    lastError = error; 
  
    //change MotorL speed depending on PID values
    MotorL = baseSpeed - MotorLAdjust;
  
    //ensure the motor doesnt go passed limits
    if(MotorL < 0)
    MotorL = 0;
    
    if(MotorL > 190)
    MotorL = 190;
  
    }
  
}

//--------------------------------------------------------Function Definitions---------------------------------------------------//



/*********************************************** NEW STUFF******************************/

void readLeftSensor()
{
   if(LsensorValue[0] >= 2000)
   {
      counter++;
      timer0 = 0;
    }
}
void LeftTurnTest()
{
  //leftTurnTest
    mediumStop();
    delay(1000);
    turnLeft90();
    hardStop();
    delay(5000);
    continueForward();
}

void RightTurnTest()
{
  //RightTurnTest
    //stop on line
    hardStop();
    delay(1000);
    turnRight90();
    hardStop();
    delay(5000);
    continueForward();
}

void hardStopTest()
{
    //hardStopTest
    hardStop();
    delay(1000);
    continueForward();
}




/*********************************************** NEW STUFF******************************/

// This function will run the motors in both directions at a fixed speed
void MotorOn()
{
  // set speed to 200 out of possible range 0~255
  analogWrite(enR, baseSpeed);

  // set speed to 200 out of possible range 0~255
  analogWrite(enL, baseSpeed);
}

//Function to make the robot move left/right as it calibrates
void calibrationMovement(){
  int calSpeed = 75;
  int j = 0;
  int i = 0;
  
  RReverse();
  analogWrite(enR, calSpeed);
  
  // turn on motor B
  LForward();
  analogWrite(enL, calSpeed);

  for(j = 0; j < 4; j++){
    qtra.calibrate();
    Left.calibrate();
    Right.calibrate();
  }
    
  for(i = 0; i < 5; i++){
      // turn on motor A
    RForward();
    analogWrite(enR, calSpeed);
    
    // turn on motor B
    LReverse();
    analogWrite(enL, calSpeed);

    for(j = 0; j < 7; j++){
      qtra.calibrate();
      Left.calibrate();
      Right.calibrate();
    }

    // turn on motor A
    RReverse();
    analogWrite(enR, calSpeed);
    
    // turn on motor B
    LForward();
    analogWrite(enL, calSpeed);

    for(int j = 0; j < 7; j++){
      qtra.calibrate();
      Left.calibrate();
      Right.calibrate();
    }
  }

  RForward();
  analogWrite(enR, calSpeed);
  
  // turn on motor B
  LReverse();
  analogWrite(enL, calSpeed);

  for(int j = 0; j < 5; j++){
    qtra.calibrate();
    Left.calibrate();
    Right.calibrate();
  }

  hardStop();
}

void turnLeft90()
{
  MotorL = 75;
  MotorR = 75;
  
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  delay(545);
  
  // turn on motor A
  RForward();

   // turn on motor B in the opposite direction of motor A
  LReverse();

  //turn left 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(580);

  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();
}

void turnRight90()
{
  MotorL = 75;
  MotorR = 75;
  
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  delay(600);
  
  // turn on motor A
  LForward();

   // turn on motor B in the opposite direction of motor A
  RReverse();

  //turn right 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(515);

  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();
}

void StartTurnRight90Time()
{
  MotorL = 75;
  MotorR = 75;

  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  delay(1000);

  
  // turn on motor A
  RReverse();

   // turn on motor B in the opposite direction of motor A
  LForward();

  //turn left 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(500);

  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();
}

void StartTurnRight90Line(int lineNumber)
{
  
  MotorL = 75;
  MotorR = 75;
  int interval0 = 300;
  
  int lineCounter = 0;
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  while(lineCounter < lineNumber + 1)
  {
    
    
    //read the front IR sensor values
    Left.read(LsensorValue);

    if(LsensorValue[0] >= 2000 && timer0 > interval0)
    {
      lineCounter++;
      timer0 = 0;
    }
    
  }

  // turn on right motor reverse
  RReverse();

   // turn on left motor 
  LForward();

  //turn right 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);

  delay(500);


  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();

  timer0 = 0;
}

void turnLeft135()
{
  MotorL = 75;
  MotorR = 75;
  
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  delay(525);
  
  // turn on motor A
  RForward();

   // turn on motor B in the opposite direction of motor A
  LReverse();

  //turn left 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(500);


  /****************************************************************************************/
  hardStop();
  delay(1000);

  
  LReverse();
  RReverse();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(50);


  
  LReverse();
  analogWrite(enL, MotorL);

  
  timer0 = 0;
  int LeftReadTime = 0;
  int RightReadTime = 0;
  int counter = 0;
  int timeSinceDetect = 0;
  
  while(counter < 3)
  {
    
    //read the front IR sensor values
    Left.read(LsensorValue);
    Right.read(RsensorValue);

    if(LsensorValue[0] >= 2000 && timer0 - timeSinceDetect > 100)
    {
      LeftReadTime = timer0;
      counter++;
      timeSinceDetect = timer0;
    }
    else if(RsensorValue[0] >= 2000 && timeSinceDetect > 100)
    {
      RightReadTime = timer0;
      counter++;
      timeSinceDetect = timer0;
    }
    
  }
  
  LForward();
  delay(RightReadTime-LeftReadTime+25);
  

  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();
  
}

void turnRight45()
{
  MotorL = 75;
  MotorR = 75;
  
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  delay(525);


  hardStopRight();


  //turn right 45 degrees
  analogWrite(enL, MotorL);
  delay(500);

  
}

void hardStop()
{
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enR, 200);
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enL, 200);
}

void mediumStop(){
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enR, 25);
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enL, 25);
}

//Set the right motor forward
void RForward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

//Set the left motor forward
void LForward(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);  
}

//Set the right motor in reverse
void RReverse(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

//Set the left motor in reverse
void LReverse(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

//use active braking to hard stop left motor
void hardStopLeft(){
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enL, 200);
  
}

//use active braking to hard stop right motor
void hardStopRight(){
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enR, 200);
}

void continueForward(){
  RForward();
  LForward();
  MotorOn();
  //delay(250);
}

//get the weighted average of the array of values in
//SensorValues
//0*value0 + 1000*value1 + 2000*value2 + ...
//--------------------------------------------
//value0  +  value1  +  value2 + ...

int getPosition(unsigned int * SensorValues)
{
    int Position = 0;
    int SumSensorValues = 0;
    
    for(int i = 0; i < 8; i++)
    {
        Position += SensorValues[i]*i*1000;
        SumSensorValues += SensorValues[i];
    }
    
    Position = Position/SumSensorValues;
    
    return Position;
}

void setCalibration()
{
  unsigned int temp;

  EEPROM.get(0,temp);
  Left.calibratedMinimumOn[0] = temp;

  EEPROM.get(4,temp);
  Right.calibratedMinimumOn[0] = temp;


  EEPROM.get(8,temp);
  Left.calibratedMaximumOn[0] = temp;

  EEPROM.get(12,temp);
  Right.calibratedMaximumOn[0] = temp;
  
  int i = 16;
  int j = 0;
  while(i < 48)
  {
    EEPROM.get(i,temp);
    qtra.calibratedMinimumOn[j] = temp;
    i = i + 4;
    j++;
  }

  j = 0;
  while(i < 80)
  {
    EEPROM.get(i,temp);
    qtra.calibratedMaximumOn[j] = temp;
    i = i + 4;
    j++;
  }
}

