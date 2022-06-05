#include <Wire.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
   
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


//servo stuff
int maxPWM = 2000;
int minPWM = 1000;
int pos = 0;    // variable to store the servo position
int servoPin = 30;
int servoUp = 173;
int servoDown = 2;
double servoMiddle = 156;
Servo myservo;  // create servo object to control a servo
int buttonPin = 52;
char color;

//magnet
int magnetPin = 7;

void setup(void) {
  tcs.begin();
  Serial.begin(9600);
  //servo stuff
  setServo();
  pinMode(magnetPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  //magnetUp();
  delay(1000);
  digitalWrite(magnetPin, LOW);
  
  }

void loop(void) {

  myservo.write(servoMiddle);

  if(!buttonPin)
  {
    digitalWrite(magnetPin, HIGH);
    delay(500);
    digitalWrite(magnetPin, LOW);
  }
  
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);

  double rd = r;
  double rg = g;
  double rb = b;
  double rc = c;
  Serial.print("R: "); Serial.print(rd); Serial.print(" ");
  Serial.print("G: "); Serial.print(rg); Serial.print(" ");
  Serial.print("B: "); Serial.print(rb); Serial.print(" ");
  Serial.print("c: "); Serial.print(rc); Serial.print(" ");
  printColor(r,g,b,c);
}
void printColor(double r, double g, double b, double c)
{
  if(r>10000 && r<15000 && g>1500 && g<5000 && b>700 && b<5700)
  {
    Serial.println("color is red");//R: 0.66 G: 0.20 B: 0.18 color is red
    color = 'r';
  }
  else if(r>8000 && r<16500 && g>13000 && g<24000 && b>6000 && b<13500)
  {
    Serial.println("color is green");//R: 0.33 G: 0.55 B: 0.28 color is grey
    color = 'g'; 
  }
  else if(r>2500 && r<8500 && g>7500 && g<13000 && b>12000 && b<22000 && c < 47000)
  {
    Serial.println("color is blue");//R: 0.17 G: 0.32 B: 0.51 color is blue
    color = 'b';
  }
  else if(r>8000 && r<14500 && g>4000 && g<10000 && b>6000 && b<11000)
  {
    Serial.println("color is Magenta");//R: 15629 G: 10205 B: 13413 NOPE
    color = 'm';
  }
  else if(r > 25000 && r < 55000 && g > 22000 && g < 50000 && b > 11000 && b < 30000)
  {
    Serial.println("color is yellow"); //R: 61512 G: 65535 B: 34756 NOPE
    color = 'y';
  }
  else if(r>6800 && r<25000 && g>16000 && g<32000 && b>16000 && b<33000 && c>=47000)
  {//changed from r<12000
    Serial.println("color is cyan"); //R: 0.21 G: 0.55 B: 0.59 color is grey
    color = 'c';
  }
  else if(r>3500 && r<8000 && g>4500 && g<7100 && b>3000 && b<6500)
  {
    Serial.println("color is grey");//R: 7462 G: 9922 B: 9211 color is grey
    color = 'x';
  }
  else
  {
    Serial.println("NOPE");
    color = 'n';
  }
}
void setServo()
{
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin, minPWM, maxPWM);
}

void magnetUp()
{
  myservo.write(servoUp);
  delay(500);
}

void magnetDown()
{
  myservo.write(servoDown);
  delay(500);
  digitalWrite(magnetPin, LOW);
  delay(500);
}

void dropToken()
{
  digitalWrite(magnetPin, HIGH);
  delay(500);
}
