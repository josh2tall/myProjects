#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>


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

//Back Sensor
QTRSensorsRC Back((unsigned char[]) {48}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int BsensorValue[NUM_SENSORS_D];

//Sensor Array
//0 is the rightmost on board
QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3,4,5,6,7}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int arraySensorValues[NUM_SENSORS_A];



// connect motor controller pins to Arduino digital pins
// Right motor
int enR = 10;
int in1 = 9;
int in2 = 8;

// Left motor
int enL = 5;
int in3 = 7;
int in4 = 6;

//MotorR speed is constant 
int MotorL = 75;
int MotorR = 75;
int baseSpeed = 75;
int lastError = 0;

bool buttonFlag = 1;
bool onoff = 0;

//tracks where on the track the robot is
int counter = 1;

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 100;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;

//pid constants
double kp = .03;
double kd = .035;
double ki = 0;

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
  
  //the functions have to be called for variables to be initialized
  Left.calibrate();   
  Right.calibrate();      
  Back.calibrate();
  qtra.calibrate();

  //loads calibration values to the IR sensors
  setCalibration();
  
  timer0 = 0;

  //button startstop
  pinMode(52, INPUT);
}


void loop()
{

  //starts and pauses all processes until the
  //button is pressed again
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
    Left.readCalibrated(LsensorValue);
    Right.readCalibrated(RsensorValue);
    Back.readCalibrated(BsensorValue);
    IRSensorCounter(500);


    switch(counter)
    {
      case 1:
      //inital turn from the white square, number of lines to cross
      StartTurnRight90Line(3);
      break;
      
      case 3:
      //mode, initial stop delay, reset timer, increment counter, initial medium or hardstop
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 5:
      //delay after stop, reset Timer, motors forward afterwards, increment counter
      hardStop(1000,true, true, true);   
      break;

      case 7:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 9:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 11:
      hardStop(1000,true, true, true);     
      break;

      case 13:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 15:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 17:                            //turning into inner loop
      turnLeft90(1, 1000, true, true, 'h');
      break;

      case 19:
      hardStop(1000,true, true, true);   
      break;

      case 22:
      //initial stopping delay, reset timer, increment counter, initial stop is hardstop or medium stop
      turnRight90(1, 1000,true,true,'h');
      break;

      case 24:
      turnLeft90(3, 1000, true, true, 'h');
      break;

      case 26:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 28:
      hardStop(1000,true, true, true);
      break;

      case 30:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 32:
      turnLeft90(3, 1000, true, true, 'm');
      break;

      case 34:
      turnRight90(0, 1000,true,true,'h');
      hardStop(30000,true, true, true);
      break;



      

      default:;
     
    }
    
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


//*********************************************************************************************************************************


//sensor sensitivity to black 0 to 2500
//0 white
//2500 black
void IRSensorCounter(int sensorSensitivity)
{
  if(LsensorValue[0] >= sensorSensitivity && timer0 > interval)
  {
      counter++;
      timer0 = 0;
  }
}




// This function will run the motors in both directions at a fixed speed
void MotorOn()
{
  // set speed to 200 out of possible range 0~255
  analogWrite(enR, baseSpeed);

  // set speed to 200 out of possible range 0~255
  analogWrite(enL, baseSpeed);
}

void turnLeft90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop)
{
  bool done = false;
  int tempCounter = 0;
  MotorL = 75;
  MotorR = 75;


  if(Stop == 'm')
  {
    mediumStop();
  }
  else
  {
    hardStop();
  }
  delay(stoppingDelay);

  if(IncrementCounter)
  {
    counter++;
  }


  

  //outer left turn
  //move forward for delay time
  //turn right until middle IR sensor detects the line
  if(mode == 0)
  {

    //drive robot forward
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    delay(615);
  
    // turn on right motor
    // turn on left motor reverse
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


    //turn right until middle IR sensor detects black line
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[5] > 400)
        done = true;
    }
  }

  //cross left turn
  //keep turning right until left IR sensor detects two black lines
  //and the middle IR sensor detects a line
  else if(mode == 1)
  {
    //drive robot forward for half a second
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    delay(500);
  
    // turn on robot left
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


    //keep turning right until left IR sensor detects two black lines
    while(tempCounter < 2)
    {
      Left.readCalibrated(LsensorValue);
  
      if(LsensorValue[0] > 400)
      {
        tempCounter++;
        if(tempCounter == 1)
          delay(50);
      }
     
    }

    //keep turning right until middle IR sensor detects the line
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[5] > 400)
        done = true;
    }
  }
  
  //using Left IR sensor
  //go froward for delay time to prevent double reads
  //turn right until middle IR sensor detects the line
  else if(mode == 2)
  {
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    delay(200);
    
    Left.readCalibrated(LsensorValue);
    
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }
    hardStop();
    delay(100);

    //turn right 
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    //turn right until middle IR sensor detects a line
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[5] > 400)
        done = true;
    }
  }

  //complicated
  //tldr: turn left
  else if(mode == 3)
  {
    //go forward for delay time to prevent double readings
    LForward();
    RForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    delay(100);

    //go forward until left IR sensor detects a line
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400)
    {
          Left.readCalibrated(LsensorValue);
    }

    //stop when either of the two outer IR sensors from the array
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if((arraySensorValues[1] > 400 && arraySensorValues[0] > 400) || (arraySensorValues[7] > 400 && arraySensorValues[6] > 400) )
        done = true;
    }
  
    done = false;
    hardStop();
    delay(100);

    //turn left for time delay to prevent double readings
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    delay(400);

    //turn left until IR sensor array detects a line
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[6] > 400)
        done = true;
    }
  }

  
  hardStop();
  delay(100);

  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;
  RForward();
  LForward();
  MotorOn();

   if(resetTimer)
   {         
      timer0 = 0;
   }
}

void turnRight90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop)
{

  int tempCounter = 0;
  bool done = false;
  MotorL = 75;
  MotorR = 75;
  
  if(Stop == 'm')
  {
    mediumStop();
  }
  else
  {
    hardStop();
  }
  delay(stoppingDelay);


  if(mode == 1)
  {
    // turn on motor A
    LForward();
     // turn on motor B in the opposite direction of motor A
    RForward();
  
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
  
    
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if((arraySensorValues[1] > 400 && arraySensorValues[0] > 400) || (arraySensorValues[7] > 400 && arraySensorValues[6] > 400) )
        done = true;
    }
  
    done = false;
  
     hardStop();
     delay(100);
  
    
    // turn on motor A
    LForward();
     // turn on motor B in the opposite direction of motor A
    RReverse();
  
    //turn right 90 degrees
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
  
        
    while(tempCounter < 2)
      {
        Right.readCalibrated(RsensorValue);
    
        if(RsensorValue[0] > 400)
        {
          tempCounter++;
          if(tempCounter == 1)
            delay(50);
        }
       
      }
  
      
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[0] > 400)
        done = true;
    }
  
    hardStop();
    delay(50);
  }

  else if(mode == 0)
  {
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    
    delay(600);
  
    // turn on right motor
    RReverse();
  
     // turn on left motor reverse
    LForward();
  
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    while(tempCounter < 2)
      {
        Right.readCalibrated(RsensorValue);
    
        if(RsensorValue[0] > 400)
        {
          tempCounter++;
          if(tempCounter == 1)
            delay(50);
        }
       
      }
  
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if(arraySensorValues[2] > 400)
        done = true;
    }
    
  }


  
  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();

  if(IncrementCounter)
  {
    counter++;
  }

  if(resetTimer)
   {         
      timer0 = 0;
   }

}

void StartTurnRight90Line(int lineNumber)
{
  bool done = false;
  MotorL = 60;
  MotorR = 60;
  int interval0 = 300;
  
  int lineCounter = 0;
  RForward();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  
  while(lineCounter < lineNumber + 1)
  {
    
    qtra.readCalibrated(arraySensorValues);
    //read the front IR sensor values
    //Left.read(LsensorValue);

    if(arraySensorValues[4] > 400  && timer0 > interval0)
    {
      lineCounter++;
      timer0 = 0;
    }
    
  }

  hardStop();
  delay(100);

  
  // turn on right motor reverse
  RReverse();

   // turn on left motor 
  LForward();

  //turn right 90 degrees

  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);

//  while(arraySensorValues[0] > 400 || arraySensorValues[1] > 400 || arraySensorValues[2] > 400 || arraySensorValues[3] > 400 ||
//  arraySensorValues[4] > 400 || arraySensorValues[5] > 400 || arraySensorValues[6] > 400 || arraySensorValues[7] > 400)
//  {
//     qtra.readCalibrated(arraySensorValues);
//  }

  delay(500);
  
  while(!done)
  {
    qtra.readCalibrated(arraySensorValues);

    if(arraySensorValues[1] > 500)
      done = true;
  }


  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  RForward();
  LForward();
  MotorOn();

  timer0 = 0;
  counter++;
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


void hardStop(int delayAfter, bool resetTimer)
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

  delay(delayAfter);

  if(resetTimer)
  {
    //reset timer
    timer0 = 0;
  }
  
}

//active braking with style
void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter)
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

  delay(delayAfter);

  if(resetTimer)
  {
    //reset timer
    timer0 = 0;
  }

  if(motorsForward)
  {
    continueForward();
  }
  if(incrementCounter)
  {
    counter++;
  }
  
}

//active braking
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

//kinda active braking
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
}

//NOT WORKING
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

//WORKS AMAZINGLY
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

  EEPROM.get(80,temp);
  Back.calibratedMinimumOn[0] = temp;

  EEPROM.get(84,temp);
  Back.calibratedMaximumOn[0] = temp;
}



