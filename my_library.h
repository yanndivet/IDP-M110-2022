#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H
#include <Arduino.h>
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


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor* myMotor_right = AFMS.getMotor(1);
Adafruit_DCMotor* myMotor_left = AFMS.getMotor(3); // warning this motor is reversed

void motor_setup() {
	// Set the speed to start, from 0 (off) to 255 (max speed)
	myMotor_right->setSpeed(150);
	myMotor_left->setSpeed(150);
	// turn on motor but won't move
	myMotor_right->run(RELEASE);
	myMotor_left->run(RELEASE);
}

void ultrasound_setup() {
// the chunk below is for ultrasonic sensor
// Clears the trigPin condition
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	// Sets the trigPin HIGH (ACTIVE) for 10 microseconds
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
}

void turn_left(double angle = 0.0) {
	// argument is the duration during which the robot turns left
	myMotor_right->run(FORWARD);
	myMotor_left->run(RELEASE); 
	delay(angle);
}

void turn_right(double angle = 0.0) {
	// argument is the duration during which the robot turns right
	myMotor_right->run(BACKWARD);
	myMotor_left->run(BACKWARD);
	delay(angle);
}

void move_forward(double distance = 0.0) {
	// argument is the duration during which the robot moves forward
	myMotor_right->run(BACKWARD);
	myMotor_left->run(BACKWARD);
	delay(distance);
}

void move_backward(double distance = 0.0) {
	// argument is the duration during which the robot moves backward
	myMotor_right->run(FORWARD);
	myMotor_left->run(BACKWARD);
	delay(distance);
}

double duration_to_distance_coef = 0.034 / 2;
double anal_to_voltage_coef = (5.0 / 1023.0);
#endif
