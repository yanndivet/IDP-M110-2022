#include <Adafruit_MotorShield.h>
#include <Servo.h>;
#include <Adafruit_MotorShield.h>
#define HS 0      // hall sensor
#define LLS 1      // most left sensor
#define LS 2      // left sensor
#define RS 3      // right sensor
#define RRS 4      // most right sensor
#define trigPin 13
#define echoPin 12
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(1); // warning this motor is reversed

int threshold = 100;
int change_counter_RRS = 0;
int color_RRS = 1;

// LED lights
int red = 5;
int green = 6;

bool hold_block = false;
bool magnetic = false;

// below for ultrasonic sensor
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
Servo myservo;

void setup() {
  // Define the ports used as inputs
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(LLS, INPUT);
  pinMode(RRS, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  myservo.attach(9);


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
  // turn on motor but won't move
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE);
}

void loop() {
  // Put your main code here, to run repeatedly:


// Move forward
  if(digitalRead(LS) && digitalRead(RS)) {
     myMotor_right->run(BACKWARD);
     myMotor_left->run(BACKWARD);

    // For initial movement: use counter to determine when to turn right
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
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)


  // read the value from hall effect sensor:
  int hall_sensor = digitalRead(HS);
  
  Serial.println(distance);
  Serial.println("cm");

  if (distance < 6){
    // add a grip function
    myMotor_right->run(RELEASE);
    myMotor_left->run(RELEASE);
    hold_block = true;
    if (!(hall_sensor)){
      magnetic = true;
      delay(6000);
      digitalWrite(red, HIGH);
      
    }

    else if (hall_sensor){
      magnetic = false;
      delay(6000);
      digitalWrite(green, HIGH);
      
    }

    myservo.write(180);
  }




  else if (distance > 6){
    hold_block = false;
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
  }

}
