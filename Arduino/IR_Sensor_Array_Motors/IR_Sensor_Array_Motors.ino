#include <QTRSensors.h>

// This example is designed for use with six QTR-1A sensors or the first six sensors of a
// QTR-8A module.  These reflectance sensors should be connected to analog inputs 0 to 5.
// The QTR-8A's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
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
// (analog voltage readings from 0 to 1023) by calling qtra.read(sensorValues) instead of
// qtra.readLine(sensorValues).

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance) 
// to 1000 (minimum reflectance) followed by the estimated location of the line as a number
// from 0 to 5000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.
// connect motor controller pins to Arduino digital pins

// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
int motorSpeedRight = 200;

// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;
int motorSpeedLeft = 255;

#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2,3,4,5,6,7}, 
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtra.read(sensorValues); instead of unsigned int position = qtra.readLine(sensorValues);
  unsigned int position = qtra.readLine(sensorValues);
  unsigned int error = position - 1000;
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    //Serial.print(sensorValues[i]);
    //Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
  delay(250);

  if(position >= 6500)
  {
    motorSpeedLeft += 20;
    motorSpeedRight -= 20;
  }
  else if(position >= 5500)
  {
    motorSpeedLeft += 15;
    motorSpeedRight -= 15;
  }
  else if(position >= 4500)
  {
    motorSpeedLeft += 10;
    motorSpeedRight -= 10;
  }
  else if(position >= 3500)
  {
    motorSpeedLeft += 5;
    motorSpeedRight -= 5;
  }
  else if(position < 3500)
  {
    motorSpeedLeft -= 5;
    motorSpeedRight += 5;
  }
  else if(position <= 2500)
  {
    motorSpeedLeft -= 10;
    motorSpeedRight += 10;
  }
  else if(position <= 1500)
  {
    motorSpeedLeft -= 15;
    motorSpeedRight += 15;
  }
  else if(position <= 500)
  {
    motorSpeedLeft -= 20;
    motorSpeedRight += 20;
  }
  // this function will run the motors in both directions at a fixed speed
  // turn on right motor 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, motorSpeedRight);
  
  // turn on left motor
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, MotorSpeedLeft);
  delay(500);

  /*
  // turn off motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // turn off motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2000);*/
  
}
