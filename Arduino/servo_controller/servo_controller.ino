
#include <Servo.h>

Servo myservo;  // create servo object to control a servo


void setup()
{
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() 
{ 
  /*for(int i = 0; i < 100; i++)
  {
  myservo.write(i);                  // sets the servo position according to the scaled value 
  delay(50);                           // waits for the servo to get there 
  }

  delay(1000);
  
  for(int i = 100; i < 200; i++)
  {
  myservo.write(i);                  // sets the servo position according to the scaled value 
  delay(50);                           // waits for the servo to get there 
  }
*/

//  myservo.write(65);
//  delay(560);
//  myservo.write(75);
//  delay(2000); 
 myservo.write(180);
 delay(122);
myservo.write(90);
  delay(500);




// for(int i = 0; i < 360; i = i + 20)
// {
//  myservo.write(i);
//
//  delay(1000);
//}
  
//  myservo.write(20);
//  
//  delay(1000);
//
//  myservo.write(40);
//
//  delay(1000);
//
//  myservo.write(60);
//
//  delay(1000);
//
//  myservo.write(80);
//  
//  delay(1000);
//
//  myservo.write(100);
//  
//  delay(1000);
//
//  myservo.write(120);
//  
//  delay(1000);
//
//  myservo.write(160);
//  
//  delay(1000);
//
//  myservo.write(180);
//  
//  delay(1000);
//
//   myservo.write(40);
//  
//  delay(1000);
  
} 

