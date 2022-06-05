bool index = false;
bool middle = false;
bool ring = false;
bool pinky = false;

int indexLED = 12;
int middleLED = 13;
int ringLED = 11;
int pinkyLED = 10;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(OUTPUT,indexLED);
  pinMode(OUTPUT,middleLED);
  pinMode(OUTPUT,ringLED);
  pinMode(OUTPUT,pinkyLED);
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float sensorValue0 = analogRead(A0);
  Serial.println(sensorValue0);
  getFinger(sensorValue0);
  if(index){
    toggleIt();
  }
  else if(middle)
  { 
    blinkFaster();
  }
  else if(ring)
  {
    flow();
  }
  else if(pinky)
  {
    pinkyToIndex();
  }
  index = false;
  middle = false;
  ring = false;
  pinky = false;

  resetLow();
  delay(200);
}

void getFinger(int sensorValue0)
{
  //thumb = 0
  //index = 500-550
  if(sensorValue0 > 500 && sensorValue0 < 550)
  {
    index = true;
  }
  //middle = 105-110
  else if(sensorValue0 > 105 && sensorValue0 < 110)
    {
      middle = true;
    }
  ///ring = 95 - 104
  else if(sensorValue0 > 205 && sensorValue0 < 220)
    {
      ring = true;
    }
  //pinky = 170- 185
  else if(sensorValue0 > 335 && sensorValue0 < 350)
    {
      pinky = true;
    }
    
}

void indexToPinky()
{
  digitalWrite(indexLED, HIGH);
  delay(200);
  digitalWrite(middleLED, HIGH);
  delay(200);
  digitalWrite(ringLED, HIGH);
  delay(200);
  digitalWrite(pinkyLED, HIGH);
  delay(200);
}

void blinkFaster()
{


    digitalWrite(indexLED, HIGH);
    digitalWrite(middleLED, HIGH);
    digitalWrite(ringLED, HIGH);
    digitalWrite(pinkyLED, HIGH);
    delay(200);
}

void flow()
{
  digitalWrite(indexLED, HIGH);
  delay(200);
  digitalWrite(middleLED, HIGH);
  delay(200);
  digitalWrite(ringLED, HIGH);
  delay(200);
  digitalWrite(pinkyLED, HIGH);
  delay(200);

  digitalWrite(pinkyLED, LOW);
  delay(200);
  digitalWrite(ringLED, LOW);
  delay(200);
  digitalWrite(middleLED, LOW);
  delay(200);
  digitalWrite(indexLED, LOW);
  delay(200);
}

void pinkyToIndex()
{
 for(int i = 0; i < 4; i++){
  digitalWrite(indexLED, HIGH);
  digitalWrite(pinkyLED, HIGH);
  delay(200);
  resetLow();
  digitalWrite(middleLED, HIGH);
  digitalWrite(ringLED, HIGH);
  delay(200);
  resetLow();
  }
}

void resetLow()
{
  digitalWrite(indexLED, LOW); 
  digitalWrite(middleLED, LOW); 
  digitalWrite(ringLED, LOW); 
  digitalWrite(pinkyLED, LOW); 
}

void toggleIt()
{
  digitalWrite(pinkyLED, HIGH);
  digitalWrite(middleLED, HIGH);
   digitalWrite(ringLED, LOW);
  digitalWrite(indexLED, LOW);
  delay(200);
  digitalWrite(pinkyLED, LOW);
  digitalWrite(middleLED,LOW);
  digitalWrite(ringLED, HIGH);
  digitalWrite(indexLED, HIGH);
  delay(200);
}

