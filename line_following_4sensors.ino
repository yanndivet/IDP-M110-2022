#include <Adafruit_MotorShield.h>
// Define all analog sensor ports used
#define LS A2      // left sensor
#define RS A1      // right sensor
#define LLS A3      // most left sensor
#define RRS A4      // most right sensor

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(3); // warning this motor is reversed

int threshold = 100;
int right_counter = 0;

void setup() {
  // Define the ports used as inputs
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(LLS, INPUT);
  pinMode(RRS, INPUT);

  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor_right->setSpeed(150);
  myMotor_left->setSpeed(150);
  // turn on motor but won't move
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE);
}

void loop() {
  // Put your main code here, to run repeatedly:


// Move forward
  if((analogRead(LS) < threshold) && (analogRead(RS) < threshold)) {
    myMotor_right->run(FORWARD);
    myMotor_left->run(BACKWARD);

    // Checks if most right sensor is over white. Could also include most left sensor to make sure, but not necessary
    if ((analogRead(RRS) > threshold)) {
      right_counter++;
      // Turn right at second intersection
      if (right_counter == 2) {
        // Turn right using both wheels until right sensor reaches white line
        while (analogRead(RS) < threshold) {
          myMotor_right->run(BACKWARD);
          myMotor_left->run(BACKWARD);
          }
        }
      }
  }

// Turn left 
  else if((analogRead(LS) < threshold) && (analogRead(RS) > threshold)) {
    myMotor_right->run(FORWARD);
    myMotor_left->run(RELEASE);
  }

// Turn right
  else if((analogRead(LS) > threshold) && (analogRead(RS) < threshold)) {
    myMotor_right->run(RELEASE);
    myMotor_left->run(BACKWARD);
  }
}
