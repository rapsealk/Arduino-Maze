#include <Servo.h>

Servo servoLeft;
Servo servoRight;

typedef enum {UP, DOWN, LEFT, RIGHT} dir_t;

typedef struct {
  int x;
  int y;
} point_t;

char MAP[11][11] = {{1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,1,0,1,0,1,0,1,0,1},
                    {1,1,3,1,3,1,3,1,3,1,1},
                    {1,0,1,0,1,0,1,0,1,0,1},
                    {1,1,3,1,3,1,3,1,3,1,1},
                    {1,0,1,0,1,0,1,0,1,0,1},
                    {1,1,3,1,3,1,3,1,3,1,1},
                    {1,0,1,0,1,0,1,0,1,0,1},
                    {1,1,3,1,3,1,3,1,3,1,1},
                    {1,0,1,0,1,0,1,0,1,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1}};
char MAP2[11][11] = {0,};                    

int trigFront = 11;
int echoFront = 10;
int trigLeft = 9;
int echoLeft = 8;
int trigRight = 7;
int echoRight = 6;

long FrontDistance = 0;
long LeftDistance = 0;
long RightDistance = 0;
long TimeDelay = 0;

int right = 0;
int left = 0;
int up = 0;
int down = 0;

point_t user = {1, 1};
point_t pass = {4, 4};
point_t dest = {5, 5};

// Declare Functions
void InitiatePoint(); // 좌표 세팅 함수 (x,y)->(2x-1,2y-1)
void Go(); // 한 칸 전진
void Turn(dir_t dir); // 회전
void GetDistance(); // 거리 측정
int Calculate(); // 계획 수립
int Move(); // 총괄 함수
void TurnMap(dir_t dir); // 지도 회전

// Main Functions
void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
  InitiatePoint();
}

void loop() {
  while(user.x != pass.x && user.y != pass.y) {
    GetDistance();
    if(!Calculate()) {
      pass = dest;
      break;
    }
    Move();
  }
  while(user.x != pass.x && user.y != pass.y) {
    GetDistance();
    if(!Calculate()) {
      break;
    }
    Move();
  }
  servoLeft.detach();
  servoRight.detach();
}

// Define Functions
void InitiatePoint()
{
  user.x = 2*user.x - 1;
  user.y = 2*user.y - 1;
  pass.x = 2*pass.x - 1;
  pass.y = 2*pass.y - 1;
  dest.x = 2*dest.x - 1;
  dest.y = 2*dest.y - 1;
}

void Go()
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(1700);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(50);
}

void Turn(dir_t dir)
{
  int angle;
  
  switch(dir) {
    case LEFT:
      TurnMap(RIGHT);
      for(angle = 0; angle < 45; angle++) {
        servoLeft.write(angle);
        servoRight.write(angle);
        delay(15);
      }
      break;
    case RIGHT:
      TurnMap(LEFT);
      for(angle = 120; angle < 170; angle++) {
        servoLeft.write(angle);
        servoRight.write(angle);
        delay(15);
      }
      break;
  }
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(100);
}

void GetDistance()
{
  digitalWrite(trigLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigLeft, LOW);
  TimeDelay = pulseIn(echoLeft, HIGH);
  LeftDistance = TimeDelay / 29 / 2;

  if(LeftDistance > 20) {
    MAP[user.x][user.y-1] = 0;
  }
  
  digitalWrite(trigRight, LOW);
  delayMicroseconds(2);
  digitalWrite(trigRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigRight, LOW);
  TimeDelay = pulseIn(echoRight, HIGH);
  RightDistance = TimeDelay / 29 / 2;

  if(RightDistance > 20) {
    MAP[user.x][user.y+1] = 0;
  }

  digitalWrite(trigFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigFront, LOW);
  TimeDelay = pulseIn(echoFront, HIGH);
  FrontDistance = TimeDelay / 29 / 2;

  if(FrontDistance > 20) {
    MAP[user.x+1][user.y] = 0;
  }
}

int Calculate()
{
  up = down = left = right = 0;
  if (user.x == dest.x) {
    up = down = 0;
    if (user.y == dest.y) {
      return 0;
    }
    else if (user.y > dest.y) {
      if (MAP[user.x][user.y - 1] != '1') {
        left = 1;
      }
    }
    else if (user.y < dest.y) {
      right = 1;
    }
  }
  else if (user.x > dest.x) {
    up = 1;
    if (user.y > dest.y) {
      left = 1;
    }
    else if (user.y < dest.y) {
      right = 1;
    }
  }
  else if (user.x < dest.x) {
    down = 1;
    if (user.y > dest.y) {
      left = 1;
    }
    else if (user.y < dest.y) {
      right = 1;
    }
  }
  return 1;
}

int Move()
{
  while (down > 0) {
    if (MAP[user.x + 1][user.y] != 1) {
      user.x += 2;
      down--;
      Go();
    }
    else {
        if (MAP[user.x][user.y + 1] != 1) {
          user.y += 2;
          right--;
          Turn(RIGHT);
          Go();
        }
        else if (MAP[user.x][user.y-1] != 1) {
          user.y -= 2;
          left--;
          Turn(LEFT);
          Go();
        }
        else {
          user.x -= 2;
          up--;
          Turn(LEFT);
          Turn(LEFT);
          Go();
        }
    }
    GetDistance();
  }
  while (up > 0) {
    if (MAP[user.x - 1][user.y] != 1) {
      user.x -= 2;
      up--;
      Turn(LEFT);
      Turn(LEFT);
      Go();
    }
    else {
        if (MAP[user.x][user.y + 1] != 1) {
          user.y += 2;
          right--;
          Turn(RIGHT);
          Go();
        }
        else if (MAP[user.x][user.y - 1] != 1) {
          user.y -= 2;
          left--;
          Turn(LEFT);
          Go();
        }
        else {
          user.x += 2;
          down--;
          Go();
        }
    }
    GetDistance();
  }
  while(right > 0) {
    if (MAP[user.x][user.y + 1] != 1) {
      user.y += 2;
      right--;
      Turn(RIGHT);
      Go();
    }
    else {
        if (MAP[user.x + 1][user.y] != 1) {
          user.x += 2;
          down--;
          Go();
        }
        else if (MAP[user.x - 1][user.y] != 1) {
          user.x -= 2;
          up--;
          Turn(LEFT);
          Turn(LEFT);
          Go();
        }
        else {
          user.y -= 2;
          left--;
          Turn(LEFT);
          Go();
        }
    }
    GetDistance();
  }
  while (left > 0) {
    if (MAP[user.x][user.y - 1] != 1) {
      user.y -= 2;
      left--;
      Turn(LEFT);
      Go();
    }
    else {
        if (MAP[user.x + 1][user.y] != 1) {
          user.x += 2;
          down--;
          Go();
        }
        else if (MAP[user.x - 1][user.y] != 1) {
          user.x -= 2;
          up--;
          Turn(LEFT);
          Turn(LEFT);
          Go();
        }
        else {
          user.y += 2;
          right--;
          Turn(RIGHT);
          Go();
        }
    }
    GetDistance();
  }
  
  return 0;
}

void TurnMap(dir_t dir)
{
  int i, j;
  point_t temp;

  switch (dir) {
  case LEFT:
    for (i = 1; i < 10; i++) {
      for (j = 1; j < 10; j++) {
        MAP2[10 - j][i] = MAP[i][j];
      }
    }
    temp.x = 10 - user.y;
    temp.y = user.x;
    user = temp;
    temp.x = 10 - pass.y;
    temp.y = pass.x;
    pass = temp;
    temp.x = 10 - dest.y;
    temp.y = dest.x;
    dest = temp;
    break;
  case RIGHT:
    for (i = 1; i < 10; i++) {
      for (j = 1; j < 10; j++) {
        MAP2[j][10 - i] = MAP[i][j];
      }
    }
    temp.x = user.y;
    temp.y = 10 - user.x;
    user = temp;
    temp.x = pass.y;
    temp.y = 10 - pass.x;
    pass = temp;
    temp.x = dest.y;
    temp.y = 10 - dest.x;
    dest = temp;
    break;
  }

  for (i = 1; i < 10; i++) {
    for (j = 1; j < 10; j++) {
      MAP[i][j] = MAP2[i][j];
    }
  }
}
