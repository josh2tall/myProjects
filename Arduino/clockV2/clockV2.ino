#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sensorPin = 0;
int hour = 7;
int minute1 = 3;
int minute0 = 1;
int second = 0;

void setup() {
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // Print a message to the LCD.
  Serial.begin(9600);

}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);

  //what the lcd shows
  Serial.println(second);
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute1);
  lcd.print(minute0);
  delay(1000);//accounting for time to complete each loop
  lcd.clear();
  second++;

  //main clock
  if (second == 60) {
    second = 0;
    minute0++;
  }
  if (minute0 == 10) {
    minute0 = 0;
    minute1++;
  }
  if (minute1 == 6) {
    minute1 = 0;
    hour++;
  }
  if (hour == 13 ) {
    hour = 1;
  } 
  
  lcd.setCursor(0, 1);
  
  }

