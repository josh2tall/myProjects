#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Servo myservo;  // create servo object to control a servo
#define NUM_SENSORS_D   1     // number of sensors used
#define NUM_SENSORS_A   8     // number of sensors used
#define TIMEOUT       2500    // waits for 2500 microseconds for sensor outputs to go low
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading


//Left IR Sensor
QTRSensorsRC Left((unsigned char[]) {51}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int LsensorValue[NUM_SENSORS_D];

//Right IR Sensor
QTRSensorsRC Right((unsigned char[]) {50}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int RsensorValue[NUM_SENSORS_D];

//Middle left IR Sensor
QTRSensorsRC side((unsigned char[]) {48}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int SsensorValue[NUM_SENSORS_D];

//Front Sensor Array
QTRSensorsAnalog qtraFront((unsigned char[]) {0,1,2,3,4,5,6,7}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int frontArraySensorValues[NUM_SENSORS_A];

//Back Sensor Array
QTRSensorsRC qtraBack((unsigned char[]) {29,28,27,26,25,24,23,22},
  NUM_SENSORS_A, TIMEOUT, QTR_NO_EMITTER_PIN); 
  unsigned int backArraySensorValues[NUM_SENSORS_A];


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
bool right = false;
bool sideSensor = false;
bool back = false;
bool arrayStop = false;

//tracks where on the track the robot is
int counter = 19;


//////////////COUNTER ^^^^ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int carouselCounter = 0;
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
double kd = .03;
double ki = 0;

//servo stuff
int maxPWM = 2300;
int minPWM = 700;
int pos = 0;    // variable to store the servo position
int servoPin = 30;
int servoUp = 135;
int servoDown = 34;
int servoMiddle = 110; //115 for three tokens

//carousel stuff
const int stepPin = 3; 
const int dirPin = 4; 
const int limitSwitch = 32;
const int fullRev = 3200;//full rev = 200
int steps = 175 * 2;
int delayTime = 250;

int tempCounter = 0;

int blueOffset = -45;
int greenOffset = -35;
int redOffset = -35;
int sensorOffset = 0;
int cyanOffset = -35;
int magentaOffset = 75;
int yellowOffset = 60;
int greyOffset = 60;

const int leftLED = 49;

//magnet
int magnetPin = 7;

//pause button
bool buttonFlag = 1;
bool onoff = 0;
int buttonPin = 52;

//color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
char color = 'z';



void setup()
{
  //color sensor
  tcs.begin();    
 
  // set all the motor control pins to outputs
  setMotorPins();

  //led ir sensor stuff
  pinMode(leftLED,OUTPUT);
  
  //servo stuff
  setServo();


  //stepper stuff
  setStepperPins();
  
  //magnet
  pinMode(magnetPin, OUTPUT);
  
  Serial.begin(9600);
  
  //the functions have to be called for variables to be initialized
  initCalibration();

  //loads calibration values to the IR sensors
  setCalibration();

  //display calibration in the serial monitor
  outputCalibration();
  
  timer0 = 0;

  //button startstop
  pinMode(buttonPin, INPUT);

  //initialize carousel and magnet position
  magnetUp();
  delay(1000);
  resetCarouselCCW();
  delay(1000);
  digitalWrite(leftLED, HIGH);
  delay(1000);
  digitalWrite(leftLED, LOW); 


  
}


void loop()
{

  //starts and pauses all processes until the
  //button is pressed again
  buttonFlag = digitalRead(52);
  if(!buttonFlag)
  {
    onoff = !onoff;
    delay(200);
  }
  
  if(!onoff)
  {
    hardStop();
  }
  else
  {

    if(timer0 > 100)
    {
        digitalWrite(leftLED, LOW);
    }
    
    //assigns speed and direction to each motor
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
  
    //reads and stores the left, right, and back IR sensor values
    readIRSensors();

    //increments counter if left IR sensor detects a line
    IRSensorCounter(500);


    
    switch(counter)
    {
      //void turnLeft90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop, bool magnet)

      case 1:
      //inital turn from the white square, number of lines to cross
      StartTurnRight90Line(3);
      break;
      
      case 4:
      //mode, initial stop delay, reset timer, increment counter, initial medium or hardstop
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 6:
      //void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
      hardStop(0,true, true, true, true);   
      break;

      case 9:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 12:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 14:
      //void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
      hardStop(0,true, true, true, true);     
      break;
      
      //turnaround 180 after picking up token
      case 17:
      hardStop(0,true, false, true, true);
      break;

      case 19:
      sideSensor = true;
      break;
      
      case 20:                            //turning into inner loop
      turnRight90(4,0,true,true, 'h', false);
      sideSensor = false;
      break;
      
      case 22:
      //initial stopping delay, reset timer, increment counter, initial stop is hardstop or medium stop
      turnRight90(1, 0,true,true,'h', true);
      break;


      case 25:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 28:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 30:
      //void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
      hardStop(0,true, true, true, true);
      break;

      
      case 33:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 36:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      //Into Blue
      case 40:
      turnRight45(0);
      break;

      //Turn Around
      case 45:
      color = 'b';
      forwardStop();
      depositTokens();
      delay(100);
      turn180(1);
      break;

      case 47:
      back = true;
      break;

      //Toward Green
      case 48:
      turnLeft45(0);
      back = false;
      break;

      case 50:
      sideSensor = true;
      break;

      //Into and Out of Green
      case 51:
      color = 'g';
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      sideSensor = false;
      break;

      case 53:
      right = true;
      break;

      //Into Red and Turn Around
      case 54:
      color = 'r';
      forwardStop();
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 56:
      back = true;
      break;

      //Toward Cyan
      case 57:
      turnLeft45(0);
      back = false;
      break;
      
      case 59:
      right = true;
      break;

      //Into Cyan and Turn Around
      case 60:
      color = 'c';
      forwardStop();
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 62:
      back = true;
      break;

      //Toward Magenta
      case 63:
      turnLeft45(0);
      back = false;
      break;

      case 65:
      sideSensor = true;
      break;

      //Into and Out of Magenta
      case 66:
      color = 'm';
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      sideSensor = false;
      break;

      case 68:
      right = true;
      break;

      //Into Yellow and Turn Around
      case 69:
      color = 'y';
      forwardStop();
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 71:
      turnLeft45(1);
      hardStop();
      delay(1000);
      timer0 = 0;
      break;

      case 72:
      if(timer0 > 3900)
      {
        hardStop();
        delay(100);
        turnLeft90(7, 0, true, false, 'h', false);      
        hardStop();
        delay(100000);
        counter++;
      }
      break;
      default:;
     
    }
    
    //calculate the error depending on which IR sensor from the array are above a line
    int position = qtraFront.readLine(frontArraySensorValues);
    int error = position-3500;   

    if(turnError)
    {
      lastError = error;
      turnError = false;
    }
    
    //calculate PID values
    calculatePID(error);
    
    lastError = error; 
  
    //change MotorL speed depending on PID values
    double MotorLAdjust = proportional + integral + derivative;
    MotorL = baseSpeed - MotorLAdjust;
  
    //ensure the motor doesnt go passed limits
    if(MotorL < 0)
    MotorL = 0;
    
    if(MotorL > 215)
    MotorL = 215;
  
    }
  
}


//*********************************************************************************************************************************


//sensor sensitivity to black 0 to 2500
//0 white 
//2500 black
void IRSensorCounter(int sensorSensitivity)
{
  if(LsensorValue[0] >= sensorSensitivity && timer0 > interval && !right && !sideSensor && !arrayStop && !back)
  {
      counter++;
      timer0 = 0;
      digitalWrite(leftLED, HIGH);
  }
  else if(RsensorValue[0] >= sensorSensitivity && timer0 > interval && right && !sideSensor && !arrayStop && !back)
  {
      counter++;
      timer0 = 0;
      digitalWrite(leftLED, HIGH);
  }
  else if(SsensorValue[0] >= sensorSensitivity && timer0 > interval && !right && sideSensor && !arrayStop && !back)
  {
    counter++;
    timer0 = 0;
  }
  else if(frontArraySensorValues[7] >= sensorSensitivity && !right && timer0 > interval && arrayStop && !back)
  {
    counter++;
    timer0 = 0;
  }
  else if(backArraySensorValues[0] >= sensorSensitivity && !right && timer0 > interval && back)
  {
    counter++; 
    timer0 = 0;
    digitalWrite(leftLED, HIGH);
  }
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
    retrieveToken();
    carouselCounter++;
  }
  
  if(IncrementCounter)
  {
    counter++;
  }

  //Outer Left Turn
  //move forward for delay time
  //turn right until middle IR sensor detects the line
  if(mode == 0)
  {

    //drive robot forward
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    
    do
    {
        side.readCalibrated(SsensorValue);
    }while(SsensorValue[0] < 400);

    hardStop();
    delay(100);
  
    // turn on right motor
    // turn on left motor reverse
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);


    //turn right until middle IR sensor detects black line
    do
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    while(frontArraySensorValues[5] < 400);
  }

  //cross left turn
  //keep turning left until left IR sensor detects two black lines
  //and the middle IR sensor detects a line
  else if(mode == 1)
  {
    //drive robot forward for half a second
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    delay(600);
  
    // turn on robot left
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);


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

    //keep turning left until middle IR sensor detects the line
    while(frontArraySensorValues[5] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
  }
  
  //using Left IR sensor
  //go froward for delay time to prevent double reads
  //turn right until middle IR sensor detects the line
  else if(mode == 2)
  {
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);

    delay(200);
    
    Left.readCalibrated(LsensorValue);
    
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }
    
    hardStop();
    delay(100);

    //turn left 
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);
    
    //turn left until middle IR sensor detects a line
    while(frontArraySensorValues[5] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
  }

  //complicated
  //tldr: turn left
  else if(mode == 3)
  {  
    //go forward for delay time to prevent double readings
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    delay(200);

    //go forward until left IR sensor detects a line
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) //
    {
          Left.readCalibrated(LsensorValue);
    }

    //stop when either 1&2 or 7&8 IR sensors from the array detect the line
    done = false;
    while(!done)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
      if((frontArraySensorValues[0] > 400 && frontArraySensorValues[1] > 400) || (frontArraySensorValues[6] > 400 && frontArraySensorValues[7] > 400) )
      done = true;
    }
  
    hardStop();
    delay(100);

    //turn left for time delay to prevent double readings
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);

    //Stay in this loop till the right IR crosses the line
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }
    
    //turn left until IR sensor array detects a line
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[6] < 400)  //<400???
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
  }

  //turn left using back IR sensor
  else if(mode == 4)
  {
    hardStop();
    delay(50);
    
    // turn on robot left
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);


    //keep turning left until left IR sensor detects two black lines
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

    //keep turning left until middle IR sensor detects the line
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[6] < 300 && frontArraySensorValues[5] < 300)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    
  hardStop();
  delay(100);
  
  }
  else if(mode == 5)
  {
    //go forward for delay time to prevent double readings
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    delay(200);

    //go forward until left IR sensor detects a line
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) //
    {
          Left.readCalibrated(LsensorValue);
    }

    //stop when either 1&2 or 7&8 IR sensors from the array detect the line
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
  
    hardStop();
    delay(50);

    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);


    //Stay in this loop till the right IR crosses the line
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }
    
    //turn left until IR sensor array detects a line
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[5] < 400) 
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
  }
  else if(mode == 6)
  {
    hardStop();
    delay(50);
    
    // turn on robot left
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);

    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    qtraBack.readCalibrated(backArraySensorValues);
    while(backArraySensorValues[3] < 400) 
    {
      qtraBack.readCalibrated(backArraySensorValues);
    }

    hardStop();
    delay(50);
  }
  else if(mode == 7)
  {
    RForward();
    LReverse();
    assignSpeed(MotorR, MotorL);
    
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[0] < 400)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }

    delay(50);
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[0] < 400)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }
    
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[0] >600)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }

    delay(75); // go a little more
    hardStop();
    delay(50);
    
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);

    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }

    RReverse();
    LReverse();
    assignSpeed(MotorR, MotorL);
    
    delay(300);

  }

 if(resetTimer)
 {         
    timer0 = 0;
 }

  //assign each motor the original value of motor B
    RForward();
    LForward();
    assignSpeed(baseSpeed, baseSpeed);



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
    retrieveToken();
    carouselCounter++;
  }

  if(mode == 0)
  {
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    
    delay(600);
  
    // turn right
    RReverse();
    LForward();
    assignSpeed(MotorR, MotorL);

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
  
    while(frontArraySensorValues[2] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    
  }

  else if(mode == 1)
  {
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    delay(200);
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 500)
    {
      Right.readCalibrated(RsensorValue);
    }
    
    while(!done)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
  
      if((frontArraySensorValues[1] > 400 && frontArraySensorValues[0] > 400) || (frontArraySensorValues[7] > 400 && frontArraySensorValues[6] > 400) )
        done = true;
    }
  
    done = false;
  
     hardStop();
     delay(50);
  
    //turn right 90 degrees
    LForward();
    RReverse();
    assignSpeed(MotorR, MotorL);
    
    delay(500); // TEST TURN DELAY RIGHT NEEDS ADJUSTMENTS PROB
        
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue);
    }
  

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[2] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
  
    hardStop();
    delay(50);
  }
  
  else if(mode == 2)
  {
    LForward();
    RReverse();
    assignSpeed(MotorR, MotorL);
    
    while(tempCounter < 3)
      {
        Right.readCalibrated(RsensorValue);
    
        if(RsensorValue[0] > 400)
        {
          tempCounter++;
          if(tempCounter == 1 || tempCounter == 2)
            delay(50);
        }
       
      }
      
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    hardStop();
    delay(100);
  }
  else if(mode == 3)
  {
    hardStop();
    delay(50);
    
    // turn on robot left
    RReverse();
    LForward();
    assignSpeed(MotorR, MotorL);


    //keep turning left until left IR sensor detects two black lines
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

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(100);
    
  }
    else if(mode == 4)
    {
      hardStop();
      delay(100);
    
      // turn on robot left
      RReverse();
      LForward();
      assignSpeed(MotorR, MotorL);

    //keep turning right until right IR sensor detects 
  
      Right.readCalibrated(RsensorValue);
      while(RsensorValue[0] < 400)
      { 
        Right.readCalibrated(RsensorValue);
      }
     
    

      qtraBack.readCalibrated(backArraySensorValues);
      while(backArraySensorValues[3] < 200)
      {
        qtraBack.readCalibrated(backArraySensorValues);
      }
    }

 
  
  //assign each motor the original value of motor B
  RForward();
  LForward();
  assignSpeed(baseSpeed, baseSpeed);

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
  assignSpeed(MotorR, MotorL);
  
  while(lineCounter < lineNumber + 1)
  {
    
    qtraFront.readCalibrated(frontArraySensorValues);
    if(frontArraySensorValues[1] > 400  && timer0 > interval0)
    {
      lineCounter++;
      timer0 = 0;
    }
    
  }

  hardStop();
  delay(100);

  
  //turn right 90 degrees
  RReverse();
  LForward();
  assignSpeed(MotorR, MotorL);
  delay(500);
  
  while(!done)
  {
    qtraFront.readCalibrated(frontArraySensorValues);

    if(frontArraySensorValues[1] > 500)
    done = true;
  }


  //assign each motor the original value of motor B
  RForward();
  LForward();
  assignSpeed(baseSpeed, baseSpeed);
  
  timer0 = 0;
  counter++;
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

  if(resetTimer)
  {
    //reset timer
    timer0 = 0;
  }
  
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

  if(magnet)
  {
    retrieveToken();
    carouselCounter++;
  }
  
  if(resetTimer)
  {
    //reset timer
    timer0 = 0;
  }

  if(motorsForward)
  {
    RForward();
    LForward();
    assignSpeed(baseSpeed, baseSpeed);
  }
  else
  {
    turn180(0);
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

//WORKS AMAZINGLY (before version 7)
void setCalibration()
{
  unsigned int temp;

  //
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
    qtraFront.calibratedMinimumOn[j] = temp;
    i = i + 4;
    j++;
  }

  j = 0;
  while(i < 80)
  {
    EEPROM.get(i,temp);
    qtraFront.calibratedMaximumOn[j] = temp;
    i = i + 4;
    j++;
  }
  
  j = 0;
  while(i < 112)
  {
    EEPROM.get(i,temp);
    qtraBack.calibratedMaximumOn[j] = temp;
    i = i + 4;
    j++;
  }
  
  j = 0;
  while(i < 144)
  {
    EEPROM.get(i,temp);
    qtraBack.calibratedMinimumOn[j] = temp;
    i = i + 4;
    j++;
  }

  EEPROM.get(144,temp);
  side.calibratedMinimumOn[0] = temp;

  EEPROM.get(148,temp);
  side.calibratedMaximumOn[0] = temp;
}

//used at beginning of loop for line following
void assignSpeed(int MotorR, int MotorL)
{
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
}

void readIRSensors()
{
    Left.readCalibrated(LsensorValue);
    Right.readCalibrated(RsensorValue);
    side.readCalibrated(SsensorValue);
    qtraBack.readCalibrated(backArraySensorValues);
}

//Used when rotating CW: From Magenta, Yellow, Grey
void resetCarouselCCW()
{
  digitalWrite(dirPin,0); // Enables the motor to move in a particular direction
  while(digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

//  for(int i = 0; i < 20; i++)
//  {
//    digitalWrite(stepPin,HIGH); 
//    delayMicroseconds(delayTime); 
//    digitalWrite(stepPin,LOW); 
//    delayMicroseconds(delayTime); 
//  }
  

  
}


//Used when coming from sensor side of the carousel --- rotating CCW: From Sensor, Blue, Green, Red, Cyan
void resetCarouselCW()
{
//  bool depressed = false;
//
//  while(!depressed){
  digitalWrite(dirPin,1); // Enables the motor to move in a particular direction
  while(digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
  for(int i = 0; i < 300; i++)
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
  delay(100);

  digitalWrite(dirPin,0);
  
  while(digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(2000);
  
  if(digitalRead(limitSwitch)){
    while(!digitalRead(limitSwitch))
    {
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(delayTime); 
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(delayTime); 
    }
  }
  
  
  //}
  
}

//  positioning carousel
void ToBlue(){turn(1,HIGH, blueOffset);}
void ToGreen(){turn(2,HIGH,greenOffset);}
void ToRed(){turn(3,HIGH,redOffset);}
void ToCyan(){turn(4,HIGH,cyanOffset);}
void ToSensor(){turn(1,HIGH, sensorOffset);}
void ResetBlue(){resetCarouselCCW();};
void ResetRed(){resetCarouselCCW();};
void ResetGreen(){resetCarouselCCW();};
void ResetCyan(){resetCarouselCCW();};

//   positioning carousel
//   resetCarousel(LOW);
void ToMagenta(){turn(4,LOW,magentaOffset);}
void ToYellow(){turn(3,LOW,yellowOffset);}
void ToGrey(){turn(2,LOW,greyOffset);}
void ResetMagenta(){resetCarouselCW();};
void ResetYellow(){resetCarouselCW();};
void ResetGrey(){resetCarouselCW();};

void magnetUp()
{
  myservo.write(servoUp);
  delay(500);
}

void magnetDown()
{
  myservo.write(servoDown);
  delay(500);
  //Magnet control circuit uses inverted logic
  digitalWrite(magnetPin, LOW);
  delay(500);
}

void dropToken()
{
  digitalWrite(magnetPin, HIGH);
  delay(500);
}

//LOW couterclockwise
//HIGH clockwise

void retrieveToken(){
  
  magnetDown();
  magnetUp();
  readColor();

  //Insert color sensor reading here

  switch(color)
  {
    case 'b':
    ToBlue();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 'g':
    ToGreen();
    dropToken();
    resetCarouselCCW();    
    break;
    
    case 'r':
    ToRed();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 'c':
    ToCyan();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 'm':
    ToMagenta();
    dropToken();
    resetCarouselCW();
    break;
    
    case 'y':
    ToYellow();
    dropToken();
    resetCarouselCW();
    break;
    
    case 'x':
    ToGrey();
    dropToken();
    resetCarouselCW();
    break;
    
    default:
    
    break;
    
  }

}

void depositTokens(){

  switch(color)
  {
    case 'b':
    ToSensor();
    delay(100);
    ToBlue();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'g':
    ToSensor();
    delay(100);
    ToGreen();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);   
    break;
    
    case 'r':
    ToSensor();
    delay(100);
    ToRed();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'c':
    ToSensor();
    delay(100);
    ToCyan();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'm':
    ToSensor();
    delay(100);
    ToMagenta();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'y':
    ToSensor();
    delay(100);
    ToYellow();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'x':
    ToSensor();
    delay(100);
    ToGrey();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    default:
    
    switch(tempCounter){
    
    case 0:
    ToSensor();
    delay(50);
    ToBlue();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
    
    case 1:
    ToSensor();
    delay(50);
    ToGreen();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
    
    case 2:
    ToSensor();
    delay(50);
    ToRed();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
    
    case 3:
    ToSensor();
    delay(50);
    ToCyan();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
    
    case 4:
    ToSensor();
    delay(50);
    ToMagenta();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
    
    case 5:
    ToSensor();
    delay(50);
    ToYellow();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    tempCounter++;
    break;
   
    default:
    ToSensor();
    delay(50);
    ToGrey();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    }
    
    break;
  }
  

}

void raiseTokens(){
  myservo.write(servoMiddle);
  delay(1000);
  digitalWrite(magnetPin, LOW);
  myservo.write(servoUp);
  delay(1000);
}

void turn(int spaces, int dir, int offset)
{
  
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*spaces + offset; x++) 
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}


void setMotorPins()
{
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void setServo()
{
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin, minPWM, maxPWM);
}

void setStepperPins()
{
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(limitSwitch,INPUT);
}

void initCalibration()
{
  Left.calibrate();   
  Right.calibrate();      
  side.calibrate();
  qtraFront.calibrate();
  qtraBack.calibrate();
}

int getError()
{
  int position = qtraFront.readLine(frontArraySensorValues);
  return (position-3500);  
}

void calculatePID(int error)
{
  proportional = kp * error;
  integral = ki * (integral + proportional);
  derivative = kd * (error-lastError);
}

void turn180(int mode)
{
  MotorL = 75;
  MotorR = 75;
  int tempCounter = 0;

  if(mode == 0)
  {
    RReverse();
    LForward();
    assignSpeed(MotorR, MotorL);
  
    
    Right.readCalibrated(RsensorValue);
    while(tempCounter < 3)
    {
      
      Right.readCalibrated(RsensorValue); 
      if(RsensorValue[0] < 400)
      {
        tempCounter++;
        delay(550);
      }
      
      digitalWrite(leftLED, HIGH);
    }
    
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
      digitalWrite(leftLED, LOW);
    }
    hardStop();
    delay(100);
  }
  else if( mode == 1)
  {

    hardStop();
    delay(50);

    /////drop tokens
    
    RReverse();
    LForward();
    assignSpeed(MotorR, MotorL);
  
    
    Right.readCalibrated(RsensorValue);
    while(tempCounter < 2)
    {
      
      Right.readCalibrated(RsensorValue); 
      if(RsensorValue[0] < 400)
      {
        tempCounter++;
        delay(550);
      }
      
      digitalWrite(leftLED, HIGH);
    }
    
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
      digitalWrite(leftLED, LOW);
    }

    
    counter++;
  }
  
}

void outputCalibration()
{
  Serial.print("Calibration Min LRM: ");
      Serial.print(Left.calibratedMinimumOn[0]);
      Serial.print(' ');
      Serial.print(Right.calibratedMinimumOn[0]);
      Serial.print(' ');
      Serial.print(side.calibratedMinimumOn[0]);
      Serial.print(' ');
      Serial.println();

      Serial.print("Calibration Min Front: ");
      for(int i = 0; i < 8; i++)
      {
         Serial.print(qtraFront.calibratedMinimumOn[i]); 
         Serial.print(' ');
      }

      Serial.println();
      
      Serial.print("Calibration Min Back: ");
      for(int i = 0; i < 8; i++)
      {
         Serial.print(qtraBack.calibratedMinimumOn[i]); 
         Serial.print(' ');
      }
        Serial.println();
      
        //print the calibration maximum values measured when emitters were on
        Serial.print("Calibration Max LRM: ");
        Serial.print(Left.calibratedMaximumOn[0]);
        Serial.print(' ');
        Serial.print(Right.calibratedMaximumOn[0]);
        Serial.print(' ');
        Serial.print(side.calibratedMaximumOn[0]);
        Serial.print(' ');

        Serial.println();

        Serial.print("Calibration Max Front: ");
      for(int i = 0; i < 8; i++)
      {
        Serial.print(qtraFront.calibratedMaximumOn[i]); 
        Serial.print(' ');
      }

      Serial.println();

      Serial.print("Calibration Max Back: ");
      for(int i = 0; i < 8; i++)
      {
        Serial.print(qtraBack.calibratedMaximumOn[i]); 
        Serial.print(' ');
      }
        Serial.println();
        Serial.println();
}


void turnLeft45(int mode)
{
  MotorL = 75;
  MotorR = 75;

  if(mode == 0)
  {
    RForward();
    LReverse();
    assignSpeed(0, MotorL);
    delay(300);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(50);

    counter++;
    
  }
  if(mode == 1)
  {
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
    
    RForward();
    LReverse();
    assignSpeed(0, MotorL);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(50);

    counter++;
    
  }
}

void turnRight45(int mode)
{
  MotorL = 75;
  MotorR = 75;
  if(mode == 0)
  {
     LForward();
     RForward();
    assignSpeed(MotorR, MotorL);
    delay(400);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400)
    {
      Left.readCalibrated(LsensorValue);
    }
    
    hardStop();
    delay(100);

    LForward();
    RReverse();
    assignSpeed(MotorR, MotorL);
    
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(100);
    
  }
  counter++;
  timer0 = 0;
}

void forwardStop(){

    RForward();
    LForward();
    assignSpeed(baseSpeed, baseSpeed);
    
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 300)
    {
      side.readCalibrated(SsensorValue);
    }

    hardStop();
    delay(100);
}

void readColor()
{
  uint16_t r, g, b, c, colorTemp, lux;
  bool thrice = false;

  char color1 = 'z';
  char color2 = 'z';
  char color3 = 'z';

  ToSensor();
  delay(1000);
  //color sensor stuff

  double rd = r;
  double rg = g;
  double rb = b;

while(!thrice){
  
  tcs.getRawData(&r, &g, &b, &c);
//  Serial.print("R: "); Serial.print(rd/c); Serial.print(" ");
//  Serial.print("G: "); Serial.print(rg/c); Serial.print(" ");
//  Serial.print("B: "); Serial.print(rb/c); Serial.print(" ");
  printColor(r,g,b,c);
  color1 = color;

  delay(10);
  
  tcs.getRawData(&r, &g, &b, &c);
//  Serial.print("R: "); Serial.print(rd/c); Serial.print(" ");
//  Serial.print("G: "); Serial.print(rg/c); Serial.print(" ");
//  Serial.print("B: "); Serial.print(rb/c); Serial.print(" ");
  printColor(r,g,b,c);
  color2 = color;

  delay(10);

  tcs.getRawData(&r, &g, &b, &c);
//  Serial.print("R: "); Serial.print(rd/c); Serial.print(" ");
//  Serial.print("G: "); Serial.print(rg/c); Serial.print(" ");
//  Serial.print("B: "); Serial.print(rb/c); Serial.print(" ");
  printColor(r,g,b,c);
  color3 = color;

  if(color1 == color2 && color2 == color3)
    thrice = true;
}

}

void printColor(double r, double g, double b, double c)
{
   if(r/c>.56 && r/c<.76 && g/c >.1 && g/c <.3 && b/c>.08 && b/c<.28)
  {
    Serial.println("color is red");//R: 0.66 G: 0.20 B: 0.18 color is red
    color = 'r';
  }
  else if(r/c>.23 && r/c<.43 && g/c>.45 && g/c<.65 && b/c >.18 && b/c<.38)
  {
    Serial.println("color is green");//R: 0.33 G: 0.55 B: 0.28 color is grey
    color = 'g';
    
  }
  else if(r/c>.07 && r/c<.27 && g/c>.22 && g/c<.42 && b/c>.41 && b/c<.61)
  {
    Serial.println("color is blue");//R: 0.17 G: 0.32 B: 0.51 color is blue
    color = 'b';
  }
  else if(r>14000 && r<17000 && g>9000 && g<12000 && b>12000 && b<16000)
  {
    Serial.println("color is Magenta");//R: 15629 G: 10205 B: 13413 NOPE
    color = 'm';
  }
  else if(r > 48000 && r < 64500 && g > 55000 && g < 68500 && b > 28000 && b < 37750)
  {
    Serial.println("color is yellow"); //R: 61512 G: 65535 B: 34756 NOPE
    color = 'y';
  }
  else if(r/c>.1 && r/c<.3 && g/c>.4 && g/c<.65 && b/c>.45 && b/c<.7)
  {
    Serial.println("color is cyan"); //R: 0.21 G: 0.55 B: 0.59 color is grey
    color = 'c';
  }
  else if(r>5000 && r<9000 && g>8000 && g<12000 && b>8000 && b<11000)
  {
    Serial.println("color is grey");//R: 7462 G: 9922 B: 9211 color is grey
    color = 'x';
  }
  else
  {
    Serial.println("NOPE");
    color = 'x';
  }
}
