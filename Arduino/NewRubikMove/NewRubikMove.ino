/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val = 1;    // variable to read the value from the analog pin
int movement = 20;
void setup()
{
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
}

void loop() 
{ 
  /*if (val == 180 || val == 0)
  {
    movement *=-1;
  }*/
  
  //val += movement;
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(100);                           // waits for the servo to get there 
} 

