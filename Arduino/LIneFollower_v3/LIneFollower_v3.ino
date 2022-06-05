#include <QTRSensors.h>
#include <elapsedMillis.h>


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
QTRSensorsRC Left((unsigned char[]) {22}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int LsensorValue[NUM_SENSORS_D];


//Right Sensor
QTRSensorsRC Right((unsigned char[]) {23}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int RsensorValue[NUM_SENSORS_D];

QTRSensorsAnalog qtra((unsigned char[]) {7, 6, 5, 4, 3, 2, 1, 0}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int arraySensorValues[NUM_SENSORS_A];



// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

int MotorA = 110;
int MotorB = 110;
int baseSpeed = 110;
int lastError = 0;

int counter = 0;

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 250;

//interval before the front IR sensors detect lines in ms
int intervalAfter = 5000;
bool frontReady = false;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;
double kp = .1;
double kd = .1;
double ki = .0001;


  
void setup()
{
    
 // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  Serial.begin(9600);
  Serial.println("Calibrating...");
  delay(500);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    Left.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    Right.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    qtra.calibrate();
  }
  
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on

  Serial.print("Calibration Min: ");
  Serial.print(Left.calibratedMinimumOn[0]);
  Serial.print(' ');
  Serial.print(Right.calibratedMinimumOn[0]);
  Serial.print(' ');
  
  Serial.println();

  //print the calibration maximum values measured when emitters were on
  Serial.print("Calibration Max: ");
  Serial.print(Left.calibratedMaximumOn[0]);
  Serial.print(' ');
  Serial.print(Right.calibratedMaximumOn[0]);
  Serial.print(' ');
  
  Serial.println();
  Serial.println();


}


void loop()
{

  //assign a speed for each motor
  analogWrite(enA, MotorA);
  analogWrite(enB, MotorB);

  //read the front IR sensor values
  Left.read(LsensorValue);
  Right.read(RsensorValue);

  //front IR sensors wont detect lines for a duration of time
  if(timer0 > intervalAfter)
  {
    frontReady = true;
  }

  //front left IR sensor counting lines crossed
  if(LsensorValue[0] >= 1500 && timer0 > interval && frontReady){
    counter++;
    timer0 = 0;
  }

  
  if(counter == 2){

    //stop on line
    hardStop();
    delay(1000);

    //reset counter
    counter = 0;

    // robot will turn 135 degrees to the left
    turnLeft135();

    //reset timer
    timer0 = 0;
    frontReady = false;
  }


  //calculate the error depending on which IR sensor from the array are above a line
  int position = qtra.readLine(arraySensorValues);
  int error = position-3500;
  
  //calculate PID values
  proportional = kp * error;
  integral = ki * (integral + proportional);
  derivative = kd * (error-lastError);
  
  
  double motorAdjust = proportional + integral + derivative;

  lastError = error; 

  //change motorA speed depending on PID values
  MotorA = baseSpeed + motorAdjust;

  //ensure the motor doesnt go passed limits
  if(MotorA < 0)
  MotorA = 0;
  
  if(MotorA > 255)
  MotorA = 255;

  
}

//--------------------------------------------------------Function Definitions---------------------------------------------------//

// This function will run the motors in both directions at a fixed speed
void MotorOn()
{
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, MotorA);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, MotorB);
}

//Function to make the robot move left/right as it calibrates
void calibrationMovement(){

  for(int i = 0; i < 5; i++){
      // turn on motor A
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, MotorA);
    
    // turn on motor B
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, MotorB);

    for(int j = 0; j < 25; j++){
      qtra.calibrate();
    }

    // turn on motor A
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, MotorA);
    
    // turn on motor B
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, MotorB);

    for(int j = 0; j < 25; j++){
      qtra.calibrate();
    }
  }
}

void turnLeft135()
{

  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

   // turn on motor B in the opposite direction of motor A
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  //store original speed of Motor B
  int temp0 = MotorB;

  //turn left 135 degrees
  MotorA = 200;
  MotorB = 200;
  analogWrite(enA, MotorA);
  analogWrite(enB, MotorB);
  delay(500);

  //assign each motor the original value of motor B
  MotorA = temp0;
  MotorB = temp0;

  //turn on motorA
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

   // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void hardStop()
{
     // turn OFF motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enA, 0);
  // turn OFF motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  // set speed to 0 out of possible range 0~255
  analogWrite(enB, 0);

  
}

