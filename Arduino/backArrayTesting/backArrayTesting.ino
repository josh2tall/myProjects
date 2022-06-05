#include <QTRSensors.h>
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

int onoff = false;

//Left Sensor
QTRSensorsRC Left((unsigned char[]) {51}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int LsensorValue[NUM_SENSORS_D];


//Right Sensor
QTRSensorsRC Right((unsigned char[]) {50}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int RsensorValue[NUM_SENSORS_D];

//Back Sensor
QTRSensorsRC MiddleLeft((unsigned char[]) {48}, NUM_SENSORS_D, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int MLsensorValue[NUM_SENSORS_D];

//Sensor Array
QTRSensorsAnalog qtraFront((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int frontArraySensorValues[NUM_SENSORS_A];

//Back Sensor Array
QTRSensorsRC qtraBack((unsigned char[]) {29,28,27,26,25,24,23,22},
  NUM_SENSORS_A, TIMEOUT, QTR_NO_EMITTER_PIN); 
  unsigned int backArraySensorValues[NUM_SENSORS_A];

  
void setup()
{
    

  Serial.begin(9600);
  delay(1000);

  Left.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  Right.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  MiddleLeft.calibrate();
  qtraFront.calibrate();
  qtraBack.calibrate();
  Serial.println("DONE");

  setCalibration();
}


void loop()
{

     // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtraBack.readLine(backArraySensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS_A; i++)
  {
    Serial.print(backArraySensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
  
  delay(250);

}

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
  MiddleLeft.calibratedMinimumOn[0] = temp;

  EEPROM.get(148,temp);
  MiddleLeft.calibratedMaximumOn[0] = temp;
}
