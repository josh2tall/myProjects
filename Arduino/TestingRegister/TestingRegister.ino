//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int buttonPin1 = 10;
int buttonPin2 = 13;
int buttonON1;
int buttonON2;
int button;


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}

void loop() {
    buttonON1 = digitalRead(buttonPin1);
    buttonON2 = digitalRead(buttonPin2);
    
    if(buttonON1 == HIGH)
    {
      button = 2;
    }
    
    if(buttonON2 == HIGH)
    {
      button = 1;
    }
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, button);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
  }

