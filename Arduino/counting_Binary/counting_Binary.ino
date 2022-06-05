const int LED0 = 12;
const int LED1 = 10;
const int LED2 = 8;
const int LED3 = 3;
const int LED4 = 2;
const int LEDT = 13;

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  
}

void loop() {
  reset();
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  
  
  digitalWrite(LED1,LOW);
  delay(500);
  reset();


  digitalWrite(LED0,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();

  digitalWrite(LED2,LOW);
  delay(500);
  reset();

  digitalWrite(LED2,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();

  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0, LOW);
  delay(500);
  reset();
  
  digitalWrite(LED3,LOW);
  delay(500);
  reset();

  digitalWrite(LED3, LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED3,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();
  
  digitalWrite(LED3,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  delay(500);
  reset();

  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();
  
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  delay(500);
  reset();
  
  digitalWrite(LED4,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();
  
  digitalWrite(LED4,LOW);
  digitalWrite(LED0,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED2,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0, LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED0,LOW);
  delay(500);
  reset();

  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  delay(500);
  reset();
  
  digitalWrite(LED4,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED0,LOW);
  delay(1000);
  reset();


  }

void reset(){
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(500);
}

