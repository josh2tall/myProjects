/*#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS     10
#define TFT_RST    9               
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
*/

int Q0 = 8;
int Q1 = 9;
int Q2 = 10;
int Q3 = 11;

int F0 = 5;
int F1 = 6;
int F2 = 7;

int Q0State;
int Q1State;
int Q2State;
int Q3State;
int F0State;
int F1State;
int F2State;

bool nextLine = false;
bool pressEqual = false;

String str1 = "";

void setup() {
 
  pinMode(Q0,INPUT);
  pinMode(Q1,INPUT);
  pinMode(Q2,INPUT);
  pinMode(Q3,INPUT);

  pinMode(F0,INPUT);
  pinMode(F1,INPUT);
  pinMode(F2,INPUT);

  
  digitalWrite(Q0,LOW);
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q3,LOW);

  digitalWrite(F0,LOW);
  digitalWrite(F1,LOW);
  digitalWrite(F2,LOW);

    Serial.begin(9600);


/*
tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  
  uint16_t time = millis();
  tft.fillScreen(ST7735_CYAN);
  time = millis() - time;
    
    tft.setRotation(1);
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(2,115);
    tft.print("52E12");

*/
    
}

void loop() {
  readInput(str1);
  
  if(pressEqual)
    {
      //tft.setCursor(120,115);
      //tft.drawLine(0,112,160,112,ST7735_BLACK);
      Serial.print("Answer is ");
      Serial.println(Eval(str1));
      nextLine = true;
    }
    
    if(nextLine)
    {
      //tft.setCursor(2,90);
      nextLine = false;
      pressEqual = false;
      str1 = "";
      
    }

  Serial.println(str1);
   delay(1000);
}
void readInput(String& str1)
{
  Q0State = digitalRead(Q0);
  Q1State = digitalRead(Q1);
  Q2State = digitalRead(Q2);
  Q3State = digitalRead(Q3);
  
  F0State = digitalRead(F0);
  F1State = digitalRead(F1);
  F2State = digitalRead(F2);

   
   Serial.print("Q0 State: ");
   Serial.println(Q0State);
   Serial.print("Q1 State: ");
   Serial.println(Q1State);
   Serial.print("Q2 State: ");
   Serial.println(Q2State);
   Serial.print("Q3 State: ");
   Serial.println(Q3State);

   Serial.print("F0 State: ");
   Serial.println(F0State);
   Serial.print("F1 State: ");
   Serial.println(F1State);
   Serial.print("F2 State: ");
   Serial.println(F2State);
   Serial.println();

   if(Q3State)
   {
    if(Q2State)
    {
      if(Q1State)
      {
        if(Q0State) //1111
          {}//pressEqual = true;
        else        //1110
          {str1 += "/";}
      }
      else
      {
        if(Q0State) //1101
          {str1 += "*";}
        else        //1100
          {str1 += "-";}
      }
    }
   
    else
    {
      if(Q1State)
      {
        if(Q0State) //1011
          {str1 += "+";}
        else        //1010
          {str1 += "9";}
      }
      else
      {
        if(Q0State) //1001
          {str1 += "8";}
        else        //1000
          {str1 += "7";}
      }
    }
   }
  else
  {
    if(Q2State)
    {
      if(Q1State)
      {
        if(Q0State) //0111
          {str1 += "6";}
        else        //0110
          {str1 += "5";}
      }
      else
      {
        if(Q0State) //0101
          {str1 += "4";}
        else        //0100
          {str1 += "3";}
      }
    }
    else
    {
      if(Q1State)
      {
        if(Q0State) //0011
          {str1 += "2";}
        else        //0010
          {str1+= "1";}
      }
      else
      {
        if(Q0State) //0001
          {str1 += "0";}
        else        //0000
          {}//not used
      }
    }
   }


   if(F2State)
   {
    if(F1State)
    {
      if(F0State)
        {}
      else
        {}
    }
    else
    {
      if(F0State)
        {str1 += ')';}
      else
        {str1 += '(';}
    }
   }
   else
   {
    if(F1State)
    {
      if(F0State)
        {str1 += '.';}
      else
        {str1 = getRidNums(str1,str1.length()-1,str1.length());
         str1 = getRidOfSpaces(str1);
         }
    }
    else
    {
      if(F0State)
        {str1 += '^';}
      else
        {} // not used
   }
  }

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

String getRidOfSpaces(String str1)
{
    long stringLength = str1.length();
    
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

//returns the position of the operater to the left of position pos
int findNextItemL(String str1, int pos)
{
    int i;
    char plus = '+';
    char sub = '-';
    char divide = '/';
    char mult = '*';
        char OP = '(';
    char CP = ')';
    int toTheLeft = 0;
    bool wasThereOper = false;
    
    for( i = pos - 1; i >= 0; i--)
    {
        if(str1[i] == plus || str1[i] == sub || str1[i] == divide || str1[i] == mult || str1[i] == OP || str1[i] == CP)
        {
            toTheLeft = i;
            i = -1;
            wasThereOper = true;
        }
        if(wasThereOper == false)
            toTheLeft = -1;
    }
    return toTheLeft;
}

//returns the position of the operater to the right of position pos
int findNextItemR(String str1, int pos)
{
    
    char plus = '+';
    char sub = '-';
    char divide = '/';
    char mult = '*';
    char OP = '(';
    char CP = ')';

    long toTheRight = 0;
    bool wasThereOper = false;
    
    for(long i = pos + 1; i < str1.length(); i++)
    {
        if(str1[i] == plus || str1[i] == sub || str1[i] == divide || str1[i] == mult || str1[i] == OP || str1[i] == CP)
        {
            toTheRight = i;
            i = str1.length();
            wasThereOper = true;
        }
    }
    if(wasThereOper == false)
        toTheRight = str1.length();
    
    return toTheRight;
}

String getSection(String str1, int start, int end)
{
    String a;
    
    for(int i = start; i <= end; i++)
    {
        a += str1[i];
    }
    return a;
}
//finds the first instance of a
int findFirstInstance(String str1,char a)
{
    int pos = -1;
    bool wasFound = false;
    
    for(int i = str1.length() - 1; i > -1; i--)
    {
        if(str1[i] == a)
        {
            pos= i;
            wasFound = true;
        }
    }
    if(wasFound)
        return pos;
    else
        return -1;
}

int findFirstInstance(String str1,char a, int x, int y)
{
    int pos = -1;
    bool wasFound = false;
    
    for(int i = y; i >= x; i--)
    {
        if(str1[i] == a)
        {
            pos= i;
            wasFound = true;
        }
    }
    if(wasFound)
        return pos;
    else
        return -1;
}

int findFirstInstancePara(String str1,char a)
{
    int pos = -1;
    bool wasFound = false;
    
    for(int i = str1.length() - 1; i > -1; i--)
    {
        if(str1[i] == a)
        {
            pos= i;
            wasFound = true;
        }
    }
    if(wasFound)
        return pos;
    else
        return str1.length();
}
int findLastInstancePara(String str1,char a, int closeP)
{
    int pos = -1;
    bool wasFound = false;
    
    for(int i = 0; i < closeP; i++)
    {
        if(str1[i] == a)
        {
            pos= i;
            wasFound = true;
        }
    }
    if(wasFound)
        return pos;
    else
        return -1;
}
//add spaces inside a string
String addSpaces(String str1, int numSpaces, int startPos)
{
    for(int j = 0; j < numSpaces; j++)
    {
    //move everything over one space
        for(int i = str1.length(); i >startPos; i--)
        {
            if(i == str1.length())
                str1 += str1[i-1];
            else
                str1[i] = str1[i-1];
        }
        str1[startPos] = ' ';
    }
    return str1;
}
// adds a string into str1
String remakeString(String str1, String op, int start)
{
    int j = 0;
    str1 = addSpaces(str1, op.length(), start);
    int i = start;
    
    for(int k = 0; k < op.length(); k++)
    {
        str1[i] = op[j];
        i++;
        j++;
    }
    
    return str1;
}

String getRidNums(String str1, int start, int end)
{
    
    for(int i = start; i <= end; i ++)
    {
        str1[i] = ' ';
    }
    
    return str1;
}

double Eval(String str1)
{
    int posMultiply = -1;
    int posDivide = -1;
    int posPlus = -1;
    int posMinus = -1;
    int posPower = -1;
    
    int posStart = 0;
    int posEnd = str1.length() - 1;
    
    String left = "";
    String right = "";
    String operation = "";
    
    double leftNum = 10000;
    double rightNum = 10000;
    
    bool notComplete = true;
    bool plus = false;
    bool minus = false;
    bool multiply = false;
    bool divide = false;
    bool power = false;
    
    while(notComplete)
    {
        plus = false;
        minus = false;
        multiply = false;
        divide = false;
        power = false;
        
        leftNum = 10000;
        rightNum = 10000;
        
        str1 = getRidOfSpaces(str1);

        
        posEnd = findFirstInstancePara(str1, ')') - 1;
        posStart = findLastInstancePara(str1,'(',posEnd) + 1;
        
        
        posDivide = findFirstInstance(str1,'/',posStart,posEnd);
        posMultiply = findFirstInstance(str1,'*',posStart,posEnd);
        posMinus = findFirstInstance(str1,'-',posStart,posEnd);
        posPlus = findFirstInstance(str1,'+',posStart,posEnd);
        posPower = findFirstInstance(str1, '^',posStart,posEnd);

        left = "";
        right = "";
        operation = "";
        
        
        if( posDivide != -1 || posMultiply != -1 || posPower != -1)
        {
  
            if(posPower > -1)
            {
                left = getSection(str1, findNextItemL(str1,posPower) + 1, posPower - 1);
                right = getSection(str1, posPower + 1, findNextItemR(str1, posPower) - 1);
                
                power = true;
            }
            else if((posMultiply < posDivide && posMultiply > -1) ||(posMultiply > 0 && posDivide == -1))
            {
                left = getSection(str1, findNextItemL(str1,posMultiply) + 1, posMultiply - 1);
                right = getSection(str1, posMultiply + 1, findNextItemR(str1, posMultiply) - 1);

                multiply = true;
            }
            else if((posDivide < posMultiply && posDivide > -1) || (posDivide > 0 && posMultiply == -1))
            {
                left = getSection(str1, findNextItemL(str1,posDivide) + 1, posDivide - 1);
                right = getSection(str1, posDivide + 1, findNextItemR(str1, posDivide) - 1);
                divide = true;
            }
        }
        else if(posPlus!= -1 || posMinus != -1)
        {
            if((posPlus < posMinus && posPlus > -1) || (posPlus > 0 && posMinus == -1))
            {
                left = getSection(str1, findNextItemL(str1,posPlus) + 1, posPlus - 1);
                right = getSection(str1, posPlus + 1, findNextItemR(str1, posPlus) - 1);                plus = true;
            }
            else if((posMinus < posPlus && posMinus > -1) ||(posMinus >0 && posPlus == -1))
            {
                left = getSection(str1, findNextItemL(str1,posMinus) + 1, posMinus - 1);
                right = getSection(str1, posMinus + 1, findNextItemR(str1, posMinus) - 1);
                minus = true;
            }
        }
        if(posPlus!= -1 || posMinus != -1 || posDivide != -1 || posMultiply != -1 || posPower != -1)
        {
          if(str1[posStart] == '(' && str1[posEnd] == ')')
          {
            str1[posStart] == ' ';
            str1[posEnd] = ' ';
          }
          
          else
          {
            leftNum = left.toFloat();
            rightNum = right.toFloat();
          }
        }

        
        if(power)
        {
            str1 = getRidNums(str1, findNextItemL(str1, posPower) + 1, findNextItemR(str1, posPower)-1);
            String operation(pow(leftNum,rightNum));
            str1 = remakeString(str1,operation,posPower);
        }
        else if(multiply)
        {
            str1 = getRidNums(str1, findNextItemL(str1, posMultiply) + 1, findNextItemR(str1, posMultiply)-1);
            String operation(leftNum * rightNum);
            str1 = remakeString(str1,operation,posMultiply);

        }
        else if(divide)
        {
            str1 = getRidNums(str1, findNextItemL(str1, posDivide) + 1, findNextItemR(str1, posDivide)-1);
            String operation(leftNum / rightNum);
            str1 = remakeString(str1,operation, posDivide);

        }
        else if(plus)
        {
            str1 = getRidNums(str1, findNextItemL(str1, posPlus) + 1, findNextItemR(str1, posPlus)-1);
            String operation(leftNum + rightNum);
            str1 = remakeString(str1,operation, posPlus);

        }
        else if(minus)
        {
            str1 = getRidNums(str1, findNextItemL(str1, posMinus) + 1, findNextItemR(str1, posMinus)-1);
            String operation(leftNum-rightNum,10);
            str1 = remakeString(str1,operation,posMinus);

        }
        
        if(posMinus == -1 && posDivide == -1 && posPlus == -1 && posMultiply == -1)
        {
          if(str1[posStart-1] == '(' && str1[posEnd+1] == ')')
            {
                str1[posStart-1]= ' ';
                str1[posEnd+1] = ' ';
            }
            else
            {
            notComplete = false;
            }
        }

    }
    return str1.toFloat();
    
}


