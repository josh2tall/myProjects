int interruptButton = 2;
int LED = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptButton), STOP, FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:

  while(true)
  {
    digitalWrite(LED, LOW);
  }
}

void STOP()
{
  while(true)
    digitalWrite(LED, HIGH);
}

