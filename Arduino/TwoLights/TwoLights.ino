


const int buzzerPin = 9;
const int redLED = 12;
const int greenLED = 13;
const int buttonPinL = 8;
const int buttonPinR = 9;
int buttonState0 = 0;
int buttonState1 = 0;

void setup() {
  // put your setup code here, to run once:
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(buttonPinL, INPUT);
    pinMode(buttonPinR, INPUT);
}

void loop() {
buttonState0 = digitalRead(buttonPinL);
buttonState1 = digitalRead(buttonPinR);

  //Red light
  if(buttonState0 == HIGH)
  {
    digitalWrite(redLED, HIGH);
  }
  else
  {
    digitalWrite(redLED, LOW);
  }

  //Green light
   if(buttonState1 == HIGH)
  {
    digitalWrite(greenLED, HIGH);
  }
  else
  {
    digitalWrite(greenLED, LOW);
  }

  
}
