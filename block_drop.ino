// code turn left/right + move forward/backward functions

bool magnetic = true;
int counter_threshold = 0;
int right_counter = 0;

void setup(){}

int turn_left(double angle=0.0) {
//   myMotor_right->run(FORWARD);
//   myMotor_left->run(RELEASE); 
   delay(angle);
   return 0;
  }

int turn_right(double angle=0.0) {
//   myMotor_right->run(BACKWARD);
//   myMotor_left->run(BACKWARD);
   delay(angle);
   return 0;
  }

void move_forward(double distance=0.0) {
//   myMotor_right->run(FORWARD);
//   myMotor_left->run(BACKWARD);
   delay(distance);
  }

void move_backward(double distance=0.0) {
//   myMotor_right->run(BACKWARD);
//   myMotor_left->run(FORWARD);
   delay(distance);
  }

void loop() {
  // if block is magnetic, drop it in red region

  if (magnetic == true) {
    counter_threshold = 4;
    }

  else {
    counter_threshold = 6;
    }

  if (right_counter >= counter_threshold) {
    turn_right(90);
    move_forward(100);
    move_backward(100);
    turn_left(90);
    }

  // if dropped in red region, move in front of green region to start next mission
  // perhaps don't always run this, especially at the end? 
  if (magnetic = true) {
    move_forward(200);
    }

  // if block isn't magnetic, drop it in green region
  else {
    move_backward(200);
    }

}
