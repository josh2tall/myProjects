#include <QTRSensors.h>
#include <elapsedMillis.h>
#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SPI.h>

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
int MotorL = 215;
int MotorR = 215;
int baseSpeed = 215;
int lastError = 0;

bool turnError = false;
bool right = false;
bool sideSensor = false;
bool back = false;
bool arrayStop = false;

//tracks where on the track the robot is
int counter = 1;
int roundCounter = 2;
int tokenCounter = 0;

//here
//////////////COUNTER ^^^^ ///////////////////////////////////////////////////////////////////////////////////////////////////


int carouselCounter = 0;
//create a timer
elapsedMillis timer0 = 0;

//interval between front IR sensor readings in ms
int interval = 100;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;

//pid constants
double kp = .06;
double kd = .06;
double ki = 0;

double bkp = .01;
double bkd = .01;
double bki = .01;


//servo stuff
int maxPWM = 2300;
int minPWM = 700;
int pos = 0;    // variable to store the servo position
int servoPin = 30;
int servoUp = 145;
int servoDown = 39;
int retrieveHeight = 119;
int travelHeight = 160;
int retrieveGreyHeight = 135;

//carousel stuff
const int stepPin = 3; 
const int dirPin = 4; 
const int limitSwitch = 32;
const int fullRev = 3200;//full rev = 200
int steps = 175 * 2;
int delayTime = 250;

int tempCounter = 0;

//carousel offsets
int blueOffset = -50;
int greenOffset = -55;
int redOffset = -35;
int sensorOffset = 0;
int cyanOffset = -35;
int magentaOffset = 75;
int yellowOffset = 75;
int greyOffset = 75;


int blueCounter = 0;
int greenCounter = 0;
int redCounter = 0;
int cyanCounter = 0;
int magentaCounter = 0;
int yellowCounter = 0;
int greyCounter = 0;

const int leftLED = 49;

//magnet
int magnetPin = 7;

//pause button
bool buttonFlag = 1;
bool onoff = 0;
int buttonPin = 52;

//color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
uint16_t r, g, b, c, colorTemp, lux;
char color = 'n';

void setup()
{
  Serial.begin(9600);
  
  //color sensor
  tcs.begin();    

  //lcd
// display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
// startUpDisplay();
  

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
  delay(500);
  resetCarouselCCW();
  delay(500);
  digitalWrite(leftLED, HIGH);
  delay(250);
  digitalWrite(leftLED, LOW); 


  
}


void loop()
{

 // displayInfo();

  //starts and pauses all processes until the
  //button is pressed again
  buttonFlag = digitalRead(52);
  if(!buttonFlag)
  {
    //timer1 = 0;
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
      case 1:
      //inital turn from the white square, number of lines to cross
      StartTurnRight90Line(3);
      break;
      
      case 3:
      //mode, initial stop delay, reset timer, increment counter, initial medium or hardstop
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 5:
      //void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
      hardStop(0,true, true, true, true);   
      break;

      case 7:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 9:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 11:
      //void hardStop(int delayAfter, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
      hardStop(0,true, true, true, true);     
      break;
      
      //turnaround 180 after picking up token
      case 13:
      turn180(2);
      break;

      case 15:
      sideSensor = true;
      break;
      
      case 16:                            //turning into inner loop// reverses to align
      turnRight90(4,0,true,true, 'h', false);
      counter++; //remove if using revese line following
      sideSensor = false;
      break;

      case 19:
      //turnRight90(int mode, int stoppingDelay, bool resetTimer, bool IncrementCounter, char Stop, bool magnet)
      turnRight90(1, 0, true, true,'h', true);
      break;

      case 21:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 23:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 25:
      hardStop(0,true, true, true, true);
      break;

      
      case 27:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 29:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      
      case 33:
      //Into Blue
      if(roundCounter == 1)
      {
        turnRight45(0);
      }
      else
      {
        counter = 100;
      } 
      break;

      //Turn Around
      case 38:
      color = 'b';
      forwardStop(0);
      depositTokens();
      delay(100);
      turn180(1);
      break;

      case 40:
      forwardStop(0);
      counter++;
      break;

      //Toward Green
      case 41:
      turnLeft45(0);
      break;

      case 43:
      sideSensor = true;
      break;

      //Into and Out of Green
      case 44:
      color = 'g';
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      sideSensor = false;
      break;

      case 46:
      right = true;
      break;

      //Into Red and Turn Around
      case 47:
      color = 'r';
      forwardStop(0);
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 49:
      forwardStop(0);
      counter++;
      break;

      //Toward Cyan
      case 50:
      turnLeft45(0);
      break;
      
      case 52:
      right = true;
      break;

      //Into Cyan and Turn Around
      case 53:
      color = 'c';
      forwardStop(0);
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 55:
      forwardStop(0);
      counter++;
      break;

      //Toward Magenta
      case 56:
      turnLeft45(0);
      break;

      case 58:
      sideSensor = true;
      break;

      //Into and Out of Magenta
      case 59:
      color = 'm';
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      delay(100);
      turnRight90(3, 0,true,false,'h', false);
      sideSensor = false;
      break;

      case 61:
      right = true;
      break;

      //Into Yellow and Turn Around
      case 62:
      color = 'y';
      forwardStop(0);
      depositTokens();
      delay(100);
      turn180(1);
      right = false;
      break;

      case 64:
      forwardStop(0);
      turnLeft45(0);
      hardStop();
      delay(100);
      timer0 = 0;
      break;

      case 65:
      if(timer0 > 3300)
      {
        hardStop();
        delay(100);
        turnLeft90(7, 0, true, false, 'h', false);      
        while(true)
        {
          hardStop();
          delay(100);
        }
        counter++;
      }
      break;
    
      case 100:
      turnLeft90(8, 0, true, true, 'h', false);
      break;
      
      case 102:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 104:
      hardStop(0,true, true, true, true); 
      break;

      case 106:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 108:
      turnLeft90(5, 0, true, true, 'h', true);
      break;

      case 110:
      hardStop(0,true, true, true, true);     
      break;

      case 113:
      hardStop(1,true, true, true, true);
      if(roundCounter == 2)
      {
        turnRight45(0);
        counter = 35;
      }
      else
      {
        counter = 200;
      }
      break;

      case 200:
      turnLeft90(10, 0, true, true, 'h', false);
      break;
      
      case 202:
      turnLeft90(0, 0, true, true, 'h', true);
      break;

      case 204:
      hardStop(0, true, true, true, true); 
      break;

      case 206:
      turnLeft90(0, 0, true, true, 'h', true);
      break;

      case 208:
      turnLeft90(0, 0, true, true, 'h', true);
      break;

      case 210:
      hardStop(0,true, true, true, true);     
      break;

      case 212:
      turnRight45(2);
      break;

      case 214:
      color = 'b';
      forwardStop(0);
      depositTokens();
      turn180(1);
      break;

      case 216:
      forwardStop(0);
      counter++;
      break;

      //Toward Green
      case 217:
      turnLeft45(0);
      break;

      case 219:
      sideSensor = true;
      break;

      //Into and Out of Green
      case 220:
      color = 'g';
      turnLeft90(6, 0, true, true, 'h', false);
      forwardStop(0);
      depositTokens();
      turn180(3);
      sideSensor = false;
      break;

      //Into Grey
      case 225:
      color = 'x';
      forwardStop(1);
      depositTokens();
      turnLeft90(9, 0, true, true, 'h', false);
      right = true;
      break;
 
       //Into Red and Turn Around
      case 231:
      color = 'r';
      forwardStop(0);
      depositTokens();
      turn180(1);
      right = false;
      break;

      case 233:
      forwardStop(0);
      counter++;
      break;

      //Toward Cyan
      case 234:
      turnLeft45(0);
      break;
      
      case 236:
      right = true;
      break;

      //Into Cyan and Turn Around
      case 237:
      color = 'c';
      forwardStop(0);
      depositTokens();
      turn180(1);
      right = false;
      break;

      case 239:
      forwardStop(0);
      counter++;
      break;

      //Toward Magenta
      case 240:
      turnLeft45(0);
      break;

      case 242:
      sideSensor = true;
      break;

      //Into and Out of Magenta
      case 243:
      color = 'm';
      turnLeft90(6, 0, true, true, 'h', false);
      hardStop();
      depositTokens();
      turnRight90(3, 0,true,false,'h', false);
      sideSensor = false;
      break;

      case 245:
      right = true;
      break;

      //Into Yellow and Turn Around
      case 246:
      color = 'y';
      forwardStop(0);
      depositTokens();
      turn180(1);
      right = false;
      break;

      case 248:
      turnLeft45(1);
      hardStop();
      delay(100);
      timer0 = 0;
      break;

      case 249:
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
    double speedChange = proportional + integral + derivative;
    if(error >= 0)
    {
      MotorL = baseSpeed - speedChange;
      MotorR = baseSpeed;
    }
    else
    {
      MotorR = baseSpeed + speedChange;
      MotorL = baseSpeed;
    }
  
    //ensure the motor doesnt go passed limits
    if(MotorL < 0)
      MotorL = 0;
  
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
  MotorL = 215;
  MotorR = 215;


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

  //Outer Left Turn
  //move forward for delay time
  //turn right until middle IR sensor detects the line
  if(mode == 0)
  {

    hardStop();
    delay(100);
    adjustPosition(0);
    
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    delay(100);

    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] > 400) //
    {
          Left.readCalibrated(LsensorValue);
    }
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) //
    {
          Left.readCalibrated(LsensorValue);
    }

    hardStop();
    delay(100);
    if(magnet)
    {
      //adjustPosition(2);
      retrieveToken();
      carouselCounter++;
    }
    
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    
    do
    {
        side.readCalibrated(SsensorValue);
    }while(SsensorValue[0] < 400);
    
    delay(200);
    
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

  else if(mode == 5)
  {
    hardStop();
    delay(100);
    adjustPosition(0);
    
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);
    //delay(100); //put back if it doesnt work


    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] > 400) //
    {
          Left.readCalibrated(LsensorValue);
    }
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) //
    {
          Left.readCalibrated(LsensorValue);
    }

    hardStop();
    delay(100);
    if(magnet)
    {
      retrieveToken();
      carouselCounter++;
    }
    
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

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
  
    hardStop();
    delay(100);

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

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400) 
    {
          Right.readCalibrated(RsensorValue);
    }
    delay(150);
    
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
  else if(mode == 8)
  {
    RForward();
    LForward();
    assignSpeed(MotorL,MotorR);
    delay(200);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    delay(200);
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[0] < 400)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }

    RForward();
    LReverse();
    assignSpeed(MotorL,MotorR);

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400) 
    {
          Right.readCalibrated(RsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }

    
    hardStop();
    delay(100);
    
  }

  else if(mode == 9)
  {
    RForward();
    LReverse();
    assignSpeed(MotorL,MotorR);
    
    digitalWrite(leftLED, HIGH);
    delay(700);
    digitalWrite(leftLED, LOW);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 900) 
    {
          Left.readCalibrated(LsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400)
    {
          qtraFront.readCalibrated(frontArraySensorValues);
    }
    

    hardStop();
    delay(100);
  }

  else if(mode == 10)
  {
    RForward();
    LForward();
    assignSpeed(MotorL,MotorR);
    delay(200);
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    delay(200);
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) 
    {
          Left.readCalibrated(LsensorValue);
    }

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }

    delay(200);
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
    
    RForward();
    LReverse();
    assignSpeed(MotorL,MotorR);

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
    adjustPosition(0);
    retrieveToken();
  }

  if(mode == 1)
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
      delay(100);
      Right.readCalibrated(RsensorValue);
      while(RsensorValue[0] < 400)
      { 
        Right.readCalibrated(RsensorValue);
      }
    
      delay(100);
//      qtraBack.readCalibrated(backArraySensorValues);
//      while(backArraySensorValues[3] < 200)
//      {
//        qtraBack.readCalibrated(backArraySensorValues);
//      }

      hardStop();
      delay(100);
      //ReverseLineFollowing(0);
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
  MotorL = 215;
  MotorR = 215;
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
void hardStop(int mode, bool resetTimer, bool motorsForward, bool incrementCounter, bool magnet)
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


  if(magnet)
  {
    if(mode == 0)
    {
    adjustPosition(0);
    }
    else if(mode == 1)
    {
      //adjustPosition(2);
    }
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
    assignSpeed(baseSpeed, baseSpeed);
  
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue); 
    }

    delay(100);

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue); 
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
      digitalWrite(leftLED, LOW);
    }

    hardStop();
    delay(100);
    
    counter++;
  }

  else if(mode == 2)
  {
    //go forward for delay time to prevent double readings
    RForward();
    LForward();
    assignSpeed(MotorR, MotorL);


    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] > 400) //
    {
          Left.readCalibrated(LsensorValue);
    }
    
    Left.readCalibrated(LsensorValue);
    while(LsensorValue[0] < 400) //
    {
          Left.readCalibrated(LsensorValue);
    }

    hardStop();
    delay(100);

    //adjustPosition(2);
    retrieveToken();
    carouselCounter++;

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
    
    counter++;
  }

  else if(mode == 3)
  {

    RReverse();
    LForward();    
    assignSpeed(MotorR, MotorL);
  
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue);
    }
    delay(200);
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue);
    }
    delay(200);
    
    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
      Right.readCalibrated(RsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[3] < 400) 
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }
    hardStop();
    delay(100);
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

    delay(400);
    
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
    delay(600);
    
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
  
  if(mode == 1)
  {
    LForward();
    RForward();
    assignSpeed(MotorR, MotorL);

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
    
    delay(200);
    
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }

    LForward();
    RReverse();
    assignSpeed(MotorR, MotorL);

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(100);
    
  }
    else if (mode == 2)
    {
      RForward();
      LForward();
      assignSpeed(MotorR, MotorL);

      Left.readCalibrated(LsensorValue);
      while(LsensorValue[0] > 400)
      {
        Left.readCalibrated(LsensorValue);
      }

      Left.readCalibrated(LsensorValue);
      while(LsensorValue[0] < 400)
      {
        Left.readCalibrated(LsensorValue);
      }
    
      hardStop();
      delay(100);

      //adjustPosition(2);
      retrieveToken();
      carouselCounter++;

      LForward();
    RForward();
    assignSpeed(MotorR, MotorL);

    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }
    
    delay(200);
    
    side.readCalibrated(SsensorValue);
    while(SsensorValue[0] < 400)
    {
      side.readCalibrated(SsensorValue);
    }

    LForward();
    RReverse();
    assignSpeed(MotorR, MotorL);

    Right.readCalibrated(RsensorValue);
    while(RsensorValue[0] < 400)
    {
          Right.readCalibrated(RsensorValue);
    }

    qtraFront.readCalibrated(frontArraySensorValues);
    while(frontArraySensorValues[4] < 400)
    {
      qtraFront.readCalibrated(frontArraySensorValues);
    }

    hardStop();
    delay(100);
      
  }
  
  counter++;
  timer0 = 0;
}

void forwardStop(int mode){

    RForward();
    LForward();
    assignSpeed(baseSpeed, baseSpeed);

    if(mode == 0)
    {
      side.readCalibrated(SsensorValue);
      while(SsensorValue[0] < 300)
      {
        side.readCalibrated(SsensorValue);
      }
  
      hardStop();
      delay(100);
    }
    else if(mode == 1)
    {
      side.readCalibrated(SsensorValue);
      while(SsensorValue[0] < 300)
      {
        side.readCalibrated(SsensorValue);
      }
      delay(200);
      
      side.readCalibrated(SsensorValue);
      while(SsensorValue[0] < 300)
      {
        side.readCalibrated(SsensorValue);
      }

      hardStop();
      delay(100);
    }

    timer0 = 0;
}

void adjustPosition(int mode)
{

  int frontPosition = qtraFront.readLine(frontArraySensorValues);
  int maxCounter = 0;
  if(mode == 0)
  {
    while((frontPosition > 3750 || frontPosition < 3250) && maxCounter < 5)
    {
      frontPosition = qtraFront.readLine(frontArraySensorValues);
      if(frontPosition > 3750)
      {
        LReverse();
        RForward();
        assignSpeed(75, 75);
        delay(25);
        hardStop();
        delay(100);
      }
      else if(frontPosition < 3250)
      {
        LForward();
        RReverse();
        assignSpeed(75, 75);
        delay(25);
        hardStop();
        delay(100);
      }
      maxCounter++;
    }
    
  }
  else if(mode == 1)
  {
    while((frontPosition > 4750 || frontPosition < 4250) && maxCounter < 5)
    {
      frontPosition = qtraFront.readLine(frontArraySensorValues);
      if(frontPosition > 4750)
      {
        LReverse();
        RForward();
        assignSpeed(75, 75);
        delay(25);
        hardStop();
        delay(250);
      }
      else if(frontPosition < 4250)
      {
        LForward();
        RReverse();
        assignSpeed(75, 75);
        delay(25);
        hardStop();
        delay(250);
      }
      maxCounter++;
    }
  }
  else if(mode == 2)
  {
    frontPosition = qtraFront.readLine(frontArraySensorValues);

    if(frontArraySensorValues[0] > 300 || frontArraySensorValues[1] > 300 || frontArraySensorValues[2] > 300)
    {
         while(!(frontArraySensorValues[4] > 300)  && maxCounter < 10){
          LForward();
          RReverse();
          assignSpeed(75, 75);
          delay(25);
          hardStop();
          delay(250);

          qtraFront.readLine(frontArraySensorValues);
          maxCounter++;
         }
    }
     else
     {
        while(!(frontArraySensorValues[3] > 300)  && maxCounter < 10){
          LReverse();
          RForward();
          assignSpeed(75, 75);
          delay(25);
          hardStop();
          delay(250);

          qtraFront.readLine(frontArraySensorValues);
          maxCounter++;
          }
    
      
    }
  }
}

void ReverseLineFollowing(int mode)
{
  if(mode == 0)
  {
    int position = qtraBack.readLine(backArraySensorValues);
    int tempCounter0 = 0;
    while(tempCounter0 < 2)
    {
      
      RReverse();
      LReverse();
      assignSpeed(MotorR, MotorL);
  
      position = qtraBack.readLine(backArraySensorValues);
      int error = position-3500;   
      
      //calculate PID values
      calculateBPID(error);
      lastError = error; 
    
      //change MotorL speed depending on PID values
      double MotorLAdjust = proportional + integral + derivative;
      MotorL = baseSpeed + MotorLAdjust;
    
      //ensure the motor doesnt go passed limits
      if(MotorL < 0)
      MotorL = 0;
      
      if(MotorL > 215)
      MotorL = 215;

      side.readCalibrated(SsensorValue);
      if(SsensorValue[0] < 400)
      {
        tempCounter0++;
        delay(300);
      }
    }
  }
}

void calculateBPID(int error)
{
  proportional = bkp * error;
  integral = bki * (integral + proportional);
  derivative = bkd * (error-lastError);
}

///////////////////////////////////////////////////////////////////////////COLOR SENSOR AND CAROUSEL//////////////////////////////////
void readColor()
{

  ToSensor();
  delay(100);
  magnetUp();
  delay(100);
  //color sensor stuff

  color = 'n';

    tcs.getRawData(&r, &g, &b, &c);
    tcs.getRawData(&r, &g, &b, &c);
    Serial.print("R: "); Serial.print(r); Serial.print(" ");
    Serial.print("G: "); Serial.print(g); Serial.print(" ");
    Serial.print("B: "); Serial.print(b); Serial.print(" ");
    printColor(r,g,b,c);


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

  digitalWrite(dirPin,1); // Enables the motor to move in a particular direction
 
  while(digitalRead(limitSwitch))
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
  
  delay(100);

  digitalWrite(dirPin,0);
  
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(100);
  
  if(digitalRead(limitSwitch)){
    while(digitalRead(limitSwitch))
    {
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(delayTime); 
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(delayTime); 
    }
  }
  
  
  //}
  
}
//  positioning carousel from sensor
void ToSensor(){turn(1,HIGH, sensorOffset);}
void SBlue(){turn(1,HIGH, blueOffset);}
void SGreen(){turn(2,HIGH,greenOffset);}
void SRed(){turn(3,HIGH,redOffset);}
void SCyan(){turn(4,HIGH,cyanOffset);}

void SGrey(){turn(2,LOW,greyOffset);}
void SYellow(){turn(3,LOW,yellowOffset);}
void SMagenta(){turn(4,LOW,magentaOffset);}

//  positioning carousel
void ToBlue(){turn(2,HIGH, blueOffset);}
void ToGreen(){turn(3,HIGH,greenOffset);}
void ToRed(){turn(4,HIGH,redOffset);}
void ToCyan(){turn(5,HIGH,cyanOffset);}

void ToGrey(){turn(1,LOW,greyOffset);}
void ToYellow(){turn(2,LOW,yellowOffset);}
void ToMagenta(){turn(3,LOW,magentaOffset);}

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
  delay(250); //previous 500
}

void dropToken()
{
  digitalWrite(magnetPin, HIGH);
  delay(250); //previous 500
}

void lowerTokens(){
  delay(500);
  myservo.write(90);
  delay(300);
  digitalWrite(magnetPin, HIGH);
  delay(200);
  myservo.write(travelHeight);
}

void retrieveToken(){
  tokenCounter++;
  
  magnetDown();
  myservo.write(travelHeight);
  delay(500);
  readColor();
  myservo.write(travelHeight);
  delay(200);
  
  switch(color)
  {
    case 'b':
    SBlue();
    dropToken();
    resetCarouselCCW();
    blueCounter++;
    break;
    
    case 'g':
    SGreen();
    dropToken();
    resetCarouselCCW();
    greenCounter++;    
    break;
    
    case 'r':
    SRed();
    dropToken();
    resetCarouselCCW();
    redCounter++;
    break;
    
    case 'c':
    SCyan();
    dropToken();
    resetCarouselCCW();
    cyanCounter++;
    break;
    
    case 'm':
    SMagenta();
    dropToken();
    resetCarouselCW();
    magentaCounter++;
    break;
    
    case 'y':
    SYellow();
    dropToken();
    resetCarouselCW();
    yellowCounter++;
    break;
    
    case 'x':
    SGrey();
    dropToken();
    resetCarouselCW();
    greyCounter++;
    break;

    case 'n':
    resetCarouselCCW();
    break;
    default:
    break;
    
  }

}

void placeToken(){
  myservo.write(travelHeight);
  delay(200);
  
  switch(color)
  {
    case 'b':
    SBlue();
    dropToken();
    resetCarouselCCW();
    blueCounter++;
    break;
    
    case 'g':
    SGreen();
    dropToken();
    resetCarouselCCW();
    greenCounter++;    
    break;
    
    case 'r':
    SRed();
    dropToken();
    resetCarouselCCW();
    redCounter++;
    break;
    
    case 'c':
    SCyan();
    dropToken();
    resetCarouselCCW();
    cyanCounter++;
    break;
    
    case 'm':
    SMagenta();
    dropToken();
    resetCarouselCW();
    magentaCounter++;
    break;
    
    case 'y':
    SYellow();
    dropToken();
    resetCarouselCW();
    yellowCounter++;
    break;
    
    case 'x':
    SGrey();
    dropToken();
    resetCarouselCW();
    greyCounter++;
    break;
    
    default:
    break;
    
  }

}

void depositTokens(){
int timeVar = 200;

  switch(color)
  {
    case 'b':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar);
    hardStop();
    delay(100);

    ToBlue();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCCW();
    lowerTokens();
    resetCarouselCCW();

    
    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(timeVar*2);
    hardStop();
    delay(100);

    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }

    
    break;
    
    case 'g':
    RForward();
    LForward();
    assignSpeed(75, 75);
    if(roundCounter == 3)
      delay(0);
    else
      delay(timeVar*4);
    hardStop();
    delay(100);
    
    ToGreen();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCCW();
    lowerTokens();  
    resetCarouselCCW();

    
    RReverse();
    LReverse();
    assignSpeed(75, 75);
    if(roundCounter == 3)
      delay(timeVar*2);
    else
      delay(timeVar*4);
    hardStop();
    delay(100);
    
    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }
    
    break;
    
    case 'r':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar);
    hardStop();
    delay(100);
    
    ToRed();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCCW();
    lowerTokens();
    resetCarouselCCW();

    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(2*timeVar);
    hardStop();
    delay(100);

    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }
    break;
    
    case 'c':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar);
    hardStop();
    delay(100);
    
    ToCyan();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCCW();
    lowerTokens();
    resetCarouselCCW();

    
    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(2*timeVar);
    hardStop();
    delay(100);
    
    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }
    
    break;
    
    case 'm':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar*4);
    hardStop();
    
    ToMagenta();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCW();
    lowerTokens();
    resetCarouselCCW();
    
    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(timeVar*4);
    hardStop();
    delay(100);

    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }
 
    break;
    
    case 'y':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar);
    hardStop();
    delay(100);
    
    ToYellow();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCW();
    lowerTokens();
    resetCarouselCCW();

    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(2*timeVar);
    hardStop();
    delay(100);

    switch(roundCounter){
      case 2:
          tokenCounter = tokenCounter - 2;
      break;
      
      case 3:  
          tokenCounter = tokenCounter - 3;
      break;
      
      default:
      break;
    }
    
    break;
    
    case 'x':
    RForward();
    LForward();
    assignSpeed(75, 75);
    delay(timeVar);
    hardStop();
    delay(100);
    
    ToGrey();
    delay(100);
    raiseTokens(retrieveGreyHeight);
    resetCarouselCW();
    lowerTokens();
    resetCarouselCCW();
    delay(500);

    ToGrey();
    delay(100);
    raiseTokens(retrieveHeight);
    resetCarouselCW();
    lowerTokens();
    resetCarouselCCW();
    delay(500);

    ToGrey();
    delay(100);
    raiseTokens(retrieveHeight - 3);
    resetCarouselCW();
    lowerTokens();
    resetCarouselCCW();
    delay(500);
    
    RReverse();
    LReverse();
    assignSpeed(75, 75);
    delay(2*timeVar);
    hardStop();
    delay(100);

    tokenCounter = tokenCounter - 6;
      
    default:
    break;
  
  }
  
  
}

void raiseTokens(int height){
  delay(200);
  myservo.write(height);
  delay(200);
  digitalWrite(magnetPin, LOW);
  delay(500);
  myservo.write(travelHeight);
  delay(500);
}

void turn(int spaces, int dir, int offset)
{
  
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction

  if(tokenCounter > 23){
    offset = offset + 20;
  }
  
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*spaces + offset; x++) 
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}


void printColor(double r, double g, double b, double c)
{
  if(r>10000 && r<15000 && g>1500 && g<5000 && b>700 && b<5700)
  {
    Serial.println("color is red");//R: 0.66 G: 0.20 B: 0.18 color is red
    color = 'r';
    //displayColor();
  }
  else if(r>8000 && r<16500 && g>13000 && g<24000 && b>6000 && b<13500)
  {
    Serial.println("color is green");//R: 0.33 G: 0.55 B: 0.28 color is grey
    color = 'g';
    //displayColor();
    
  }
  else if(r>2500 && r<8500 && g>7500 && g<13000 && b>12000 && b<18500 && c < 45000)
  {
    Serial.println("color is blue");//R: 0.17 G: 0.32 B: 0.51 color is blue
    color = 'b';
    //displayColor();
  }
  else if(r>7000 && r<14500 && g>4000 && g<9000 && b>6000 && b<11000)
  {
    Serial.println("color is Magenta");//R: 15629 G: 10205 B: 13413 NOPE
    color = 'm';
    //displayColor();
  }
  else if(r > 25000 && r < 55000 && g > 22000 && g < 50000 && b > 11000 && b < 30000)
  {
    Serial.println("color is yellow"); //R: 61512 G: 65535 B: 34756 NOPE
    color = 'y';
    //displayColor();
  }
  else if(r>6800 && r<12000 && g>16000 && g<30000 && b>16000 && b<30000 && c>=45000)
  {
    Serial.println("color is cyan"); //R: 0.21 G: 0.55 B: 0.59 color is grey
    color = 'c';
    //displayColor();
  }
  else if(r>3500 && r<6000 && g>4500 && g<7000 && b>3000 && b<6000)
  {
    Serial.println("color is grey");//R: 7462 G: 9922 B: 9211 color is grey
    color = 'x';
//    displayColor();
  }
  else
  {
    Serial.println("NOPE");
    color = 'n';
//    displayColor();
  }
}

////Color Sensor and Carousel ^^^^^^^
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void shiftLeft(){

  MotorL = 150;
  MotorR = 150;
  
  RForward();
  LReverse();
  assignSpeed(MotorR, MotorL);
  delay(300);

  hardStop();
  delay(50);

  RReverse();
  LForward();
  assignSpeed(MotorR, MotorL);
  delay(300);

  hardStop();
  delay(50);

}
