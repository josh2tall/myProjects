#include <QTRSensors.h>
#include <elapsedMillis.h>


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
int enB = 3;
int in3 = 7;
int in4 = 6;

int combSpeed = 70;
int MotorA = combSpeed;
int MotorB = combSpeed;
int baseSpeed = combSpeed;
int lastError = 0;

int counter = 0;

//create a timer
elapsedMillis timer0;

//interval between front IR sensor readings in ms
int interval = 100;

//interval before the front IR sensors detect lines in ms
int intervalAfter = 1000;
bool frontReady = false;

//Initializing PID values
//kp = kd and ki >= .1kp
int proportional = 0;
int derivative = 0;
int integral = 0;
double kp = .1;
double kd = .1;
double ki = .0001;

int start = 0;

  
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


   setPwmFrequency(10,8);
   setPwmFrequency(3,8);

   

}


void loop()
{

  //assign a speed for each motor
  analogWrite(enA, MotorB);
  analogWrite(enB, MotorA);

  //read the front IR sensor values
  Left.read(LsensorValue);
  Right.read(RsensorValue);

//  if(start == 0)
//  {
//     analogWrite(enA, 120);
//     analogWrite(enB, 120);
//
//     delay(200);
//
//     start++;
//  }
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
    //hardStop();
    //delay(1000);

    //reset counter
    counter = 0;

    // robot will turn 135 degrees to the left
    //turnLeft135();

    //reset timer
    timer0 = 0;
    frontReady = false;


  }


  //calculate the error depending on which IR sensor from 
  //the array are above a line
  int position = qtra.readLine(arraySensorValues);
  int error = position-3500;
  Serial.println(position);
  
  //calculate PID values
  proportional = kp * error;
  integral = ki * (integral + error);
  derivative = kd * (error-lastError);
  
  
  double motorAdjust = proportional + derivative + integral;

  lastError = error; 

  //change motorA speed depending on PID values
  MotorA = baseSpeed + motorAdjust;

  //ensure the motor doesnt go passed limits
  if(MotorA < 0)
  MotorA = 0;
  
  if(MotorA > 150)
  MotorA = 150;

  
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
  delay(1000);

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

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

