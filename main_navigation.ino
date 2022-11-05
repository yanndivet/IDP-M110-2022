#include <Servo.h>
#include <Adafruit_MotorShield.h>

// ------------------ VARIABLE DEFINITIONS ------------------ 

// Define all pins
#define HS A0      // hall sensor
#define RS 1      // right sensor
#define LS 2      // left sensor
#define echoPin 5
#define trigPin 6
#define button 7
#define echoPin2 8
#define trigPin2 9

// Define pins for LED lights
int red = 12;
int green = 13;
int flash = 11;

// Variables required for button to start running program
int trigger = 0;
bool start = false;
int button_counter = 0;

// Connect motors to according ports
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed
int motor_speed = 255;   // Set the speed to start, from 0 (off) to 255 (max speed)

// Define threshold variables
int distance_threshold = 5;
double hall_threshold = 1.0;
int tunnel_distance;
int tunnel_threshold = 10;

// Define coefficients
double duration_to_distance = 0.034 / 2;
double hs_to_voltage = 5.0 / 1023.0;

// Define time variables
int illuminate_time = 6000;
int initial_movement_time = 3700;
int right_angle_time = 1020;
int waiting_time = 5*60*1000;
int back_to_initial_time = 3500;
int greenregion_forward_time = 5500;
int redregion_forward_time = 9000;
int nudge_time = 80;
int forward_aft_tunnel_time = 12000;
int small_wait_time = 1000;

// Define other boolean variables
bool hold_block = false;
bool magnetic = false;
bool nudge = true;

// Ultrasonic sensor variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
long duration2; // variable for the duration of sound wave travel (ultrasonic on the side)
int distance2; // variable for the distance measurement (ultrasonic on the side)

// Servo motor initialisation
Servo myservo;
int servo_initial = -180;
int servo_collect = 110;


void setup() {
  // Define the ports used as inputs and outputs
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(flash, OUTPUT);
  myservo.attach(10);
  pinMode(button, INPUT);


  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz (change argument for different frequency)
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed of robot
  myMotor_right->setSpeed(motor_speed);
  myMotor_left->setSpeed(motor_speed);
  wait(); // turn on motor but don't move
  myservo.write(servo_initial); // get servo to appropriate position before running main loop
}

void loop() {
  // Put your main code here, to run repeatedly:

  // ------------------ INITIAL MOVEMENT CODE ------------------ 
  trigger = digitalRead(button);

  if (trigger == 0 && !(start)) {
    start = false;
  }

  // Only start the program when button is pressed
  else {
    start = true;
    button_counter ++;
  }

  if (start == true){
    digitalWrite(flash, HIGH);

    // If button is clicked, start code with initial, hard coded forward movement
    if (button_counter == 1) {
      forward();
      delay(initial_movement_time);
    }

  // ------------------ LINE FOLLOWING CODE ------------------ 
  // Move forward
    if(digitalRead(LS) && digitalRead(RS)) {
      forward();
    }

  // Turn right
    else if ((digitalRead(LS)) && !(digitalRead(RS))) {
      right();
    }

  // Turn left
    else if (!(digitalRead(LS)) && (digitalRead(RS))) {
      left();
    }

    else if (!(digitalRead(LS)) && !(digitalRead(RS))) {
      forward();
    }



  // ------------------ ULTRASONIC 1 CODE ------------------ 
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance in centimeter
    distance = duration * duration_to_distance; // Speed of sound wave divided by 2 (go and back)


  // ------------------ BLOCK COLLECTION CODE ------------------ 
    if ((distance < distance_threshold) && !(hold_block)) {
      digitalWrite(flash, LOW);
      wait();
      int hall_sensor = analogRead(HS);
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = hall_sensor * hs_to_voltage;
      Serial.println(voltage);

      // When block is magnetic, wait 6s and then red LED will turn on
      if ((hall_sensor) > hall_threshold){
        magnetic = true;
        digitalWrite(red, HIGH);
        delay(illuminate_time);
        digitalWrite(red, LOW);
      }

      // When block isn't magnetic, wait 6s and then green LED will turn on
      else {
        magnetic = false;
        digitalWrite(green, HIGH);
        delay(illuminate_time);
        digitalWrite(green, LOW);
      }

      // Activate servo to collect block
      myservo.write(servo_collect);
      delay(small_wait_time);

      hold_block = true;
      forward();
      delay(small_wait_time);
    }

  // ------------------ ULTRASONIC 2 CODE ------------------ 
    // When holding block, use second ultrasonic sensor on the side to detect whether the robot is in the tunnel or not

    if (hold_block) {
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration2 = pulseIn(echoPin2, HIGH);
    // Calculating the distance in centimeter
    distance2 = duration2 * duration_to_distance; // Speed of sound wave divided by 2 (go and back)
    Serial.println("cm");
    tunnel_distance = distance2;
    }

  // ------------------ TUNNEL CODE ------------------ 
    if ((tunnel_distance < tunnel_threshold) && nudge) {
      left();
      delay(nudge_time);
      forward();
      nudge = false;
      delay(forward_aft_tunnel_time);
      block_drop(magnetic);
    }
  }
}

// ---------------------- MOVEMENT FUNCTION DEFINITIONS ----------------------

void wait() {
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE); }

void forward () {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(BACKWARD); }

void left() {
  myMotor_right->run(BACKWARD);
  myMotor_left->run(FORWARD); }

void right() {
  myMotor_right->run(FORWARD);
  myMotor_left->run(BACKWARD); }

void backward() {
  myMotor_right->run(FORWARD);
  myMotor_left->run(FORWARD); 
}

void block_drop(bool magnetic) {
    // if block is magnetic, drop it in red region
    // if block isn't magnetic, drop it in green region

  left();
  delay(right_angle_time);
  if (magnetic == true) {
    // Get inside red region
    forward();
    delay(redregion_forward_time);
  }

  else {
    // Get to other side of block then get inside green region
    forward();
    delay(greenregion_forward_time);
    right();
    delay(2*right_angle_time);
    forward();
    delay(greenregion_forward_time);
  }
  // drop block
  myservo.write(servo_initial);
  hold_block = false;

    // Get back to initial region
  backward();
  delay(back_to_initial_time);

  // Reset robot
  wait();
  start = false;
  digitalWrite(flash, LOW);
  delay(waiting_time);
  
}



