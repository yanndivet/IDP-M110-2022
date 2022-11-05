#include <Servo.h>;
#include <Adafruit_MotorShield.h>

// Define all pins
#define HS A0      // hall sensor
#define LS 3      // left sensor
#define RS 1      // right sensor
#define echoPin 5
#define trigPin 6
#define echoPin2 8
#define trigPin2 9
#define button 7
// LED lights
int red = 12;
int green = 13;
int flash = 11;

int trigger = 0;
bool start = 0;

int button_counter = 0;

// Connect motors to according ports
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed

// Define variables
int threshold = 100;
int change_counter_RRS = 0;
int color_RRS = 1;
int motor_speed = 255;   // Set the speed to start, from 0 (off) to 255 (max speed)
double duration_to_distance = 0.034 / 2;
int distance_threshold = 5;
int illuminate_time = 6000;
double hall_threshold = 0.83;

int block_hold_time;

bool hold_block = false;
bool magnetic = false;
int tunnel_distance = 100;
bool nudge = true;

// below for ultrasonic sensor
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
long duration2; // variable for the duration of sound wave travel (ultrasonic on the side)
int distance2; // variable for the distance measurement (ultrasonic on the side)

Servo myservo;


void setup() {
  // Define the ports used as inputs
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
  myservo.write(-180);
}

void loop() {
  // Put your main code here, to run repeatedly:
  trigger = digitalRead(button);
  Serial.println(trigger);
  if (trigger == 0 && (button_counter == 0)){
    start = false;
  }

  else{
    start = true;
    button_counter ++;
  }

  if (start == true){
    // digitalWrite(flash, HIGH);

    if (button_counter == 1){
      forward();
      delay(4000);
      Serial.println("Done with initial movement");
    }
  // Move forward
    if(digitalRead(LS) && digitalRead(RS)) {
      forward();
      Serial.println("Moved forward");
      Serial.println(digitalRead(LS));
      Serial.println(digitalRead(RS));

      // For initial movement: use counter to determine when to turn right
      // if ((digitalRead(RRS)) != color_RRS) {
      //   color_RRS = digitalRead(RRS);
      //   change_counter_RRS ++;
      //   Serial.println("Colour change");
      //   Serial.println(change_counter_RRS);
      //   // Turn right at second intersection
      //   if (change_counter_RRS == 3) {
      //     // Turn right using both wheels until right sensor reaches white line
      //     Serial.println("3 Colour Changes Detected");
      //     forward();
      //     delay(1000);
      //     while (digitalRead(LS)) {
      //       Serial.println("Turn right");
      //       right();
            
      //     }
      //   }
      // }
    }
    

  // Turn right
    else if ((digitalRead(LS)) && !(digitalRead(RS)) && (change_counter_RRS > 0)) {
      right();
      Serial.println("Turned right");
    }

  // Turn left
    else if (!(digitalRead(LS)) && (digitalRead(RS))) {
      left();
      Serial.println("Turned left");
    }

    else if (!(digitalRead(LS)) && !(digitalRead(RS))) {
      forward();
      Serial.println("Moved forward");
    }



    // the chunk below is for ultrasonic sensor
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



    // Idea: read this value only once (inside loop when distance close enough)
    // read the value from hall effect sensor:


    if ((distance < distance_threshold) && !(hold_block)) {

      
      // add a grip function
      digitalWrite(flash, LOW);
      wait();
      int hall_sensor = analogRead(HS);
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = hall_sensor * (5.0 / 1023.0);
      Serial.println(voltage);

      if ((hall_sensor) > hall_threshold){
        magnetic = true;
        digitalWrite(red, HIGH);
        delay(illuminate_time);
        digitalWrite(red, LOW);
        
      }

      else {
        magnetic = false;
        digitalWrite(green, HIGH);
        delay(illuminate_time);
        digitalWrite(green, LOW);
      }


      

      myservo.write(110);
      delay(1000);

      hold_block = true;
      forward();
      delay(1000);
      block_hold_time = millis();
    }

    if (hold_block){
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

    if ((tunnel_distance < 10) && nudge){
      left();
      delay(60);
      forward();
      nudge = false;
    }

    int current_time = millis();
    int distance_since_collect = current_time - block_hold_time;
    if ((distance_since_collect > 12000) && hold_block) {
      forward();
      delay(8000);
      Serial.println("Should be stuck to wall");
      left();
      delay(900);
      Serial.println("Turned left");
      forward();
      delay(10000);
      Serial.println("In red zone");
      myservo.write(-180);
      backward();
      delay(5000);
      wait();
      Serial.println("Arrived in white zone. Program will now stop");
      delay(1000000);
    }
  }

  

}

// ---------------------- FUNCTIONS DEFINITIONS ----------------------

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

