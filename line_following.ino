#include <Adafruit_MotorShield.h>

#define LLS 2      // most left sensor
#define LS 3      // left sensor
#define RS 4      // right sensor
#define RRS 5      // most right sensor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(2);


void setup() {
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  int right_counter = 0;
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor_right->setSpeed(200);
  myMotor_left->setSpeed(200);
  // turn on motor
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:

// For efficiency, we should rank each operation by most frequent use.

// Move Forward
if(!(digitalRead(LS)) && !(digitalRead(RS))) {  
    myMotor_right->run(FORWARD);
    myMotor_left->run(FORWARD);

    // Increment counter when we reach the drop off zones
    if(!(digitalRead(LLS)) && (digitalRead(RRS))) {  
    right_counter ++;
    } 
  } 


// Turn right
else if(!(digitalRead(LS)) && digitalRead(RS)){ 
    myMotor_right->run(RELEASE);
    myMotor_left->run(FORWARD);
  }


// Turn left
else if(digitalRead(LS) && !(digitalRead(RS))){ 
    myMotor_right->run(RELEASE);
    myMotor_left->run(FORWARD);
  }  


}
