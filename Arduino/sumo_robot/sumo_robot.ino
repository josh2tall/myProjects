#include <SoftwareSerial.h>

const int RX_PIN = 10;
const int TX_PIN = 3;
const int BLUETOOTH_BAUD_RATE = 9600;

SoftwareSerial bluetooth(RX_PIN, TX_PIN);

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 9;
int in1 = 11;
int in2 = 12;
// motor two
int enB = 5;
int in3 = 13;
int in4 = 8;

int speed1 = 255;
int speed2 = 255;

//SoftwareSerial mySerial(0, 1); // RX, TX
char val; // variable to receive data from the serial port
int ledpin = 2; // LED connected to pin 48 (on-board LED)
char val1;
void setup()
{

  

  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(ledpin, OUTPUT);  // pin 48 (on-board LED) as OUTPUT
  Serial.begin(9600);       // start serial communication at 9600bps
  bluetooth.begin(BLUETOOTH_BAUD_RATE);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, speed1);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, speed2);
}

void loop()
{
    if (bluetooth.available()) {
      val1 = bluetooth.read();
      Serial.print(val1);
    }

    
  if(val1 == '0')
  {
    digitalWrite(in1, HIGH);//forward
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
      speed1 = 255;
      speed2 = 255;
  }
  else if(val1 == '1')
  {
  digitalWrite(in1, LOW);//back
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  speed1 = 255;
  speed2 = 255;
  }
  else if(val1 == '2')
  {
    digitalWrite(in1, HIGH);//left
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
      speed1 = 0;
      speed2 = 255;
  }
  else if(val1 == '3')
  {
    digitalWrite(in1, HIGH);//right
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
      speed1 = 255;
      speed2 = 0;
  }
  else
  {
  digitalWrite(in1, LOW);//stop
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  speed1 = 255;
  speed2 = 255;
  }


   analogWrite(enA, speed1);
   analogWrite(enB, speed2);

}
