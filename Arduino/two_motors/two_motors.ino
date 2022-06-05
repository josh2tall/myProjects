int motorPin1 = 3;
int motorPin2 = 5;
int sp1 = 0;
int sp2 = 0;

void setup() 
{ 
  pinMode(motorPin1, OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Speed 0 to 255");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPin1, speed);
    }
  }
} 
