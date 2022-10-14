// code turn left/right + move forward/backward functions

bool magnetic = true;

void turn_left(angle) {
  
  }

void turn_right(angle) {
  
  }

void move_forward(distance) {
  
  }

void move_backward(distance) {
  
  }

void block_drop {
  // if block is magnetic, drop it in red region

  if (magnetic == true) {
    counter_threshold = 4
    }

  else {
    counter_threshold = 6
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
    move_straight(200);
    }

  


  // if block isn't magnetic, drop it in green region

  

}
