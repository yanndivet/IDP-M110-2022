#include <Adafruit_MotorShield.h>
#include <my_library.h>
#include <movement_library.h>

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

	// Define the ports used as inputs
	pinMode(LS, INPUT);
	pinMode(RS, INPUT);
	pinMode(LLS, INPUT);
	pinMode(RRS, INPUT);

  motor_setup();

  // Hardcode initial movement
  initial_movement();
  Serial.println("setups done");
}

void loop() {
  // Put your main code here, to run repeatedly:
// ---------------------------------- LINE FOLLOWING ----------------------------------
// Move forward
  if(digitalRead(LS) && digitalRead(RS)) {
     move_forward();
     Serial.println("move forward");
  }

// Turn left 
  else if(!(digitalRead(LS)) && digitalRead(RS)) {
    turn_left();
    Serial.println("turn left");
  }

// Turn right
  else if(digitalRead(LS) && !(digitalRead(RS))) {
    turn_right();
    Serial.println("turn right");
  }
}
