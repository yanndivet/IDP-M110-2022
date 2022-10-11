#include <Adafruit_MotorShield.h>
#define LS 2      // left sensor

#define RS 3      // right sensor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(200);
  // turn on motor
  myMotor->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:


  myMotor_right->run(FORWARD);
  myMotor_left->run(FORWARD);
if(digitalRead(LS) && digitalRead(RS))     // Move Forward

  {

    
    myMotor_right->run(FORWARD);
    myMotor_left->run(FORWARD);

  }

  

  if(!(digitalRead(LS)) && digitalRead(RS))     // Turn right

  {

    myMotor_right->run(Release);
    myMotor_left->run(FORWARD);

  }

  

  if(digitalRead(LS) && !(digitalRead(RS)))     // turn left

  {

    myMotor_right->run(FORWARD);
    myMotor_left->run(RELEASE);

  }

  

  if(!(digitalRead(LS)) && !(digitalRead(RS)))     // move forward

  {

    myMotor_right->run(FORWARD);
    myMotor_left->run(FORWARD);

  }
}
