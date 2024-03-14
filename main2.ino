#include <Adafruit_MotorShield.h>
#include <ArxContainer.h>
//#include <vector>
#include <arduino-timer.h>

auto blue_flash =  timer_create_default();
#include<Servo.h>
#define MAX_RANG (520)
#define ADC_SOLUTION (1023.0)
#define leftfor FORWARD // These are just defined as variables because we had issues with the motors being installed the wrong way etc and this saves going through and changing all the FORWARDs to BACKWARD and vice versa
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

int pause= 0; // Pause changes from 0 to 1 or vice versa every time the button is pressed. When pause = 1, the robot is halted until pause is 0. 
bool leftenable = true; // When leftenable is false, the left line sensor does not have any effect, and similar for rightenable
bool rightenable = true;
int forward = true; // When forward is set to false, instead of forwards line following occurring, the robot reverses.
int sensityPin = A3; // Ultrasound pin
Servo grabber;

// -1 left, 0 straight, 1 right,-2 reverse left, 2 reverse right, -3 anticlockwise 180, 3 clockwise 180, -4 turn left and grab, 4 turn right and grab, -5 turn left and release, 5 turn right and release, 15 straight and release, 16 return to base
std::vector<int> commandlist = {}; // Stores the list of junction instructions
std::vector<int> visited = {}; // Stores the stations we've already been to
int curr; // Current/most recent location of the robot
int tar; // Target location
int crashsensorpin = 12;

// Parameters
int reversetime = 150; // For turnright or turnleft
int turntime = 500; // 
int innerwheelturnspeed = 80; 
int forwardtime = 200; // For backturnright and backturnleft
int backturntime = 600;

int slowwheelspeed = 80; // During line following

int for_speed_180 = 170; // Speed of the wheel going forward during a 180
int rvs_speed_180 = 250; // Speed of the wheel going backward during a 180 - faster = tighter
int delay_180_straight = 300; // How long it keeps going after seeing the line
int delay_180_turn = 1000; // How long it turns for
int post_180_delay = 600; // How long it goes straight for when it sees the new line

int slowlinefollow_fast = 200; // These are the line following speeds when doing slowlinefollow
int slowlinefollow_slow = 100; //

void slowlinefollow();
int blueon = 0; // Stores whether the blue LED, which must flash at 2Hz when moving, is currently on
int flash = 0; // Stores whether the blue LED should currently be flashing
//Servo myservo;
void blueflash(){
  if (flash){
    if(blueon == 0){
    blueon = 1;
  }else{
    blueon = 0;
  }
  digitalWrite(blue,blueon);
  }

// Fairly unnecessary functions tbh
int readfarleft(){

  return digitalRead(farleftPin);
}
int readfarright(){
  return digitalRead(farrightPin);
}

// Only actually reads the left pin if leftenable = true
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

// Reads the colour sensor. Considering the relative magnitude of red compared to clear (all light) was found to be more consistent than 
// using the actual value for red when distinguishing between red and black blocks, because red and clear move together with angle of sensor.
bool is_red(){
  uint16_t clear, r, g, b;
  tcs.getRGBC(&r, &g, &b, &clear);
  Serial.println(r);
  if(r > 0.5 *clear){
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

// Used when reversing from the red pad and going to stations 4, 5 or 6
void clockwise180() {
  flash = 1;
  grabber.write(15); // Put the grabber down, as we've just reversed from the pad
  Serial.println("Clockwise 180");
  forward = true;
  delay(delay_180_straight); // Reversing for a short while after detecting the junction helps avoid detecting the junction again while turning
  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  leftMotor->setSpeed(for_speed_180);
  rightMotor->setSpeed(rvs_speed_180);
  delay(delay_180_turn);
  while(true){
    blue_flash.tick();
    if(pause == 1){
      return;
    }
    // Stop if the button is pressed
    if(digitalRead(buttonPin)){
      delay(50);
      Serial.print(digitalRead(buttonPin)); // Debouncing

      pause = 1;
      flash = 0;
      digitalWrite(blue,0);
      leftMotor->setSpeed(0);
    	rightMotor->setSpeed(0);
      break;
    }
    // Once the left line sensor pin sees the line, we can start line following again. Straightfor means it won't read the 
    // outer sensors for a period, so that it doesn't pick up the junction we've just turned at
    if(digitalRead(leftlinesensorPin)){
      flash = 1;
      straightfor(post_180_delay);
      break;
    }
  }
}

// Used when reversing from the green pad and going to stations 4, 5 or 6
void ccw180() {
  grabber.write(15);
  Serial.println("Clockwise 180");
  forward = true;
  delay(delay_180_straight);
  flash = 1;
  leftMotor->run(leftback);
  rightMotor->run(rightfor);
  leftMotor->setSpeed(rvs_speed_180);
  rightMotor->setSpeed(for_speed_180);
  delay(delay_180_turn);
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
      straightfor(post_180_delay);
      break;
    }
  }
}

// Not actually line following (name is just a remnant) - just reverses
void backlineFollow(){
  flash = 1;
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  leftMotor->run(leftback);
  rightMotor->run(rightback);
}

// The main line following code - just slows down the wheel of whichever side's line sensor triggers
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
      leftMotor->setSpeed(slowwheelspeed);
      rightMotor->setSpeed(255);
    }
    else if (valRight) {
      rightMotor->setSpeed(slowwheelspeed);
      leftMotor->setSpeed(255);
    }
    else {
      leftMotor->setSpeed(255);
      rightMotor->setSpeed(255);
    }
}

// Just line follows (while flashing the light), no reading of outer sensors 
void straightfor(int p){
  for(int i=0;i<p/15;i++){
    blue_flash.tick();
    leftenable = true;
    rightenable = true;
    lineFollow();
    delay(15);
  }
}

// Used when picking up a block - the normal line follow is sometimes too fast and it would hit the buildings
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
      leftMotor->setSpeed(slowlinefollow_slow);
      rightMotor->setSpeed(slowlinefollow_fast);
    }
    else if (valRight) {
      rightMotor->setSpeed(slowlinefollow_slow);
      leftMotor->setSpeed(slowlinefollow_fast);
    }
    else {
      leftMotor->setSpeed(slowlinefollow_fast);
      rightMotor->setSpeed(slowlinefollow_fast);
    }
}

int detectblock(){
  if(ultrasoundread()<3){
      return true;
  }
  return false;
}

// Grab is called after the last turn before a block. It calls slow line follow and flashes blue light until it detects the block
void grab(){
    visited.push_back(tar); // Add this station to the list of where we've been

    grabber.write(100);
    rightenable = true;
    leftenable = true;
    while(!detectblock()){
      blue_flash.tick();
      slowlinefollow();
    }
    // leftMotor->setSpeed(135);
    // rightMotor->setSpeed(135);
    //delay(20);
    flash = 0;
    digitalWrite(blue,0);
    leftMotor->setSpeed(0);
      rightMotor->setSpeed(0);
    delay(150);
    for(int i=80;i>15;i--){
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

    get_path(curr,tar);
    flash = 1;

}

// Called after passing the last junction before the red or green pad. Calls line follow until the crash sensor detects the pad, 
// then releases the block
void release(){
  leftenable = true;
  rightenable = true;
  while(digitalRead(crashsensorpin)){
    blue_flash.tick();
        lineFollow();
  }
  grabber.write(80); // Extending the grabber releases the block
  flash = 0;
  digitalWrite(blue,0);
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
  delay(1500);

  int here = tar;
  // We will always go to 3 first, defined in setup(). We should always go to 4 second.
  if (visited.size() == 1) {
    tar = 4; 
  }
  // Decision only needed if on the third block - if at red, go to 5; if at black, go to 6.
  else if (visited.size() == 2) {
    if (tar == 1) {
      tar = 5;
    }
    else if (tar == 2) {
      tar = 6;
    }
  }
  // Go to whichever one we didn't just visit
  else if (visited.size() == 3) {
    if (visited[2] == 5) {
      tar = 6;
    }
    else if (visited[2] == 6) {
      tar = 5;
    }
  }
  curr = here;
  flash = 1;
  get_path(curr, tar);

}
void turnRight() {
  //grabber.write(0);
  flash = 1;
  digitalWrite(blue,1);
  Serial.println("Turning right");
  leftMotor->run(leftback);
  rightMotor->run(rightback);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(reversetime);

  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250); // Set right motor to max reverse speed to jumpstart it
  delay(100);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(innerwheelturnspeed);
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
      straightfor(600);
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
  leftMotor->setSpeed(innerwheelturnspeed);
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
      straightfor(600);
      break;
    }
  }
}

// Used when reversing from a pad or block pick-up and turning right
void backturnright(){
  grabber.write(15);
  Serial.println("Back turn right");
  forward = true;
  leftMotor->run(leftfor);
  rightMotor->run(rightfor);
  flash = 1;
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
  delay(forwardtime);

  leftMotor->setSpeed(250);
  rightMotor->setSpeed(200);
  leftMotor->run(leftfor);
  rightMotor->run(rightback);
  delay(backturntime);
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
      straightfor(1000);
      break;
      
    }
  }
}
void backturnleft(){
  grabber.write(15);
  Serial.println("Back turn left");
  forward = true;

  // drive slightly forward to avoid back wall
  flash = 1;
  leftMotor->setSpeed(180);
  rightMotor->setSpeed(250);
  leftMotor->run(leftfor);
  rightMotor->run(rightfor);
  delay(forwardtime);

  leftMotor->run(leftback);
  rightMotor->run(rightfor);
  delay(backturntime);
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
      straightfor(1000);
      break;
    }
  }
}

// Called when we reach a junction. Calls the function pertaining to the number at the front of the command list.
void donextcommand(){
  // Read first command, then remove from the queue
  int next = commandlist.front();
  commandlist.erase(commandlist.begin());
    
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
    grabber.write(100);
    delay(20);
    turnLeft();
    grab();
  }
  else if (next == 4){
    grabber.write(100);
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
  // Go straight then release the block
  else if(next == 15){
    flash = 1;
    //digitalWrite(blue,1);
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(255);
    delay(500);
    release();
  }
  // At the very end when returning to start position, drive forward for a bit then stop.
  else if (next == 16){
    leftMotor->setSpeed(255);
    rightMotor->setSpeed(255);
    delay(900);
    leftMotor->setSpeed(0);
    rightMotor->setSpeed(0);
    pause = 1;
  }
}

// Sets commandlist based on the current location and the target station
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

// This function controls all the map navigation at a high level - follow the line, do a maneuvre, or stop.
void move() {
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
    delay(40);
    if(!readfarleft() && !readfarright()){return;}  // Returns to loop if it was just a speck of dust
    leftenable = false;
    rightenable = false;
    donextcommand();
    leftenable = true;
    rightenable = true;
  }
}


void setup() {
  tcs.begin();
  blue_flash.every(250, blueflash);

  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  pause = 1;
  forward = true;
  grabber.attach(9);
  grabber.write(0);
  curr = 0; // Initially we want to go from start position, 0, to station 3. 
  tar = 3;
  get_path(curr, tar);
}

void loop() {
  blue_flash.tick();
  move();
}
