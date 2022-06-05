int switchState = 0;
int greenLED = 3;
int yellowLED = 4;
int redLED = 5;
void setup() {

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(2, INPUT);

  
}

void loop() {

  switchState = digitalRead(2);

  if (switchState == LOW){
    digitalWrite(greenLED,LOW);
    digitalWrite(redLED,HIGH);
    digitalWrite(yellowLED,LOW);
    delay(250);
    digitalWrite(greenLED,LOW);
    digitalWrite(yellowLED,HIGH);
    digitalWrite(redLED, LOW);
    delay(250);
  }
  else{
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    delay(250);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(250);
  }
  
}
