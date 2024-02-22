#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

int leftlinesensorPin = 6;
int rightlinesensorPin = 4;
int farleftPin = 5;
int farrightPin = 7;


void lineFollow() {
  while (true) {
    int valLeft = digitalRead(leftlinesensorPin); // read left input value
    Serial.print(valLeft);
    int valRight = digitalRead(rightlinesensorPin); // read right input value
    Serial.println(valRight);

    if (valLeft) {
      leftMotor->setSpeed(50);
      rightMotor->setSpeed(250);
    }
    else if (valRight) {
      rightMotor->setSpeed(50);
      leftMotor->setSpeed(250);
    }
    else {
      leftMotor->setSpeed(250);
      rightMotor->setSpeed(200);
    }

    delay(50);

    if (digitalRead(farleftPin) /*or digitalRead(farrightPin)*/) {
      break;
    }
  }
}


void turnLeft() {
  delay(500);
  leftMotor->run(FORWARD);
  delay(700);
  int val = 0;
  while(!val) {
    val = digitalRead(leftlinesensorPin);
  }
}

void turnLeft2() {
  delay(600);
  //stage 1, far triggered, stage 2 near triggered
  int stage = 1;
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(250);
  while(stage != 2){
    if(stage ==0){
      if(digitalRead(farleftPin)){
        stage =1;
      }
    }
    else{
      if(digitalRead(leftlinesensorPin)){
        leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(250);
  rightMotor->setSpeed(250);
        stage =2;
        break;
      }
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
}

void loop() {
  lineFollow();
  delay(500);
  lineFollow();
  turnLeft2();
  lineFollow();
  turnLeft2();
  lineFollow();
  turnLeft2();
  
  
  

}
