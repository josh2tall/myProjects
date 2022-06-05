#include <EEPROM.h>

void setup() {
  Serial.begin(9600);

  unsigned int temp;

  EEPROM.get(0,temp);
  Serial.print("Left minimum: ");
  Serial.println(temp);

  EEPROM.get(4,temp);
  Serial.print("Right minimum: ");
  Serial.println(temp);
  
  EEPROM.get(80,temp);
  Serial.print("Back minimum: ");
  Serial.println(temp);
  
  EEPROM.get(8,temp);
  Serial.print("Left maximum: ");
  Serial.println(temp);

  EEPROM.get(12,temp);
  Serial.print("Right maximum: ");
  Serial.println(temp);

  EEPROM.get(84,temp);
  Serial.print("Back maximum: ");
  Serial.println(temp);

  
  Serial.print("qtra minimum: ");

  for(int i = 16; i < 48; i = i + 4)
  {
    EEPROM.get(i,temp);
    Serial.print(temp);
    Serial.print(" ");
  }

  Serial.println();
  Serial.println();
  Serial.print("qtra maximum: ");

  for(int i = 48; i < 80; i = i + 4)
  {
    EEPROM.get(i,temp);
    Serial.print(temp);
    Serial.print(" ");
  }

  
  
  
  Serial.println();
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
