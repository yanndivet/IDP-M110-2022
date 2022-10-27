#include <Adafruit_MotorShield.h>


// Connect motors to according ports
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed


int mov_time;
void wait(int mov_time = 0);
void forward(int mov_time = 0);
void backward(int mov_time = 0);
void left(int mov_time = 0);
void right(int mov_time = 0);


int motor_speed = 255;
int time_right_angle = 900;
int time_back_from_block = 300;
int time_change_squares = 3000;
int time_stop = 5*60*1000;
int time_forward_dropblock = 1000; // needs to be tested to see if accurate


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
  // block_drop(magnetic, counter_RRSorsmth);
  return_robot(magnetic);

}

// ---------------------- FUNCTIONS DEFINITIONS ----------------------

void wait(int mov_time) {
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE); 
  delay(mov_time); }

void forward (int mov_time) {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(BACKWARD);
  delay(mov_time); }

void backward (int mov_time) {
  myMotor_right->run(FORWARD);
  myMotor_left->run(FORWARD); 
  delay(mov_time); }

void left (int mov_time) {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(FORWARD); 
  delay(mov_time); }

void right(int mov_time) {
  myMotor_right->run(FORWARD);
  myMotor_left->run(BACKWARD); 
  delay(mov_time); }

// ---------------------- MORE SOPHISTICATED MOVEMENTS ----------------------
void block_drop(bool magnetic, int right_counter) {
    // if block is magnetic, drop it in red region
    int counter_turn;

    if (magnetic == true) {
      counter_turn = 1;
    }
    else {
      counter_turn = 5;
    }

    if (right_counter == counter_turn) {
      // Get inside square region
      right(time_right_angle);
      forward(time_forward_dropblock);

      // drop block
      // myservo.write(angle_backward);
      hold_block = false;

      // get out of function, we don't want this repeating
      right_counter ++;

    }
}

void return_robot(bool magnetic) {

  if (!(magnetic)){
    backward(time_back_from_block);
    left(time_right_angle);
    forward(time_change_squares);
    wait(time_stop);
  }



  else if (magnetic){
    backward(time_back_from_block); 
    right(time_right_angle);
    forward(time_change_squares);
    wait(time_stop);
  }
}
