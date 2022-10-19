#include <Adafruit_MotorShield.h>
#include <setup_basicmovement.h>

int threshold = 100;
int change_counter_LLS = 0;
int color_LLS = 1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pin_setup();
  motor_setup();
}

void loop() {
  // put your main code here, to run repeatedly:


// Move forward
  if(digitalRead(LS) && digitalRead(RS)) {
    move_forward();

    // Check if this is the initial movement
    if ((digitalRead(LLS)) != color_LLS) {
      color_LLS = digitalRead(LLS);
      change_counter_LLS ++;
      // Turn right at second intersection
      if (change_counter_LLS == 3) {
        // Turn right (hardcoded) to go over the white line with the left sensor
        turn_right();
        delay(1000);
        while (digitalRead(LS)) { // turn right until right sensor reaches white line
          turn_right();
        }
      }
    }
  }

// Turn right
  else if ((digitalRead(LS)) && !(digitalRead(RS))) {
    turn_right();
  }

// Turn left
  else if (!(digitalRead(LS)) && (digitalRead(RS))) {
    turn_left();
  }
}
