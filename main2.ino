#include <Adafruit_MotorShield.h>
#include <ArxContainer.h>
//#include <vector>
#include <arduino-timer.h>

auto blue_flash =  timer_create_default();
#include<Servo.h>
#define MAX_RANG (520)
#define ADC_SOLUTION (1023.0)
#define leftfor FORWARD
#define leftback BACKWARD
#define rightback BACKWARD
#define rightfor FORWARD
#include "DFRobot_TCS34725.h"
DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
float dist_t, sensity_t;
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
int buttonPin = 8;
int leftlinesensorPin = 6;
int rightlinesensorPin = 4;
int farleftPin = 5;
int farrightPin = 7;
int green = 10;
int blue =11;
int red = 13;

int ultrasounddistance = 100;
int pause= 0;
bool leftenable = true;
bool rightenable = true;
int forward = true;
int sensityPin = A3; 
//using namespace std;
Servo grabber;
// -1 left, 0 straight, 1 right,-2 reverse left, 2 reverse right, -3 anticlockwise 180, 3 clockwise 180, -4 turn left and grab, 4 turn right and grab, -5 turn left and release, 5 turn right and release, 15 straight and release, 16 return to base
std::vector<int> commandlist = {};
int curr;
int tar;
int crashsensorpin = 12;

// Parameters
int reversetime = 100;
int turntime = 800;
void slowlinefollow();
int blueon = 0;
int flash = 0;
//Servo myservo;
void blueflash(){
  if (flash){
  if(blueon == 0){
    blueon = 1;
  }else{
    blueon = 0;
  }
  Serial.print("here");
  digitalWrite(blue,blueon);
  }
}
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
bool is_red(){
  uint16_t clear, r, g, b;
  tcs.getRGBC(&r, &g, &b, &clear);
  if(r > 150){
    return true;
  }
  else{
    return false;
  }
}
int ultrasoundread(){
  sensity_t = analogRead(sensityPin);
 // turn the ledPin on
 Serial.print(sensity_t * MAX_RANG / ADC_SOLUTION);
 return dist_t = sensity_t * MAX_RANG / ADC_SOLUTION;
}
void clockwise180() {
  flash = 1;
  grabber.write(0);
  Serial.println("Clockwise 180");
  forward = true;
  delay(300);
  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  leftMotor->setSpeed(230);
  rightMotor->setSpeed(250);
  delay(1500);
   Serial.print("here");
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

      pause = 1;
      flash = 0;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(leftlinesensorPin);
    if(digitalRead(leftlinesensorPin)){
      flash = 1;
      straightfor(600);
      break;
    }
  }
}

void ccw180() {
  grabber.write(0);
  Serial.println("Clockwise 180");
  forward = true;
  delay(300);
  flash = 1;
  leftMotor->run(leftback);
  rightMotor->run(rightfor);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(230);
  delay(1500);
   Serial.print("here");
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));

      pause = 1;
      flash = 0;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(rightlinesensorPin);
    if(digitalRead(rightlinesensorPin)){
      flash = 1;
      straightfor(600);
      break;
    }
  }
}

void backlineFollow(){
flash = 1;
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  leftMotor->run(leftback);
  rightMotor->run(rightback);
}

void lineFollow() {
    flash = 1;
    Serial.println("Line following");
    leftMotor->run(leftfor);
    rightMotor->run(rightfor);
    int valLeft = readleft();
    //Serial.print(valLeft);
    int valRight = readright();
    //Serial.println(valRight);

    if (valLeft) {
      leftMotor->setSpeed(90);
      rightMotor->setSpeed(255);
    }
    else if (valRight) {
      rightMotor->setSpeed(90);
      leftMotor->setSpeed(255);
    }
    else {
      leftMotor->setSpeed(255);
      rightMotor->setSpeed(255);
    }
}
void straightfor(int p){
  for(int i=0;i<p/15;i++){
    leftenable = true;
    rightenable = true;
    lineFollow();
    delay(15);
  }
}
void slowlinefollow() {
    flash = 1;
    Serial.println("Line following");
    leftMotor->run(leftfor);
    rightMotor->run(rightfor);
    int valLeft = readleft();
    //Serial.print(valLeft);
    int valRight = readright();
    //Serial.println(valRight);

    if (valLeft) {
      leftMotor->setSpeed(80);
      rightMotor->setSpeed(230);
    }
    else if (valRight) {
      rightMotor->setSpeed(80);
      leftMotor->setSpeed(230);
    }
    else {
      leftMotor->setSpeed(230);
      rightMotor->setSpeed(230);
    }
}
int detectblock(){
  if(ultrasoundread()<3){
      delay(100);
      if(ultrasoundread()<3)
      return true;
  }
  return false;
}
void grab(){
    

    grabber.write(80);
    rightenable = true;
    leftenable = true;
    while(!detectblock()){
      blue_flash.tick();
        slowlinefollow();
    }
    leftMotor->setSpeed(185);
    rightMotor->setSpeed(185);
    delay(190);
    flash = 0;
    digitalWrite(blue,0);
    leftMotor->setSpeed(0);
      rightMotor->setSpeed(0);
    delay(150);
    for(int i=80;i>0;i--){
    grabber.write(i);
    delay(10);
    }
    delay(250);
    curr = tar;
    if(is_red()){
      digitalWrite(red,1);
      digitalWrite(green,0);
      tar = 1;
    }
    else{
      digitalWrite(red,0);
      digitalWrite(green,1);
      tar = 2;
    }
    delay(5000);
    digitalWrite(red,0);
      digitalWrite(green,0);
    
    //tar = 1;
    get_path(curr,tar);
    flash = 1;

}
// int detect_hit(){
//   int hit = digitalRead(crashsensorpin);

// }
void release(){
  leftenable = true;
    rightenable = true;
  while(digitalRead(crashsensorpin)){
    blue_flash.tick();
        lineFollow();
    }
  grabber.write(80);
  flash = 0;
  digitalWrite(blue,0);
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
  delay(1500);
  
  //grabber.write(0);
  int temp = tar;
  tar = curr +1;
  if(tar>6){
    tar = 0;
  }
  curr = temp;
  get_path(curr,tar);
  flash = 1;

}
void turnRight() {
  //grabber.write(0);
  flash = 1;
  digitalWrite(blue,1);
  Serial.println("Turning right");
  leftMotor->run(leftback);
  rightMotor->run(rightback);
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(reversetime);

  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(100);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(80);
  delay(turntime);
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));
      pause = 1;
      flash = 0;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(leftlinesensorPin);
    if(digitalRead(leftlinesensorPin)){
      straightfor(500);
      break;
    }
  }
}
void turnLeft() {
  //grabber.write(0);
  flash = 1;
  Serial.println("Turning left");
  leftMotor->run(leftback);
  rightMotor->run(rightback);
  
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  delay(reversetime);

  leftMotor->run(leftback);
  rightMotor->run(rightfor);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(100);
  leftMotor->setSpeed(80);
  rightMotor->setSpeed(250);
  delay(turntime);
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));
      pause = 1;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(rightlinesensorPin);
    if(digitalRead(rightlinesensorPin)){
      straightfor(800);
      break;
    }
  }
}
void backturnright(){
  grabber.write(0);
  Serial.println("Back turn right");
  forward = true;
  leftMotor->run(leftfor);
  rightMotor->run(rightfor);
  flash = 1;
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(200);

  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  delay(700);
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
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
      straightfor(200);
      break;
      
    }
  }
}
void backturnleft(){
  grabber.write(0);
  Serial.println("Back turn left");
  forward = true;

  // drive slightly forward to avoid back wall
  flash = 1;
  leftMotor->setSpeed(180);
  rightMotor->setSpeed(250);
  leftMotor->run(leftfor);
  rightMotor->run(rightfor);
  delay(200);

  leftMotor->run(leftback);
  rightMotor->run(rightfor);
  delay(700);
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin));
      pause = 1;
      flash = 0;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    Serial.println(rightlinesensorPin);
    if(digitalRead(rightlinesensorPin)){
      straightfor(800);
      break;
    }
  }
}

void donextcommand(){
  // Read first command, then remove from the queue
  int next = commandlist.front();
  commandlist.erase(commandlist.begin());
  //int next = -4;
  
  if(next == -1){
      turnLeft();
    }
  else if(next == 0){
    flash = 1;
    //digitalWrite(blue,1);
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(180);
    delay(300);
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
  else if (next == -3) {
    ccw180();
  }
  else if (next == -4){
    grabber.write(80);
    delay(20);
    turnLeft();
    grab();
  }
  else if (next == 4){
    grabber.write(80);
    delay(20);
    turnRight();
    grab();
  }
  else if (next == 5){
    turnRight();
    release();
  }
  else if (next == -5){
    turnLeft();
    release();
  }
  else if(next == 15){
    flash = 1;
    //digitalWrite(blue,1);
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(255);
    delay(500);
    release();
  }
  else if (next == 16){
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(255);
    delay(900);
    leftMotor->setSpeed(0);
    rightMotor->setSpeed(0);
    pause = 1;
  }
  // if(commandlist.empty()){
  //   get_path(0,7);
  // }
}

void get_path(int currentPos, int target) {
  forward = false;  // In most cases we need to reverse after getting the path
  if (currentPos == 0 and target == 3) {
    forward = true;  // This is the only time we want to go forward after getting the path
    commandlist = {0, -1, 4};
  }
  
  else if (currentPos == 1 and target == 4) {
    commandlist = {3, -1, -4};
  }
  else if (currentPos == -1 and target == -1) {
    forward = true;
    commandlist = {-4};
  }
  else if (currentPos == 1 and target == 5) {
    commandlist = {3, 0, -4};
  }
  else if (currentPos == 1 and target == 6) {
    commandlist = {3, 0, 0, -1, 4};
  }

  else if (currentPos == 2 and target == 4) {
    commandlist = {-3, 1, 0, 4};
  }
  else if (currentPos == 2 and target == 5) {
    commandlist = {-3, 0, 0, 4};
  }
  else if (currentPos == 2 and target == 6) {
    commandlist = {-3, 1, -1, -4};
  }

  else if (currentPos == 3 and target == 1) {
    commandlist = {2, 0, 5};
  }
  else if (currentPos == 3 and target == 2) {
    commandlist = {-2, -5};
  }
  else if (currentPos == 4 and target == 1) {
    commandlist = {-2, 1, 15};
  }
  else if (currentPos == 4 and target == 2) {
    commandlist = {2, 0, -1, 15};
  }
  else if (currentPos == 5 and target == 1) {
    commandlist = {-2, 0, 15};
  }
  else if (currentPos == 5 and target == 2) {
    commandlist = {2, 0, 0, 15};
  }
  else if (currentPos == 6 and target == 1) {
    commandlist = {-2, -1, 0, 1, 15};
  }
  else if (currentPos == 6 and target == 2) {
    commandlist = {-2, 1, -1, 15};
  }
  else if (currentPos == 1) {
    commandlist = {2, -1,16};
  }
  else if (currentPos == 2) {
    commandlist = {-2, 0, 1,16};
  }
  else if (currentPos == 0 and target == 7) {
    forward = true;
    commandlist = {0, -1, 0, -1, 0, -1};
  }
}


void move() {
  // This function controls all the map navigation at a high level - follow the line, do a maneuvre, or stop.
  while (pause) {
      if (digitalRead(buttonPin)) {pause = 0;}
      delay(200);
    }
  if(digitalRead(buttonPin)){
    pause = 1;
    flash = 0;
    digitalWrite(blue,0);
    leftMotor->setSpeed(0);
    rightMotor->setSpeed(0);
    delay(500);
    return;
    
  }

  if (forward) {lineFollow();}
  else {backlineFollow();}

  if (commandlist.empty()) {
    // get_cube();
  }
  
  if(readfarleft() || readfarright()){
    delay(80);
    if(!readfarleft() && !readfarright()){return;}  // Returns to loop if it was just a speck of dust
    leftenable = false;
    rightenable = false;
    donextcommand();
    leftenable = true;
    rightenable = true;
  }
}

// get_cube() {
//   do {
//     ultrasounddistance = analogRead(ultrasoundPin) * 520/1023;
//     linefollow();
//   }
//   while (ultrasounddistance > 5);
//   delay(100);
//   if (analogRead(ultrasoundPin) * 520/1023 > 5) {get_cube();}
//   // grabber;
// -1 left, 0 straight, 1 right,-2 reverse left, 2 reverse right, -3 anticlockwise 180, 3 clockwise 180, -4 turn left and grab, 4 turn right and grab
// }

void setup() {
  tcs.begin();
  blue_flash.every(500, blueflash);

  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
    Serial.println("Adafruit Motorshield v2 - DC Motor test!");

    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
      while (1);
    }
    Serial.println("Motor Shield found.");
  pause = 1;
  // leftMotor->setSpeed(250);
  // rightMotor->setSpeed(230);
  // leftMotor->run(leftfor);
  // rightMotor->run(rightfor);
  forward = true;
  grabber.attach(9);
  grabber.write(0);
  curr = 0;
  tar = 3;
  get_path(curr, tar);
}

void loop() {
  blue_flash.tick();
  move();
}
