class StepperMotor {
public:
    StepperMotor(int In1, int In2, int In3, int In4);    // Constructor that will set the inputs
    void setStepDuration(int duration);    // Function used to set the step duration in ms
    void step(int noOfSteps);    // Step a certain number of steps. + for one way and - for the other

    int duration;    // Step duration in ms
    int inputPins[4];    // The input pin numbers
};

StepperMotor::StepperMotor(int In1, int In2, int In3, int In4){
    // Record pin numbers in the inputPins array
    this->inputPins[0] = In1;
    this->inputPins[1] = In2;
    this->inputPins[2] = In3;
    this->inputPins[3] = In4;

    // Iterate through the inputPins array, setting each one to output mode
    for(int inputCount = 0; inputCount < 4; inputCount++){
        pinMode(this->inputPins[inputCount], OUTPUT);
    }
    duration = 50;
}

void StepperMotor::setStepDuration(int duration){
    this->duration = duration;
}

void StepperMotor::step(int noOfSteps){
    /*
        The following 2D array represents the sequence that must be
        used to acheive rotation. The rows correspond to each step, and
        the columns correspond to each input. L
    */
    bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
                          {LOW, LOW, HIGH, HIGH},
                          {LOW, LOW, HIGH, LOW },
                          {LOW, HIGH, HIGH, LOW},
                          {LOW, HIGH, LOW, LOW },
                          {HIGH, HIGH, LOW, LOW},
                          {HIGH, LOW, LOW, LOW },
                          {HIGH, LOW, LOW, HIGH}};
                      
    int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1 
    noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

    /* 
        The following algorithm runs through the sequence the specified number 
        of times
    */
    for(int sequenceNum = 0;  sequenceNum <= noOfSteps/8; sequenceNum++){
        for(int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum*8 )); position++){
            delay(duration);
            for(int inputCount = 0; inputCount < 4; inputCount++){
                digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5*factor) + (factor*position))][inputCount]);
            }
        } 
    }
}


StepperMotor motorBase(8,9,10,11);
StepperMotor motorLeftUp(4,5,6,7);
StepperMotor motorRightForward(2,3,12,13);

void setup(){
  Serial.begin(9600);
  motorBase.setStepDuration(1);
  motorLeftUp.setStepDuration(1);
  motorRightForward.setStepDuration(1);
  //pinMode(12, INPUT);

  
}

void loop(){
  
   /* bool start = 0;

  while(!start)
  {
    start = digitalRead(12);
  }
  */

  /*
  delay(100);
  motorBase.step(-400);
  delay(1000);
  motorBase.step(400);
  delay(1000);
  

  motorLeftUp.step(300);
  delay(1000);
  motorLeftUp.step(-300);
  delay(1000);
  
  
  motorRightForward.step(300);
  delay(1000);
  motorRightForward.step(-300);
  delay(1000);

*/

  int steps = 400;
  int delayT = 500;
  //move from starting position to ground and back 
  //to starting position
  //400 steps good
  
    delay(delayT);
  motorBase.step(-600);
  
  motorRightForward.step(steps);
  delay(delayT);
  motorLeftUp.step(-steps);
  delay(delayT);
  motorRightForward.step(steps);
  delay(delayT);
  motorRightForward.step(-steps);
  delay(delayT);
  motorLeftUp.step(steps);
  delay(delayT);
  motorRightForward.step(-steps);
  delay(delayT);

  motorBase.step(600);
  delay(delayT);
}



