int LED = 2;
int LED1 = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  delay(500);
  digitalWrite(LED1,HIGH);
  delay(500);
  digitalWrite(LED1,LOW);
  delay(500);
}
