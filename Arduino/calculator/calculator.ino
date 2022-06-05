#include <LiquidCrystal.h>

bool add_B = false;
bool sub_B = false;
bool multiply_B = false;
bool divide_B = false;
bool cosine_B = false;
bool sine_B = false;
bool power_B = false;
bool factorial_B = false;
bool readInputX_B = true;
bool readInputY_B = false;
bool negativeP = false;
float inputX;
float inputY;
float answer;
int nextNum = 0;
float lastNum = -1;
float const e = 2.7183;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  float sensorValue0 = analogRead(A0);
  float sensorValue1 = analogRead(A1);
  float sensorValue2 = analogRead(A2);
  float sensorValue3 = analogRead(A3);

 
  if(sensorValue0 > 480 && sensorValue0 < 520){ //PI
    if(readInputX_B){
      
        if(inputX != 0)
        inputX *= PI;
        else
        inputX = PI;
        
      lcd.clear();
      lcd.print(inputX);
      delay(1000);
    }
    if(readInputY_B){
      
      if(inputY != 0)
      inputY *= PI;
      else
      inputY = PI;
      
      lcd.clear();
      lcd.print(inputY);
      delay(1000);
    }
    lastNum = PI;
    
  }
  
  if(sensorValue0 > 650 && sensorValue0 < 690){ //e
    if(readInputX_B){
      
        if(inputX != 0)
        inputX *= e;
        else
        inputX = e;
        
      lcd.clear();
      lcd.print(inputX);
      delay(1000);
    }
    if(readInputY_B){
      
      if(inputY != 0)
      inputY *= e;
      else
      inputY = e;
      
      lcd.clear();
      lcd.print(inputY);
      delay(1000);
    }
    lastNum = e;
  }
  if(sensorValue1 > 200 && sensorValue1 < 299){ //clear
   lcd.clear();
   inputX = 0;
   inputY = 0;
   readInputX_B = true;
   readInputY_B = false;
   nextNum = 0;
   add_B = false;
   sub_B = false;
   multiply_B = false;
   divide_B = false;
   cosine_B = false;
   sine_B = false;
   factorial_B = false;
    
  }
  if(sensorValue3 > 970 && sensorValue3 < 1000){ //0
    
    nextNum++;
    delay(1000);
    lastNum = 0;
  }

if(sensorValue2 > 918 && sensorValue2 < 940){ //1
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 1*pow(10,nextNum);
      if(inputX<0)
      inputX -= 1*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 1*pow(10,nextNum);
      if(inputY<0)
      inputY -= 1*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 1;
    nextNum++;
    
  }

  if(sensorValue2 > 880 && sensorValue2 < 915){ //2
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 2*pow(10,nextNum);
      if(inputX<0)
      inputX -= 2*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 2*pow(10,nextNum);
      if(inputY<0)
      inputY -= 2*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 2;
    nextNum++;
    
    
  }
  if(sensorValue2 > 300 && sensorValue2 < 400){ //3
    
   if(readInputX_B){
      if(inputX>=0)
      inputX += 3*pow(10,nextNum);
      if(inputX<0)
      inputX -= 3*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 3*pow(10,nextNum);
      if(inputY<0)
      inputY -= 3*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 3;
    nextNum++;
    
  }
  if(sensorValue1 > 915 && sensorValue1 < 940){ //4
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 4*pow(10,nextNum);
      if(inputX<0)
      inputX -= 4*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 4*pow(10,nextNum);
      if(inputY<0)
      inputY -= 4*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 4;
    nextNum++;
    
  }

if(sensorValue1 > 880 && sensorValue1 < 900){ //5
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 5*pow(10,nextNum);
      if(inputX<0)
      inputX -= 5*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 5*pow(10,nextNum);
      if(inputY<0)
      inputY -= 5*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 5;
    nextNum++;
    
  }

  if(sensorValue1 > 470 && sensorValue1 < 550){ //6
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 6*pow(10,nextNum);
      if(inputX<0)
      inputX -= 6*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 6*pow(10,nextNum);
      if(inputY<0)
      inputY -= 6*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 6;
    nextNum++;
    
    
  }
  if(sensorValue0 > 990 && sensorValue0 < 1030){ //7
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 7*pow(10,nextNum);
      if(inputX<0)
      inputX -= 7*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 7*pow(10,nextNum);
      if(inputY<0)
      inputY -= 7*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 7;
    nextNum++;
  }

if(sensorValue0 > 30 && sensorValue0 < 60){ //8
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 8*pow(10,nextNum);
      if(inputX<0)
      inputX -= 8*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 8*pow(10,nextNum);
      if(inputY<0)
      inputY -= 8*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 8;
    nextNum++;
    
  }

  if(sensorValue0 > 710 && sensorValue0 < 760){ //9
    
    if(readInputX_B){
      if(inputX>=0)
      inputX += 9*pow(10,nextNum);
      if(inputX<0)
      inputX -= 9*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputX);
       
    }
    if(readInputY_B){
      if(inputY>=0)
      inputY += 9*pow(10,nextNum);
      if(inputY<0)
      inputY -= 9*pow(10,nextNum);
      lcd.clear();
      lcd.print(inputY);
      
    }
    delay(500);
    lastNum = 9;
    nextNum++;
    
    
  }

  if(sensorValue0 > 770 && sensorValue0 < 830){ //+

    readInputX_B = false;
    readInputY_B = true;
    add_B = true;
    nextNum = 0;
    lcd.clear();
    delay(1000);
    
  }
  if(sensorValue2 > 470 && sensorValue2 < 550){ //mult

    readInputX_B = false;
    readInputY_B = true;
    multiply_B = true;
    nextNum = 0;
    lcd.clear();
    delay(1000);
    
  }
  if(sensorValue3 > 918 && sensorValue3 < 940){ //sub

    readInputX_B = false;
    readInputY_B = true;
    sub_B = true;
    nextNum = 0;
    lcd.clear();
    delay(1000);
    
  }
  if(sensorValue3 > 880 && sensorValue3 < 915){ //divide

    readInputX_B = false;
    readInputY_B = true;
    divide_B = true;
    nextNum = 0;
    lcd.clear();
    delay(1000);
    
  }
  if(sensorValue1 > 970 && sensorValue1 < 1000){ //sine

      sine_B = true;
      lcd.clear();
      lcd.print("sin ");
      
    
  }

  if(sensorValue1 > 945 && sensorValue1 < 965){ //cosine
    
     cosine_B = true;
     lcd.clear();
     lcd.print("cos ");
    
  }
  if(sensorValue1 >300 && sensorValue1 < 400){//negative
    lcd.clear();
    
    if(readInputX_B){
      inputX *= -1;
      lcd.print(inputX);
    }
    if(readInputY_B){
      inputY *= -1;
      lcd.print(inputY);
    }
    delay(500);

   
  }
  if(sensorValue2 > 945 && sensorValue2 < 965){//pow
    readInputX_B = false;
    readInputY_B = true;
    power_B = true;
    nextNum = 0;
    lcd.clear();
    delay(1000);
  }
  
  if(sensorValue2 > 970 && sensorValue2 < 1000){ //!

    lcd.print("!");
    
    factorial_B = true;
    nextNum = 0;
    delay(500);
    
  }
  


if(sensorValue3 > 470 && sensorValue3 < 515){ //enter
      
      lcd.clear();
      
      if(add_B){
        answer = add(inputX,inputY);
        lcd.print(answer);
        add_B = false;
      }
      if(sine_B){
        answer = sine(inputX);
        lcd.print(answer);
        sine_B = false;
      }
      if(cosine_B){
        answer = cosine(inputX);
        lcd.print(answer);
        cosine_B = false;
      }
      if(sub_B){
        answer = subtract(inputX, inputY);
        lcd.print(answer);
        sub_B = false;

      }
      if(multiply_B){
        answer = multiply(inputX, inputY);
        lcd.print(answer);
        multiply_B = false;
      }
      if(divide_B){
        answer = divide(inputX, inputY);
        lcd.print(answer);
        divide_B = false;
      }
      if(factorial_B){
        answer = factorial(inputX);
        lcd.print(answer);
        factorial_B = false;
      }
      if(power_B){
        answer = pow(inputX,inputY);
        lcd.print(answer);
        power_B = false;
        
      }
        inputX = answer;
        inputY = 0;
        readInputX_B = false;
        readInputY_B = true;
                delay(1000);
  }

  
}





float readInputX(){
  while(readInputX_B = true){
    
  }
}

float readInputY(){
  
}


float add(float x, float y){

  answer = x + y;
  return answer;
}

float multiply(float x, float y){

  answer = x * y;
  return answer;
}

float subtract(float x, float y){

  answer = x - y;
  return answer;
}

float divide(float x, float y){

  answer = x / y;
  return answer;
}

float factorial(int factorialBase){
  answer = 1;
  while(factorialBase > 0){
  answer*= factorialBase;
  factorialBase--;

  }

  return answer;
}

float power(float base, float power){

  if(power = 0){
    answer = 1;
  }
  
  if( power < 0){
    power *=-1;
    negativeP = true;
  }
  
  while(power > 0){
    answer *= base;
    power--;
  }
  
  if(negativeP){
    answer = 1 / answer; 
  }
  return answer;
}

float sine(float x){
  
answer = sin(x);

  return answer;
}

float cosine(float x){
  answer = cos(x); 
  return answer;
}

float squareRoot(float x){
  answer = sqrt(x);
  return answer;
}

float currentInput(){
  if(readInputX_B)
    return inputX;
  if(readInputY_B)
    return inputY;
}






