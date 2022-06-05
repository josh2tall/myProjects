#include <elapsedMillis.h>
#include <SoftwareSerial.h>

const int RX_PIN = 10;
const int TX_PIN = 11;
const int ledPin = 13;
const int BLUETOOTH_BAUD_RATE = 9600;

char state = 'g';

SoftwareSerial bluetooth(RX_PIN, TX_PIN);

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 3;
int in1 = 5;
int in2 = 4;
// motor two
int enB = 9;
int in3 = 6;
int in4 = 7;

elapsedMillis timer0;

int speed0 = 100;
bool slowdown = 0;

void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  bluetooth.begin(BLUETOOTH_BAUD_RATE);
  Serial.begin(9600);
}
void loop()
{
  if(bluetooth.available() > 0)
  { 
    // Checks whether data is comming from the serial port
    state = bluetooth.read(); // Reads the data from the serial port
  }
  
  if (state == 'w')//forward
  {
    // turn on left motor
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // set speed to 200 out of possible range 0~255
    analogWrite(enB, 200);
  
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // set speed to 200 out of possible range 0~255
    analogWrite(enA, 200);
    
    delay(500);
  }
  else if (state == 's')//backwards
  {
    // turn on left motor
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // set speed to 200 out of possible range 0~255
    analogWrite(enB, 200);
  
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // set speed to 200 out of possible range 0~255
    analogWrite(enA, 200);
    
    delay(500);
  }
  else if (state == 'a')//left
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // set speed to 200 out of possible range 0~255
    analogWrite(enA, 100);
    
    delay(500);
  }
  else if (state == 'd')//right
  {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // set speed to 200 out of possible range 0~255
    analogWrite(enA, 100);
    
    delay(500);
  }
  

  // turn off motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

   // turn off motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  state = 'g';
}
