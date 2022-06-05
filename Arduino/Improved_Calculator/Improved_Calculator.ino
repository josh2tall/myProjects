#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS     10
#define TFT_RST    9               
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!

int Q0 = 2;
int Q1 = 3;
int Q2 = 4;
int Q3 = 5;
int Q4 = 6;
int Q0State;
int Q1State;
int Q2State;
int Q3State;
int Q4State;
bool D0 = false; //esc
bool D1 = false; //7
bool D2 = false; //8
bool D3 = false; //9
bool D4 = false; // add
bool D5 = false; // subtract
bool D6 = false; // trig
bool D7 = false; // 4
bool D8 = false; // 5
bool D9 = false; // 6
bool D10 = false; // multiply
bool D11 = false; // divide
bool D12 = false; // constants
bool D13 = false; // 1
bool D14 = false; // 2
bool D15 = false; // 3
bool D16 = false; // open parentheses
bool D17 = false; // close parentheses
bool D18 = false; // delete
bool D19 = false; // 0
bool D20 = false; // decimal point
bool D21 = false; // negative
bool D22 = false; // power
bool D23 = false; // enter/equals
bool nextLine = false;
bool pressEqual = false;


void setup() {
 
  pinMode(Q0,INPUT);
  pinMode(Q1,INPUT);
  pinMode(Q2,INPUT);
  pinMode(Q3,INPUT);
  pinMode(Q4,INPUT);
  digitalWrite(Q0,LOW);
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q3,LOW);
  digitalWrite(Q4,LOW);
    Serial.begin(9600);



tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  
  uint16_t time = millis();
  tft.fillScreen(ST7735_CYAN);
  time = millis() - time;
    
    tft.setRotation(1);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(2,115);
    tft.print("52E12");


    
}

void loop() {
  readInput();
  Serial.println(digitalRead(Q0));
  Serial.println(digitalRead(Q1));
  Serial.println(digitalRead(Q2));
  Serial.println(digitalRead(Q3));
  Serial.println(digitalRead(Q4));
  Serial.println();
  delay(1000);


  if(pressEqual)
    {
      //tft.setCursor(120,115);
      //tft.drawLine(0,112,160,112,ST7735_BLACK);ss
      nextLine = true;
    }
    
    if(nextLine)
    {
      //tft.setCursor(2,90);
      nextLine = false;
    }
    
}


void readInput()
{
  Q0State = digitalRead(Q0);
  Q1State = digitalRead(Q1);
  Q2State = digitalRead(Q2);
  Q3State = digitalRead(Q3);
  Q4State = digitalRead(Q4);

  if(Q4State == HIGH)
  {
    if(Q3State == HIGH)
    {
      if(Q0State == LOW)
        D23 = true;
    }
    else
      {
        if(Q2State == HIGH)
          {
            if(Q1State == HIGH)
            {
              if(Q0State == HIGH)
                D22 = true;
              else
                D21 = true;
            }
            else
            {
              if(Q0State == HIGH)
                D20 = true;
               else
                D19 = true;
            }
          }
        else
        {
          if(Q1State == HIGH)
          {
            if(Q0State == HIGH)
              D18 = true;
            else
              D17 = true;
          }
          else
          {
            if(Q0State == HIGH)
              D16 = true;
              else 
              D15 = true;
          }
        }
      }
  }
  else
  {
    if(Q3State == HIGH)
    {
      if(Q2State == HIGH)
        {
          if(Q1State == HIGH)
          {
            if(Q0State == HIGH)
              D14 = true;
            else
              D13 = true;
          }
          else
          {
            if(Q0State == HIGH)
              D12 = true;
            else
              D11 = true;
          }
        }
        else
        {
          if(Q1State == HIGH)
          {
            if(Q0State == HIGH)
              D10 = true;
            else
              D19 = true;
          }
          else
          {
            if(Q0State == HIGH)
              D8 = true;
            else
              D7 = true;
          }
        }
    }
    else 
    {
      if(Q2State == HIGH)
        {
          if(Q1State == HIGH)
          {
            if(Q0State == HIGH)
              D6 = true;
            else
              D5 = true;
          }
          else
          {
            if(Q0State == HIGH)
              D4 = true;
            else
              D3 = true;
          }
        }
        else
        {
          if(Q1State == HIGH)
          {
            if(Q0State == HIGH)
              D2 = true;
            else
              D1 = true;
          }
          else
          {
            if(Q0State == HIGH)
              D0 = true;
          }
        }
    }
    }
  }

void clearInputs()
{
 D0 = false; //esc
 D1 = false; //7
 D2 = false; //8
 D3 = false; //9
 D4 = false; // add
 D5 = false; // subtract
 D6 = false; // trig
 D7 = false; // 4
 D8 = false; // 5
 D9 = false; // 6
 D10 = false; // multiply
 D11 = false; // divide
 D12 = false; // constants
 D13 = false; // 1
 D14 = false; // 2
 D15 = false; // 3
 D16 = false; // open parentheses
 D17 = false; // close parentheses
 D18 = false; // delete
 D19 = false; // 0
 D20 = false; // decimal point
 D21 = false; // negative
 D22 = false; // power
 D23 = false; // enter/equals
}

String getRidOfSpaces(String str1)
{
    int stringLength = str1.length();

    for(int i = 0; i < stringLength ; i++)
    {
      if(str1[i] == ' ')
      {
        str1.remove(i,1);
        stringLength = str1.length();
        i--;
      }
    }
    return str1;
}

int findPosString(String str1, char findChar, int start)
{
    int stringLength = str1.length();
    bool continueLooking = true;
    bool noMatch = false;
    
    while(continueLooking)
    {
        if(str1[start] == findChar)
        {
            continueLooking = false;
        }
        
        if(start > stringLength)
        {
            noMatch = true;
            continueLooking = false;
        }
        
        start++;
    }
    if(noMatch)
        return -1;
    else
        return start - 1;
}

String reverseString(String str1)
{
    String newString;
    long stringLength = str1.length();
    
    for(int i = 0; i <= stringLength; i++)
    {
        newString += str1[stringLength - i];
    }
    
    return newString;
}

String left(String str1, int pos)
{
    String num;
    int b = pos;
    
    for(int i = 0 ; i < pos ;i++)
    {
        num += str1[b-1];
        b--;
    }
    
    num = reverseString(num);
    return num;
}


String right(String str1, int pos)
{
    String num;
    int b = pos;
    
    for(int i = 0 ; i < str1.length() - pos;i++)
    {
        
        num += str1[b+1];
        b++;
    }
    return num;
}

int lastInst(String str1, char search)
{
    int b = 0;
    long maxPos = str1.length();
    bool nothingFound = true;
    for(int i = maxPos; i > 0; i--)
    {
        if(str1[i] == search)
        {
            b = i;
            i = 0;
            nothingFound = false;
        }
    }
    if(nothingFound)
    {
        return -1;
    }
    else
        return b;
}

int findPar(String str1, char search)
{
    int b = 0;
    long maxPos = str1.length();
    bool nothingFound = true;
    int POpen = 0;
    int PClose = 0;
    
    for(int i = maxPos; i > 0; i--)
    {
        if(str1[i] == '(')
            POpen++;
        if(str1[i] == ')')
            PClose++;
        
        if(str1[i] == search && POpen == PClose)
        {
            b = i;
            i = 0;
            nothingFound = false;
        }
    }
    
    if(nothingFound)
    {
        return -1;
    }
    else
        return b;
}

String deletePar( String str1)
{
    if(str1[0] == '(' && str1[str1.length() - 1] == ')')
    {
        str1[0] = ' ';
        str1[str1.length() - 1] = ' ';
    }
    return str1;
}

double Eval(String str1)
{

    str1 = getRidOfSpaces(str1);
    String part1;
    String part2;

    long pos_minus = lastInst(str1, '-');
    long pos_plus = lastInst(str1, '+');
    long pos_divide = lastInst(str1, '/');
    long pos_multiply = lastInst(str1, '*');
    long pos_power = lastInst(str1, '^');

    str1 = deletePar(str1);

    if(pos_minus > pos_plus)
    {
    if(pos_minus != -1)
    {
        part1 = left(str1, pos_minus);
        part2 = right(str1, pos_minus);
        return Eval(part1) - Eval(part2);
    }
    
    if(pos_plus != -1)
    {
    part1 = left(str1, pos_plus);
    part2 = right(str1, pos_plus);
        return Eval(part1) + Eval(part2);
    }
    }
    if(pos_minus < pos_plus)
    {
        if(pos_plus != -1)
        {
            part1 = left(str1, pos_plus);
            part2 = right(str1, pos_plus);
            return Eval(part1) + Eval(part2);
        }
        if(pos_minus != -1)
        {
            part1 = left(str1, pos_minus);
            part2 = right(str1, pos_minus);
            return Eval(part1) - Eval(part2);
        }
        
    }
    
    if(pos_multiply > pos_divide)
    {
    if(pos_multiply != -1)
    {
        part1 = left(str1, pos_multiply);
        part2 = right(str1, pos_multiply);
        return Eval(part1) * Eval(part2);
    }
        
    if(pos_divide != -1)
    {
        part1 = left(str1, pos_divide);
        part2 = right(str1, pos_divide);
        return Eval(part1) / Eval(part2);
    }
    }
    if(pos_multiply < pos_divide)
    {
        if(pos_divide != -1)
        {
            part1 = left(str1, pos_divide);
            part2 = right(str1, pos_divide);
            return Eval(part1) / Eval(part2);
        }
        
        if(pos_multiply != -1)
        {
            part1 = left(str1, pos_multiply);
            part2 = right(str1, pos_multiply);
            return Eval(part1) * Eval(part2);
        }
        
        
    }
        
    if(pos_power != -1)
    {
        part1 = left(str1, pos_power);
        part2 = right(str1, pos_power);
        return pow(Eval(part1),Eval(part2));
    }
    
    return str1.toFloat();
    
}




