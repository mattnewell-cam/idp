#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
int buttonPin = 8;
int leftlinesensorPin = 6;
int rightlinesensorPin = 4;
int farleftPin = 5;
int farrightPin = 7;
int pause= 0;
bool leftenable = true;
bool rightenable = true;
int forward = true;
// -1 left, 0 straight, 1 right,-2 reverse left, 2 reverse right
int commandlist[] = {2,1,-1,0,-1,1};
int commandlength = 6;
int commandpos = 0;
int readfarleft(){
  return digitalRead(farleftPin);
}
int readfarright(){
  return digitalRead(farrightPin);
}
int readleft(){
  if(leftenable){
    return digitalRead(leftlinesensorPin);
  }
  else{
    return 0;
  }
}
int readright(){
  if(rightenable){
    return digitalRead(rightlinesensorPin);
  }
  else{
    return 0;
  }
}
int nextcommand(){
  int temp = commandlist[commandpos];
  commandpos++;
  if(commandpos >=commandlength ){
    commandpos = 0;
  }
  return temp;
}
void backturnright(){
  Serial.println("Turning right");
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  //delay(250);
  //stage 1, far triggered, stage 2 near triggered
  int stage = 1;

  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(900);
  while(stage != 2){
    if(pause == 1){
      continue;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

        pause = 1;
        leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(leftlinesensorPin);
    if(digitalRead(leftlinesensorPin)){
      leftMotor->run(BACKWARD);
      rightMotor->run(BACKWARD);
      leftMotor->setSpeed(250);
      rightMotor->setSpeed(250);
      stage = 2;
      Serial.println("left");
      break;
      
    }
}
}
void backlineFollow(){
  leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    int valLeft = readleft(); // read left input value
    //Serial.print(valLeft);
    int valRight = readright(); // read right input value
    //Serial.println(valRight);
    if (valLeft) {
      leftMotor->setSpeed(20);
      rightMotor->setSpeed(250);
      delay(70);
      leftenable = false;
      rightenable = true;
    }
    else if (valRight) {
      rightMotor->setSpeed(20);
      leftMotor->setSpeed(250);
      leftenable = true;
      rightenable = false;
      delay(70);
    }
    else {
      leftMotor->setSpeed(230);
      rightMotor->setSpeed(250);
      // leftenable = true;
      // rightenable = true;
    }
}
void lineFollow() {
    Serial.println("Line following");
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    int valLeft = readleft(); // read left input value
    //Serial.print(valLeft);
    int valRight = readright(); // read right input value
    //Serial.println(valRight);

    if (valLeft) {
      leftMotor->setSpeed(20);
      rightMotor->setSpeed(250);
    }
    else if (valRight) {
      rightMotor->setSpeed(20);
      leftMotor->setSpeed(250);
    }
    else {
      leftMotor->setSpeed(230);
      rightMotor->setSpeed(250);
    }
}


void turnLeft() {
  delay(500);
  leftMotor->setSpeed(150);
  leftMotor->run(FORWARD);
  delay(1000);
  int val = 0;
  while(!val) {
    val = digitalRead(leftlinesensorPin);
  }
}
void turnRight2() {
  Serial.println("Turning right");
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(250);
  //stage 1, far triggered, stage 2 near triggered
  int stage = 1;

  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(100);
  delay(1000);
  while(stage != 2){
    if(pause == 1){
      continue;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

        pause = 1;
        leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(leftlinesensorPin);
    if(digitalRead(leftlinesensorPin)){
      leftMotor->run(BACKWARD);
      rightMotor->run(BACKWARD);
      leftMotor->setSpeed(250);
      rightMotor->setSpeed(250);
      stage = 2;
      Serial.println("left");
      break;
      
    }
  }

  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
}
void turnLeft2() {
  Serial.println("Turning left");
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(200);
  //stage 1, far triggered, stage 2 near triggered
  int stage = 1;

  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(100);
  rightMotor->setSpeed(250);
  delay(1000);
  while(stage != 2){
    if(pause == 1){
      continue;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

        pause = 1;
        leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(rightlinesensorPin);
    if(digitalRead(rightlinesensorPin)){
      leftMotor->run(BACKWARD);
      rightMotor->run(BACKWARD);
      leftMotor->setSpeed(250);
      rightMotor->setSpeed(250);
      stage = 2;
      Serial.println("right");
      break;
      
    }
  }

  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
}

void turnRight() {
  rightMotor->setSpeed(0);
  delay(500);
  int val = 0;
  while(!val) {
    val = digitalRead(leftlinesensorPin);
  }
}
void intersection(int turn){
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Adafruit Motorshield v2 - DC Motor test!");

    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
      while (1);
    }
    Serial.println("Motor Shield found.");

  leftMotor->setSpeed(250);
  rightMotor->setSpeed(230);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  forward = false;
}

void loop() {
  if(pause){
    leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
    return;
  }
  if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

        pause = 1;
        leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);

   }
   if(!forward){
      backlineFollow();
   }
   else{lineFollow();}
  
  if(readfarleft() || readfarright()){
    delay(50);
    if(!readfarleft()){return;}
    leftenable = false;
    rightenable = false;
    int next = nextcommand();
    if(next == -1){
      turnLeft2();
    }
    else if(next == 0){
      leftMotor->setSpeed(230);
      rightMotor->setSpeed(250);
      delay(1000);
    }
    else if(next == 1){
      
      turnRight2();
    
    }
    else if(next == 2){
      
      backturnright();
      forward = true;
    
    }
    leftenable = true;
    rightenable = true;
  }
}
