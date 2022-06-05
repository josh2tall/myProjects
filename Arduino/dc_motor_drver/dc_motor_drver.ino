// connect motor controller pins to Arduino digital pins
// motor one
int enR = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enL = 5;
int in3 = 7;
int in4 = 6;

void setup()
{
  // set all the motor control pins to outputs
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
void loop()
{
  delay(1000);
  // this function will run the motors in both directions at a fixed speed
  // turn on right motor 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enR, 75);
  analogWrite(enL, 75);
  delay(2000);

  // turn off motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // turn off motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(20000);
 
}

