#include <Adafruit_MotorShield.h>
#include <ArduinoSTL.h>
#include <vector>

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
// -1 left, 0 straight, 1 right,-2 reverse left, 2 reverse right, -3 anticlockwise 180, 3 clockwise 180
std::vector<int> commandlist = {};

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

void clockwise180() {
  Serial.println("Clockwise 180");
  forward = true;
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(1800);
  while(true){
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
      break;
    }
  }
}
void backlineFollow(){
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(230);
  rightMotor->setSpeed(250);
//   int valLeft = readleft(); // read left input value
//   //Serial.print(valLeft);
//   int valRight = readright(); // read right input value
//   //Serial.println(valRight);
//   if (valLeft) {
//     leftMotor->setSpeed(20);
//     rightMotor->setSpeed(250);
//     delay(70);
//     leftenable = false;
//     rightenable = true;
//   }
//   else if (valRight) {
//     rightMotor->setSpeed(20);
//     leftMotor->setSpeed(250);
//     leftenable = true;
//     rightenable = false;
//     delay(70);
//   }
//   else {
//     leftMotor->setSpeed(230);
//     rightMotor->setSpeed(250);
//     // leftenable = true;
//     // rightenable = true;
//   }
}

void lineFollow() {
    Serial.println("Line following");
    // leftMotor->setSpeed(250);
    // rightMotor->setSpeed(250);
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    int valLeft = readleft();
    //Serial.print(valLeft);
    int valRight = readright();
    //Serial.println(valRight);

    if (valLeft) {
      leftMotor->setSpeed(0);
      rightMotor->setSpeed(250);
    }
    else if (valRight) {
      rightMotor->setSpeed(0);
      leftMotor->setSpeed(250);
    }
    else {
      leftMotor->setSpeed(230);
      rightMotor->setSpeed(250);
    }
}

void turnRight() {
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
void turnLeft() {
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
void backturnright(){
  Serial.println("Back turn right");
  forward = true;
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(250);
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
      delay(100);
      break;
      
    }
  }
}
void backturnleft(){
  Serial.println("Back turn left");
  forward = true;
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(250);
  //stage 1, far triggered, stage 2 near triggered
  int stage = 1;

  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
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
    Serial.println(rightlinesensorPin);
    if(digitalRead(rightlinesensorPin)){
      leftMotor->run(BACKWARD);
      rightMotor->run(BACKWARD);
      leftMotor->setSpeed(250);
      rightMotor->setSpeed(250);
      delay(100);
      stage = 2;
      break;
      
    }
  }
}

void donextcommand(){
  // Read first command, then remove from the queue

  int next = commandlist.front();
  //commandlist.remove(0);
  commandlist.erase(commandlist.begin());
  
  if(next == -1){
      turnLeft();
    }
  else if(next == 0){
    leftMotor->setSpeed(230);
    rightMotor->setSpeed(250);
    delay(1000);
  }
  else if(next == 1){
    turnRight();
  }
  else if(next == 2){
    backturnright();
    forward = true;
  }
  else if (next == -2){
    backturnleft();
    forward = true;
  }
  else if (next == 3){
    clockwise180();
  }
}

void get_path(int currentPos, int target) {
  forward = false;  // In most cases we need to reverse after getting the path
  if (currentPos == 0 and target == 3) {
    forward = true;  // This is the only time we want to go forward after getting the path
    commandlist = {0, -1, 1};
  }
  
  else if (currentPos == 1 and target == 4) {
    commandlist = {3, -1, -1};
  }
  else if (currentPos == 1 and target == 5) {
    commandlist = {3, 0, -1};
  }
  else if (currentPos == 1 and target == 6) {
    commandlist = {3, -1, 0, 1, -1};
  }

  else if (currentPos == 2 and target == 4) {
    commandlist = {3, 1, 0, 1};
  }
  else if (currentPos == 2 and target == 5) {
    commandlist = {3, 0, 0, 1};
  }
  else if (currentPos == 2 and target == 6) {
    commandlist = {3, 1, -1, -1};
  }

  else if (currentPos == 3 and target == 1) {
    commandlist = {2, 0, 1};
  }
  else if (currentPos == 3 and target == 2) {
    commandlist = {-2, -1};
  }
  else if (currentPos == 4 and target == 1) {
    commandlist = {-2, 1, 0};
  }
  else if (currentPos == 4 and target == 2) {
    commandlist = {2, 0, -1, 0};
  }
  else if (currentPos == 5 and target == 1) {
    commandlist = {-2, 0, 0};
  }
  else if (currentPos == 5 and target == 2) {
    commandlist = {2, 0, 0, 0};
  }
  else if (currentPos == 6 and target == 1) {
    commandlist = {-2, -1, 0, 1, 0};
  }
  else if (currentPos == 6 and target == 2) {
    commandlist = {-2, 1, -1, 0};
  }
  else if (currentPos = 1) {
    commandlist = {2, -1};
  }
  else if (currentPos = 2) {
    commandlist = {-2, 0, 1};
  }
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
  while(!digitalRead(buttonPin)){
    continue;
  }
  delay(500);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(230);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  forward = false;
  get_path(5,2);
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
  if (forward){
    lineFollow();
   }
  else{backlineFollow();}
  
  if(readfarleft() || readfarright()){
    delay(50);
    if(!readfarleft() && !readfarright()){return;}  // Returns to loop if it was just a speck of dust
    //leftenable = false;
    //rightenable = false;
    donextcommand();

    leftenable = true;
    rightenable = true;
  }
}
