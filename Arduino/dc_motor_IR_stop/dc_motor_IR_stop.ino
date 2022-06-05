#include <QTRSensors.h>
#include <elapsedMillis.h>


// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

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

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 100;


int motorSpeedLeft = 200;
int motorSpeedRight = 220;

int originalLeft = motorSpeedLeft;
int originalRight = motorSpeedRight;

int counter = 0;
void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

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
    //qtra.calibrate();
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
  Left.read(LsensorValue);
  Right.read(RsensorValue);
  
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
  analogWrite(enB, motorSpeedLeft);

  if(LsensorValue[0] >= 1500 && counter == 0 && timer0 > interval){
    motorSpeedRight = originalRight - 60;
    motorSpeedLeft = originalLeft - 50;
    counter++;

    timer0 = 0;
  }
  
  else if(LsensorValue[0] >= 1500 && counter == 1 && timer0 > interval){
    counter =0;
     hardStop();

     delay(5000);

      motorSpeedRight = originalRight;
      motorSpeedLeft = originalLeft;
      
     timer0 = 0;
  }
   

  /* turn off motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // turn off motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2000);*/
  /*
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  delay(2000);
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);
  */
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

