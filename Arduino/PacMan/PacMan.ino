int leftPin = 2;
int rightPin = 3;
int upPin = 4;
int downPin = 5;
bool movingDown;
bool movingUp;
bool movingRight;
bool movingLeft;
class pacman
{
  public:
  int x, y, r;
};

void setup() {
pinMode(leftPin, INPUT);
pinMode(rightPin, INPUT);
pinMode(upPin, INPUT);
pinMode(downPin, INPUT);
}

void loop() {
  int leftState = digitalRead(leftPin);
  int rightState = digitalRead(rightPin);
  int downState = digitalRead(downPin);
  int upState = digitalRead(upPin);

pacman P1;
P1.x = 5;
P1.y = 5;
P1.r = 5;

if(leftState == HIGH)
{
  movingLeft = true;
  movingDown = false;
  movingUp = false;
  movingRight = false;
}
if(rightState == HIGH)
{
  movingRight = true;
  movingLeft = false;
  movingDown = false;
  movingUp = false;
}
if(upState == HIGH)
{
  movingRight = false;
  movingLeft = false;
  movingDown = false;
  movingUp = true;
}
if(downState == HIGH)
{
  movingRight = false;
  movingLeft = false;
  movingDown = true;
  movingUp = false;
}

if(movingRight)
  P1.x += 5;
if(movingLeft)
  P1.x -= 5;
if(movingUp)
  P1.y += 5;
if(movingDown)
  P1.y -= 5;
fillCircle(P1.x,P1.y, P1.r, Yellow);
delay(10);


}



