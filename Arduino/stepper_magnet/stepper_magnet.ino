#include <Servo.h>

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

const int magnetPin = 7;
const int servoPin = 30;

const int limitSwitch = 32;

const int fullRev = 3200;//full rev = 200
int steps = 350;
int delayTime = 750;

int servoUp = 137;
int servoDown = 30;
int servoMiddle = 110;

int maxPWM = 2300;
int minPWM = 700;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(limitSwitch,INPUT);  
  
  pinMode(servoPin, OUTPUT);
  pinMode(magnetPin, OUTPUT);
  myservo.attach(servoPin, minPWM, maxPWM);  // attaches the servo on pin 9 to the servo object

  
  magnetUp();
  delay(3000);
  resetCarouselDouble(HIGH);
  
  delay(1000);
  
}
void loop() {


  
magnetDown();
magnetUp();
ToSensor();
ToGrey();
dropToken();
ResetGrey();



 
// *********//
// magnetUp();
// magnetDown();
// magnetUp();
// 
// delay(1000);
// ZeroOne();
// delay(1000);
// OneTwo();
// delay(1000);
// resetCarouselDouble(HIGH);

//**********//

//  
//  ZeroOne();
//  delay(500);
//  OneThree();
//  delay(500);
//  resetCarouselDouble(HIGH);
//  delay(500);
//  
//  ZeroOne();
//  delay(500);
//  OneFour();
//  delay(500);
//  resetCarouselDouble(HIGH);
//  delay(500);
//  
//  ZeroOne();
//  delay(500);
//  OneFive();
//  delay(500);
//  resetCarouselDouble(HIGH);
//  delay(500);
  
//  ZeroOne();
//  delay(500);
//  OneSix();
//  delay(500);
//  resetCarousel(LOW);
//  delay(500);
//  
//  ZeroOne();
//  delay(500);
//  OneSeven();
//  delay(500);
//  resetCarousel(LOW);
//  delay(500);
//  
//  ZeroOne();
//  delay(500);
//  OneEight();
//  delay(500);
//  resetCarousel(LOW);
//  delay(500);

//  delay(1000); // One second delay
//  
//  digitalWrite(dirPin,LOW); //Changes the rotations direction
//  for(int x = 0; x < fullRev; x++) {
//    digitalWrite(stepPin,HIGH);
//    delayMicroseconds(5000);
//    digitalWrite(stepPin,LOW);
//    delayMicroseconds(5000);
//  }
//  delay(1000);

}

void readColor()
{
  ToSensor();
  //color sensor stuff
  //
}

//  positioning carousel
//  resetCarouselDouble(HIGH);
void ToBlue(){turn(1,LOW,0);}
void ToRed(){turn(2,LOW,0);}
void ToGreen(){turn(3,LOW,0);}
void ToCyan(){turn(4,LOW,0);}
void ToSensor(){turn(1,LOW,0);}
void ResetBlue(){resetCarouselDouble(HIGH);};
void ResetRed(){resetCarouselDouble(HIGH);};
void ResetGreen(){resetCarouselDouble(HIGH);};
void ResetCyan(){resetCarouselDouble(HIGH);};

//   positioning carousel
//   resetCarousel(LOW);
void ToMagenta(){turn(4,HIGH,30);}
void ToYellow(){turn(3,HIGH,20);}
void ToGrey(){turn(2,HIGH,20);}
void ResetMagenta(){resetCarousel(LOW);};
void ResetYellow(){resetCarousel(LOW);};
void ResetGrey(){resetCarousel(LOW);};


//   positioning carousel
void ZeroOne(){turn(1,LOW,0);}
void OneZero(){turn(1,HIGH,0);}
void OneTwo(){turn(1,LOW,0);}
void TwoZero(){turn(2,HIGH,0);}
void OneThree(){turn(2,LOW,0);}
void ThreeZero(){turn(3,HIGH,0);}
void OneFour(){turn(3,LOW,0);}
void FourZero(){turn(4,HIGH,0);}
void OneFive(){turn(4,LOW,0);}
void FiveZero(){turn(5,HIGH,0);}
void OneSix(){turn(4,HIGH,30);}
void SixZero(){turn(3,LOW,0);}
void OneSeven(){turn(3,HIGH,20);}
void SevenZero(){turn(2,LOW,0);}
void OneEight(){turn(2,HIGH,20);}
void EightZero(){turn(1,LOW,0);}

void RevolutionClockwise()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*8; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}

void RevolutionCounterClockwise()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*8; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}

void turn(int spaces, int dir, int offset)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*spaces + offset; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
    }
}

void magnetUp()
{
  myservo.write(servoUp);
  delay(1000);
}

void magnetDown()
{
  myservo.write(servoDown);
  delay(1000);
  digitalWrite(magnetPin, HIGH);
  delay(1000);
}

void dropToken()
{
  digitalWrite(magnetPin, LOW);
  delay(1000);
}

//LOW couterclockwise
//HIGH clockwise
void resetCarousel(int dir)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}
void resetCarouselDouble(int dir)
{
  digitalWrite(dirPin,dir); // Enables the motor to move in a particular direction
  while(!digitalRead(limitSwitch))
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
  delay(1000);

  
  if(dir == HIGH){dir = LOW;}
  else if(dir == LOW){dir = HIGH;}
  digitalWrite(dirPin,dir);
  
  while(!digitalRead(limitSwitch))
  {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
  
}


