#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>
#include <Servo.h>

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
bool ML = false;
bool back = false;
bool arrayStop = false;

//tracks where on the track the robot is
int counter = 1;
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
int magentaOffset = 65;
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
char color = 'z';

void setup()
{
    
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
  if(buttonFlag)
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
    assignSpeed();
  
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
      //delay after stop, reset Timer, motors forward afterwards, increment counter
      hardStop(0,true, true, true, true);   
      break;

      case 9:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 12:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 14:
      hardStop(0,true, true, true, true);     
      break;
      
      //turnaround 180 after picking up token
      case 17:
      hardStop(0,true, false, true, true);
      break;

      case 19:
      ML = true;
      break;
      
      case 20:                            //turning into inner loop
      turnRight90(4,0,true,true, 'h', false);
      hardStop();
      ML = false;
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
      hardStop();
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
      ML = true;
      break;

      //Into and Out of Green
      case 51:
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      ML = false;
      break;

      case 53:
      right = true;
      break;

      //Into Red and Turn Around
      case 54:
      hardStop();
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
      hardStop();
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
      ML = true;
      break;

      //Into and Out of Magenta
      case 66:
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      ML = false;
      break;

      case 68:
      right = true;
      break;

      //Into Yellow and Turn Around
      case 69:
      hardStop();
      depositTokens();
      delay(100);
      turn180(1);
      
      hardStop();
      delay(10000);
      right = false;
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
  if(LsensorValue[0] >= sensorSensitivity && timer0 > interval && !right && !ML && !arrayStop && !back)
  {
      counter++;
      timer0 = 0;
      digitalWrite(leftLED, HIGH);
  }
  else if(RsensorValue[0] >= sensorSensitivity && timer0 > interval && right && !ML && !arrayStop && !back)
  {
      counter++;
      timer0 = 0;
      digitalWrite(leftLED, HIGH);
  }
  else if(SsensorValue[0] >= sensorSensitivity && timer0 > interval && !right && ML && !arrayStop && !back)
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
    assignSpeed();
    
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


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
    assignSpeed();
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
    assignSpeed();

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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

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
    assignSpeed();
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


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
  }
  else if(mode == 5)
  {
    //go forward for delay time to prevent double readings
    assignSpeed();
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
    delay(500);

    //turn left for time delay to prevent double readings
    RForward();
    LReverse();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


    //Stay in this loop till the right IR crosses the line
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }
    
    //turn left until IR sensor array detects a line
    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[5] < 400)  //<400???
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    qtraBack.readCalibrated(backArraySensorValues);
    while(backArraySensorValues[3] < 400)  //<400???
    {
      qtraBack.readCalibrated(backArraySensorValues);
    }

    hardStop();
    delay(50);

    
  }

  
  hardStop();
  delay(100);

  //assign each motor the original value of motor B
  continueForward();


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
    retrieveToken();
    carouselCounter++;
  }

  if(mode == 0)
  {
    assignSpeed();
    
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
  
    while(frontArraySensorValues[2] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    
  }

  else if(mode == 1)
  {
    assignSpeed();
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);


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
      delay(50);
    
      // turn on robot left
      RReverse();
      LForward();
      analogWrite(enR, MotorR);
      analogWrite(enL, MotorL);


    //keep turning left until left IR sensor detects two black lines
  
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
  assignSpeed();
  
  while(lineCounter < lineNumber + 1)
  {
    
    qtraFront.readCalibrated(frontArraySensorValues);

    if(frontArraySensorValues[1 ] > 400  && timer0 > interval0)
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
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);
  delay(500);
  
  while(!done)
  {
    qtraFront.readCalibrated(frontArraySensorValues);

    if(frontArraySensorValues[1] > 500)
    done = true;
  }


  //assign each motor the original value of motor B
  continueForward();
  
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
    continueForward();
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

void continueForward(){
  RForward();
  LForward();
  MotorOn();
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
    side.readCalibrated(SsensorValue);
    qtraBack.readCalibrated(backArraySensorValues);
}

void readColor()
{
  ToSensor();
  delay(1000);
  //color sensor stuff
  //
}

//Used when rotating CW: From Magenta, Yellow, Grey
void resetCarouselCCW()
{
  digitalWrite(dirPin,0); // Enables the motor to move in a particular direction
  while(!digitalRead(limitSwitch))
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
  while(!digitalRead(limitSwitch))
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
  
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(2000);
  
  if(digitalRead(!limitSwitch)){
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
    switch(carouselCounter){
    
    case 0:
    ToBlue();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 1:
    ToGreen();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 2:
    ToRed();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 3:
    ToCyan();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 4:
    ToMagenta();
    dropToken();
    resetCarouselCW();
    break;
    
    case 5:
    ToYellow();
    dropToken();
    resetCarouselCW();
    break;
    
    case 6:
    ToBlue();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 7:
    ToGreen();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 8:
    ToRed();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 9:
    ToCyan();
    dropToken();
    resetCarouselCCW();
    break;
    
    case 10:
    ToMagenta();
    dropToken();
    resetCarouselCW();
    break;
    
    case 11:
    ToYellow();
    dropToken();
    resetCarouselCW();
    break;
    
    default:
    ToBlue();
    dropToken();
    resetCarouselCCW();
    break;
    
    }
    
  }

}

void depositTokens(){
  //Insert color sensor reading here

  switch(color)
  {
    case 'b':
    ToSensor();
    ToBlue();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'g':
    ToSensor();
    ToGreen();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);   
    break;
    
    case 'r':
    ToSensor();
    ToRed();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'c':
    ToSensor();
    ToCyan();
    raiseTokens();
    resetCarouselCCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'm':
    ToSensor();
    ToMagenta();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'y':
    ToSensor();
    ToYellow();
    raiseTokens();
    resetCarouselCW();
    digitalWrite(magnetPin, HIGH);
    break;
    
    case 'x':
    ToSensor();
    delay(50);
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

void center()
{
  MotorL = 75;
  MotorR = 75;


  RReverse();
  LReverse();
  analogWrite(enR, MotorR);
  analogWrite(enL, MotorL);

  
  Left.readCalibrated(LsensorValue);
  while(LsensorValue[0] < 800)
  {
    Left.readCalibrated(LsensorValue);
  }

  hardStop();
  delay(100);
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
  
    
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
    continueForward();
    
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 300)
    {
      side.readCalibrated(SsensorValue);
    }

    hardStop();
    delay(50);

    /////drop tokens
    

    
    RReverse();
    LForward();
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
  
    
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
    analogWrite(enR, 0);
    analogWrite(enL, MotorL);
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
    
    RForward();
    LReverse();
    analogWrite(enR, 0);
    analogWrite(enL, MotorL);
    
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
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
    analogWrite(enR, MotorR);
    analogWrite(enL, MotorL);
    
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

