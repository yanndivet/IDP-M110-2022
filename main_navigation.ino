#include <Adafruit_MotorShield.h>
// Define all analog sensor ports used
#define LS A2      // left sensor
#define RS A1      // right sensor
#define LLS A3      // most left sensor
#define RRS A4      // most right sensor
#define HS A0      // hall sensor
// below for ultrasonic sensor
#define echoPin 2 // attach pin D2 
#define trigPin 3 //attach pin D3

// LED lights
int red = 13;
int green = 12;




Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor_right = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor_left = AFMS.getMotor(3); // warning this motor is reversed

int threshold = 100;
int right_counter = 0;
bool hold_block = false;
bool magnetic = false;

// below for ultrasonic sensor
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

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

  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor_right->setSpeed(150);
  myMotor_left->setSpeed(150);
  // turn on motor but won't move
  myMotor_right->run(RELEASE);
  myMotor_left->run(RELEASE);
}

void loop() {
  // Put your main code here, to run repeatedly:


// Move forward
  if((analogRead(LS) < threshold) && (analogRead(RS) < threshold)) {
    myMotor_right->run(FORWARD);
    myMotor_left->run(BACKWARD);

    // Checks if most right sensor is over white. Could also include most left sensor to make sure, but not necessary
    if ((analogRead(RS) > threshold)) {
      right_counter++;
      // Turn right at second intersection
      if (right_counter == 2) {
        // Turn right using both wheels until right sensor reaches white line
        while (analogRead(RS) < threshold) {
          myMotor_right->run(BACKWARD);
          myMotor_left->run(BACKWARD);
          }
        }
      }
  }

// Turn left 
  else if((analogRead(LS) < threshold) && (analogRead(RS) > threshold)) {
    myMotor_right->run(FORWARD);
    myMotor_left->run(RELEASE);
  }

// Turn right
  else if((analogRead(LS) > threshold) && (analogRead(RS) < threshold)) {
    myMotor_right->run(RELEASE);
    myMotor_left->run(BACKWARD);
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
  int sensorValue = analogRead(HS);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);

  if (distance < 5){
    // add a grip function
    hold_block = true;
    if (voltage > 2.4){
      magnetic = true;
      digitalWrite(red, HIGH);
    }

    else if (voltage < 2.4){
      magnetic = false;
      digitalWrite(green, HIGH);
    }
  }


  else if (distance > 5){
    hold_block = false;
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
  }

}