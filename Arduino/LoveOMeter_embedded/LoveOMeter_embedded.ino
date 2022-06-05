const int sensorPin = A0;
const float baselineTemp = 20.0;

void setup() {
  Serial.begin(9600);
  
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
}

void loop() {
  int sensorVal = analogRead(sensorPin);

  Serial.print("sensor Value: ");
  Serial.print(sensorVal);

  float voltage = (sensorVal / 1024.0) * 5.0;

  Serial.print(", Volts: ");
  Serial.print(voltage);

  Serial.print(", degrees C: ");
  float temperature = (voltage - .5) * 100;
  Serial.print(temperature);

  Serial.print(", degrees F: ");
  float Ftemperature = temperature*1.8+32;
  Serial.println(Ftemperature);
  
  if (temperature < baselineTemp + 3) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

  } 
  else if (temperature >= baselineTemp + 3 && temperature < baselineTemp + 6) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  } 
  else if (temperature >= baselineTemp + 6 && temperature < baselineTemp + 9) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  } 
  else if (temperature >= baselineTemp + 9 && temperature < baselineTemp + 12) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
  else if (temperature >= baselineTemp + 12 && temperature < baselineTemp + 15) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  
  delay(1);
}
