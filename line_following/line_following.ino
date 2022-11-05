#include <Adafruit_MotorShield.h>
#define LS 2      // left sensor
#define RS 3      // right sensor
#define LLS 1      // most left sensor
#define RRS 4      // most right sensor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed

int threshold = 100;
int change_counter_RRS = 0;
int color_RRS = 1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     

void setup() {
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
  myMotor_right->setSpeed(255);
  myMotor_left->setSpeed(255);
  // turn on motor
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:


// Move forward
  if(digitalRead(LS) && digitalRead(RS)) {
    myMotor_right->run(BACKWARD);
    myMotor_left->run(BACKWARD);

    // Check if this is the initial movement
    if ((digitalRead(RRS)) != color_RRS) {
      color_RRS = digitalRead(RRS);
      change_counter_RRS ++;
      Serial.println("Colour change");
      Serial.println(change_counter_RRS);
      // Turn right at second intersection
      if (change_counter_RRS == 3) {
        // Turn right using both wheels until right sensor reaches white line
        Serial.println("3 Colour Changes Detected");
        myMotor_right->run(BACKWARD);
        myMotor_left->run(BACKWARD);
        delay(1000);
        while (digitalRead(LS)) {
          Serial.println("Turn right");
          myMotor_right->run(FORWARD);
          myMotor_left->run(BACKWARD);
          
        }
      }
    }
  }

// Turn right
  else if ((digitalRead(LS)) && !(digitalRead(RS))) {
    myMotor_right->run(RELEASE);
    myMotor_left->run(BACKWARD);
  }

// Turn left
  else if (!(digitalRead(LS)) && (digitalRead(RS))) {
    myMotor_right->run(BACKWARD);
    myMotor_left->run(RELEASE);
  }
}
