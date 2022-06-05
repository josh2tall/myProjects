
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

const int fullRev = 3200;//full rev = 200
int steps = 340;
int delayTime = 750;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(2,OUTPUT); 

  digitalWrite(2,HIGH);
  delay(1000);
  
}
void loop() {

  //RevolutionClockwise();

  //delay(1000);

  //RevolutionCounterClockwise();

  //delay(1000);
//  //one is color sensor
  //zeroOne();

  //delay(1000);

  //OneZero();

  //delay(1000);
//  OneTwo();
//
  zeroOne();
  OneThree();
//
//  zeroOne();
//  OneFour();

//  OneTwo();//bin 8
//  
//  zeroOne();//color sensor
//  OneSeven();//bin 7
//
//  zeroOne();//color sensor
//  OneSix();//bin 6
//  
//  delay(1000); // One second delay
//  
//  digitalWrite(dirPin,LOW); //Changes the rotations direction
//  for(int x = 0; x < fullRev; x++) {
//    digitalWrite(stepPin,HIGH);
//    delayMicroseconds(5000);
//    digitalWrite(stepPin,LOW);
//    delayMicroseconds(5000);
//  }
//  delay(1000);

}


void zeroOne()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(1000);

}

void OneZero()
{
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(1000);
}

void OneTwo()
{
  int steps = 22;

  //one to two
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(1000);

  //two to zero
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*2; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(1000);

}

void OneThree()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*2; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }

  delay(1000);

  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*3; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
    }

    delay(1000);
}


void OneFour()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*3; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }

  delay(1000);

  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*4; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
    }

    delay(1000);
}

void OneFive()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*4; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
  }

  delay(1000);

  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*5; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
    }

    delay(1000);

}

void OneSix()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*4; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
  }

  delay(1000);

  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*3; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
    }

    delay(1000);

}

void OneSeven()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*3; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
  }

  delay(1000);

  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*2; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
    }

    delay(1000);

}

void OneEight()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*2; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
  }

  delay(1000);

  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1000); 
    }

    delay(1000);

}

void RevolutionClockwise()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*8; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}

void RevolutionCounterClockwise()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps*8; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(delayTime); 
  }
}

