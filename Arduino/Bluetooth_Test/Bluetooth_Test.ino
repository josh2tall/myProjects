int anaSense = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // initialize the led pin
  pinMode(13, OUTPUT);
}

void loop() {
  /*for(int x = 0; x< 15; x++){
    if(x = 1){
      t +=1;
      Serial.println("time: ");
      Serial.println(t);
    }*/
    int reading = analogRead(anaSense);
  
  float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

     Serial.println(temperatureF);
     delay(1000);
  }
       
  /*while (Serial.available()) {
    char inChar = (char)Serial.read();
    switch(inChar) {
      case '1':
        digitalWrite(13, HIGH);
      break;
      case '0':
        digitalWrite(13, LOW);
      break;
    }

  }*/

