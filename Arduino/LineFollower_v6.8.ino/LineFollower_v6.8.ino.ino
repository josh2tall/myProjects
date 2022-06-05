#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
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
int in1 = 13;
int in2 = 12;
int enR = 11;

// Left motor
int in3 = 8;
int in4 = 9;
int enL = 10;

//MotorR speed is constant 
int MotorL = 75;
int MotorR = 75;
int baseSpeed = 75;
int lastError = 0;


bool turnError = false;
bool back = false;
bool arrayStop = false;

//tracks where on the track the robot is
int counter = 1;
int CarouselCounter = 0;

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 75;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;

//pid constants
double kp = .03;
double kd = .035;
double ki = 0;

//servo stuff
int maxPWM = 2300;
int minPWM = 700;
int pos = 0;    // variable to store the servo position
int servoPin = 30;
int servoUp = 137;
int servoDown = 30;
int servoMiddle = 110;

//carousel stuff
const int stepPin = 3; 
const int dirPin = 4; 
const int limitSwitch = 32;
const int fullRev = 3200;//full rev = 200
int steps = 350;
int delayTime = 750;


//led pin
const int leftLED = 49;

//magnet
int magnetPin = 7;

//pause button
bool buttonFlag = 1;
bool onoff = 0;
int buttonPin = 52;

void setup()
{
    
 // set all the motor control pins to outputs
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //led pin
  pinMode(leftLED, OUTPUT);
  
  //servo stuff
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin, minPWM, maxPWM);
  myservo.write(servoUp);


  //stepper stuff
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(limitSwitch,INPUT);  
  
  //magnet
  pinMode(magnetPin, OUTPUT);

  Serial.begin(9600);
  
  //the functions have to be called for variables to be initialized
  Left.calibrate();   
  Right.calibrate();      
  Back.calibrate();
  qtra.calibrate();

  //loads calibration values to the IR sensors
  setCalibration();
  
  timer0 = 0;

  //button startstop
  pinMode(buttonPin, INPUT);

  //initialize carousel and magnet position
  magnetUp();
  resetCarouselDouble(HIGH);  
}


void loop()
{

  //starts and pauses all processes until the
  //button is pressed again
  buttonFlag = digitalRead(52);
  if(buttonFlag)
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
    //assigns speed and direction to each motor
    assignSpeed();
  
    //reads and stores the left, right, and back IR sensor values
    readIRSensors();

    //increments counter if left IR sensor detects a line
    IRSensorCounter(500);


    switch(counter)
    {
      case 1:
      //inital turn from the white square, number of lines to cross
      StartTurnRight90Line(3);
      break;
      
      case 4:
      //mode, initial stop delay, reset timer, increment counter, initial medium or hardstop
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 6:
      //delay after stop, reset Timer, motors forward afterwards, increment counter
      hardStop(0,true, true, true, true);   
      break;

      case 9:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 12:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 14:
      hardStop(0,true, true, true, true);     
      break;

      case 17:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 20:
      turnLeft90(3, 100, true, true, 'h', false);
      turnError = true;
      break;

      case 22:
      back = true;
      break;
      
      case 23:                            //turning into inner loop
      baseSpeed = 60;
      turnLeft90(4, 100, true, true, 'h', false);
      back = false;
      turnError = true;
      break;
      
      case 25:
      //initial stopping delay, reset timer, increment counter, initial stop is hardstop or medium stop
      turnRight90(1, 0,true,true,'h', true);
      arrayStop = true;
      break;


      case 27:
      turnLeft90(3, 0, true, true, 'h', true);
      arrayStop = false;
      hardStop();
      break;

      case 31:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 33:
      hardStop(0,true, true, true, true);
      break;

      case 36:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 39:
      turnLeft90(3, 0, true, true, 'h', true);
      break;

      case 41:
      turnRight90(0, 0,true,true,'h', false);
      hardStop(30000,true, true, true, false);
      break;

      default:;
     
    }

    if(timer0 > 500){digitalWrite(leftLED, LOW);}
    
    //calculate the error depending on which IR sensor from the array are above a line
    int position = qtra.readLine(arraySensorValues);
    int error = position-3500;   

    if(turnError)
    {
      lastError = error;
      turnError = false;
    }
    
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
  if(LsensorValue[0] >= sensorSensitivity && timer0 > interval && !back && !arrayStop)
  {
      counter++;
      timer0 = 0;
      digitalWrite(leftLED, HIGH);
  }
  else if(BsensorValue[0] >= sensorSensitivity && timer0 > interval && back && !arrayStop)
  {
    counter++;
    timer0 = 0;
  }
  else if(arraySensorValues[7] >= sensorSensitivity && timer0 > interval && arrayStop)
  {
    counter++;
    timer0 = 0;
  }
}


// This function will run the motors in both directions at a fixed speed
void MotorOn()
{
  analogWrite(enR, baseSpeed);
  analogWrite(enL, baseSpeed);
}

void turnLeft90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop, bool magnet)
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

  if(magnet)
  {
    if(CarouselCounter%6 == 0)
    {
      BlueMovements();
    }
    else if(CarouselCounter%6 == 1)
    {
      GreenMovements();
    }
    else if(CarouselCounter%6 == 2)
    {
      RedMovements();
    }
    else if(CarouselCounter%6 == 3)
    {
      CyanMovements();
    }
    else if(CarouselCounter%6 == 4)
    {
      MagentaMovements();
    }
    else if(CarouselCounter%6 == 5)
    {
      GreyMovements();
    }

    CarouselCounter++;
  }
  
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
    
    Back.readCalibrated(BsensorValue);
    while(BsensorValue[0] < 400)
    {
          Back.readCalibrated(BsensorValue);
    }

    hardStop();
    delay(100);
  
    // turn on right motor
    // turn on left motor reverse
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


    //turn right until middle IR sensor detects black line
    while(arraySensorValues[5] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
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
    delay(600);
  
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
    while(arraySensorValues[5] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
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
    while(arraySensorValues[5] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
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
    delay(200);

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
  
    hardStop();
    delay(100);

    //turn left for time delay to prevent double readings
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }
    

    //turn left until IR sensor array detects a line
    qtra.readCalibrated(arraySensorValues);
    while(arraySensorValues[6] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
    }
  }

  //turn left using back IR sensor
  else if(mode == 4)
  {
    hardStop();
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
    qtra.readCalibrated(arraySensorValues);
    while(arraySensorValues[6] < 300 && arraySensorValues[5] < 300)
    {
      qtra.readCalibrated(arraySensorValues);
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

void turnRight90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop, bool magnet)
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

  if(magnet)
  {
    GreyMovements();
  }
  if(mode == 0)
  {
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    
    delay(600);
  
    // turn right
    RReverse();
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
  
    while(arraySensorValues[2] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
    }
    
  }
  else if(mode == 1)
  {
    LForward();
    RForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    delay(200);
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 500)
    {
      Right.readCalibrated(RsensorValue);
    }
    
    while(!done)
    {
      qtra.readCalibrated(arraySensorValues);
  
      if((arraySensorValues[1] > 400 && arraySensorValues[0] > 400) || (arraySensorValues[7] > 400 && arraySensorValues[6] > 400) )
        done = true;
    }
  
    done = false;
  
     hardStop();
     delay(100);
  
    //turn right 90 degrees
    LForward();
    RReverse();
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
  

    qtra.readCalibrated(arraySensorValues);
    while(arraySensorValues[0] < 400)
    {
      qtra.readCalibrated(arraySensorValues);
    }
  
    hardStop();
    delay(50);
  }


  //assign each motor the original value of motor B
  MotorL = baseSpeed;
  MotorR = baseSpeed;

  continueForward();

  if(IncrementCounter){counter++;}

  if(resetTimer){timer0 = 0;}

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
    if(arraySensorValues[4] > 400  && timer0 > interval0)
    {
      lineCounter++;
      timer0 = 0;
    }
    
  }

  hardStop();
  delay(100);

  //**********************************NEEEEEEEEEEEEDS FIXING TO GET RID OF DELAY
  //turn right 90 degrees
  RReverse();
  LForward();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
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

  continueForward();

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

  continueForward();
  
}


void hardStop(int delayAfter, bool resetTimer)
{
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enR, 200);
  
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enL, 200);

  delay(delayAfter);

  if(resetTimer){timer0 = 0;}
  
}

//active braking with style
void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
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

  if(magnet){GreyMovements();}
  
  if(resetTimer){timer0 = 0;}
  
  if(motorsForward){continueForward();}
  
  if(incrementCounter){counter++;}
  
}

//active braking
void hardStop()
{
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enR, 200);
  
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enL, 200);

  
}

//kinda active braking
void mediumStop(){
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enR, 25);
  
  // turn OFF LEFT motor
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
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
  analogWrite(enL, 200);
}

//use active braking to hard stop right motor
void hardStopRight(){
  // turn OFF RIGHT motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
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

void doMagnet(int delayM)
{
    myservo.write(servoDown);
    digitalWrite(magnetPin, HIGH);
    delay(delayM);
    
    myservo.write(servoUp);
    delay(delayM);
    digitalWrite(magnetPin, LOW);
    delay(delayM);
}

//used at beginning of loop for line following
void assignSpeed()
{
    RForward();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
}

void readIRSensors()
{
    Left.readCalibrated(LsensorValue);
    Right.readCalibrated(RsensorValue);
    Back.readCalibrated(BsensorValue);
}

void readColor()
{
  ToSensor();
  //color sensor stuff
  //
}

//  positioning carousel
//  resetCarouselDouble(HIGH);
void ToBlue(){turn(1,LOW,0);}
void ToRed(){turn(2,LOW,0);}
void ToGreen(){turn(3,LOW,0);}
void ToCyan(){turn(4,LOW,0);}
void ToSensor(){turn(1,LOW,0);}
void ResetBlue(){resetCarouselDouble(HIGH);};
void ResetRed(){resetCarouselDouble(HIGH);};
void ResetGreen(){resetCarouselDouble(HIGH);};
void ResetCyan(){resetCarouselDouble(HIGH);};

//   positioning carousel
//   resetCarousel(LOW);
void ToMagenta(){turn(4,HIGH,30);}
void ToYellow(){turn(3,HIGH,20);}
void ToGrey(){turn(2,HIGH,20);}
void ResetMagenta(){resetCarousel(LOW);};
void ResetYellow(){resetCarousel(LOW);};
void ResetGrey(){resetCarousel(LOW);};


//   positioning carousel
void ZeroOne(){turn(1,LOW,0);}
void OneZero(){turn(1,HIGH,0);}
void OneTwo(){turn(1,LOW,0);}
void TwoZero(){turn(2,HIGH,0);}
void OneThree(){turn(2,LOW,0);}
void ThreeZero(){turn(3,HIGH,0);}
void OneFour(){turn(3,LOW,0);}
void FourZero(){turn(4,HIGH,0);}
void OneFive(){turn(4,LOW,0);}
void FiveZero(){turn(5,HIGH,0);}
void OneSix(){turn(4,HIGH,30);}
void SixZero(){turn(3,LOW,0);}
void OneSeven(){turn(3,HIGH,20);}
void SevenZero(){turn(2,LOW,0);}
void OneEight(){turn(2,HIGH,20);}
void EightZero(){turn(1,LOW,0);}


//turning stuff
void BlueMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToBlue();
  dropToken();
  ResetBlue(); 
}

void RedMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToRed();
  dropToken();
  ResetRed(); 
}

void GreenMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToGreen();
  dropToken();
  ResetGreen(); 
}

void MagentaMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToMagenta();
  dropToken();
  ResetMagenta(); 
}

void CyanMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToCyan();
  dropToken();
  ResetCyan(); 
}

void YellowMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToYellow();
  dropToken();
  ResetYellow(); 
}

void GreyMovements()
{
  magnetDown();
  magnetUp();
  ToSensor();
  ToGrey();
  dropToken();
  ResetGrey(); 
}

void turn(int spaces, int dir, int offset)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction

  for(int x = 0; x < steps*spaces + offset; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
    }
}

void magnetUp()
{
  myservo.write(servoUp);
  delay(1000);
}

void magnetDown()
{
  myservo.write(servoDown);
  delay(1000);
  digitalWrite(magnetPin, HIGH);
  delay(1000);
}

void dropToken()
{
  digitalWrite(magnetPin, LOW);
  delay(1000);
}

//LOW couterclockwise
//HIGH clockwise
void resetCarousel(int dir)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}

void resetCarouselDouble(int dir)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
  for(int i = 0; i < 150; i++)
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
  if(dir == HIGH){dir = LOW;}
  else if(dir == LOW){dir = HIGH;}
  digitalWrite(dirPin,dir);
  
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
}


