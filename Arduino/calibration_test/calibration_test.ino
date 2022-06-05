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
QTRSensorsAnalog qtra((unsigned char[]) {7, 6, 5, 4, 3, 2, 1, 0}, 
  NUM_SENSORS_A, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN);
  unsigned int arraySensorValues[NUM_SENSORS_A];

void setup()
{
    

  Serial.begin(9600);
  delay(1000);

  Serial.println("Calibrating...");
  
     digitalWrite(13, HIGH);     // turn off Arduino's LED to indicate we are through with calibration

    for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
    {
      Left.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
      Right.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
      Back.calibrate();
      qtra.calibrate();
    }
        digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  
    // print the calibration minimum values measured when emitters were on
  
    Serial.print("Calibration Min: ");
    Serial.print(Left.calibratedMinimumOn[0]);
    Serial.print(' ');
    Serial.print(Right.calibratedMinimumOn[0]);
    Serial.print(' ');
    Serial.print(Back.calibratedMinimumOn[0]);
    Serial.print(' ');
    
    for(int i = 0; i < 8; i++)
    {
       Serial.print(qtra.calibratedMinimumOn[i]); 
       Serial.print(' ');
    }
      Serial.println();
    
      //print the calibration maximum values measured when emitters were on
      Serial.print("Calibration Max: ");
      Serial.print(Left.calibratedMaximumOn[0]);
      Serial.print(' ');
      Serial.print(Right.calibratedMaximumOn[0]);
      Serial.print(' ');
      Serial.print(Back.calibratedMaximumOn[0]);
      Serial.print(' ');
      
    for(int i = 0; i < 8; i++)
    {
      Serial.print(qtra.calibratedMaximumOn[i]); 
      Serial.print(' ');
    }
      
      Serial.println();
      Serial.println();
  
        //store the minimum calibrated values for Left
    //and right IR sensors
    EEPROM.put(0,Left.calibratedMinimumOn[0]);
    EEPROM.put(4,Right.calibratedMinimumOn[0]);

    //store the maximum calibrated values for Left
    //and right IR sensors
    EEPROM.put(8,Left.calibratedMaximumOn[0]);
    EEPROM.put(12,Right.calibratedMaximumOn[0]);
  
    //store the minimum calibrated values for IR sensor Array
    int i = 16;
    int j = 0;
    while(i < 48)
    {
      EEPROM.put(i,qtra.calibratedMinimumOn[j]);
      i = i + 4;
      j++;
    }
  
    //store the maximum calibrated values for IR sensor Array
    j = 0;
    while(i < 80)
    {
      EEPROM.put(i,qtra.calibratedMaximumOn[j]);
      i = i + 4;
      j++;
    }

    EEPROM.put(80,Back.calibratedMinimumOn[0]);
    EEPROM.put(84,Back.calibratedMaximumOn[0]);

  


}


void loop()
{

}


