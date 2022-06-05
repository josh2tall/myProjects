int clock = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(clock,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(clock,HIGH);
  delay(5000);
  digitalWrite(clock,LOW);
  delay(5000);
}
