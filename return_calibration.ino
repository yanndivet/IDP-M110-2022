#include <Adafruit_MotorShield.h>


// Connect motors to according ports
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed

int motor_speed = 255;



bool hold_block = false;
bool magnetic = false;






void setup() {
  // Define the ports used as inputs



  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor_right->setSpeed(motor_speed);
  myMotor_left->setSpeed(motor_speed);
  // turn on motor but don't move
  wait();

}


void loop() {
  // put your main code here, to run repeatedly:
  magnetic = false;
  return_robot(magnetic);

}

// ---------------------- FUNCTIONS DEFINITIONS ----------------------

void wait() {
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE); 
}

void forward () {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(BACKWARD); 
}

void left() {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(FORWARD); 
}

void right() {
  myMotor_right->run(FORWARD);
  myMotor_left->run(BACKWARD); 
}

void backward() {
  myMotor_right->run(FORWARD);
  myMotor_left->run(FORWARD); 
}

// ---------------------- MORE SOPHISTICATED MOVEMENTS ----------------------
// void block_drop(bool magnetic, int right_counter) {
//     // if block is magnetic, drop it in red region
//     int counter_turn;
//     // these counter values have to be adjusted experimentally.
//     if (magnetic == true) {
//       counter_turn = 11;
//     }
//     else {
//       counter_turn = 15;
//     }

//     if (right_counter == counter_turn) {
//       // Get inside square region
//       right(1000);
//       right_till_line();
//       forward(3000);

//       // drop block
//       myservo.write(angle_backward);
//       hold_block = false;

//       // Get outside square region and back on line
//       backward(3000);
//       left(1000);
//       right_till_line();
//     }
// }

void return_robot(bool magnetic) {

  if (!(magnetic)){
    backward();
    delay(300);
    left();
    delay(900);
    forward();
    delay(3000);
    wait();
    delay(100000);
  }



  else if (magnetic){
    backward();
    delay(300);  
    right();
    delay(900);
    forward();
    delay(3000);
    wait();
    delay(100000);
  }
}
