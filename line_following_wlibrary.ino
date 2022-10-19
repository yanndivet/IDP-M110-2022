#include <Adafruit_MotorShield.h>
#include <setup_basicmovement.h>

int threshold = 100;
int change_counter_LLS = 0;
int color_LLS = 1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pin_setup();
  motor_setup();
  Serial.println("Pins and motors are setup")
  
  initial_movement();
  Serial.println("Robot at starting position, ready for line_following")
}

void loop() {
  // put your main code here, to run repeatedly:

// Move forward
  if(digitalRead(LS) && digitalRead(RS)) {
    move_forward();
    Serial.println("Moved forward")
    }

// Turn right
  else if ((digitalRead(LS)) && !(digitalRead(RS))) {
    turn_right();
    Serial.println("Turned right")
  }

// Turn left
  else if (!(digitalRead(LS)) && (digitalRead(RS))) {
    turn_left();
    Serial.println("Turned left")
  }
}
