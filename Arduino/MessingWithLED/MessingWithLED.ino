#include <LiquidCrystal.h>

int LED = 12;
int BUTTON = 9;
int anaSense = 0;

LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);

  lcd.begin(16,2);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  lcd.setCursor(0,1);
  int reading = analogRead(anaSense);
  
  if(digitalRead(BUTTON) == HIGH)
  {
  digitalWrite(LED, HIGH);
  
  float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // print out the voltage
 Serial.print(voltage);
 Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 lcd.print(temperatureF);
 lcd.println(" degrees F");
 delay(1000);
  }
  else
  {
  digitalWrite(LED,LOW);
  }
  
}
