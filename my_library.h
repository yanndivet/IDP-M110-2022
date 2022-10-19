#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H
#include <Arduino.h>
#include <Adafruit_MotorShield.h>


// Define all analog sensor ports used
#define LS A2      // left sensor
#define RS A1      // right sensor
#define LLS A3      // most left sensor
#define RRS A4      // most right sensor

void pin_setup() {
	// Define the ports used as inputs
	pinMode(LS, INPUT);
	pinMode(RS, INPUT);
	pinMode(LLS, INPUT);
	pinMode(RRS, INPUT);
}


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

void turn_left(double angle = 0.0) {
	myMotor_right->run(FORWARD);
	myMotor_left->run(RELEASE); 
	delay(angle);
}

void turn_right(double angle = 0.0) {
	myMotor_right->run(BACKWARD);
	myMotor_left->run(BACKWARD);
	delay(angle);
}

void move_forward(double distance = 0.0) {
	myMotor_right->run(BACKWARD);
	myMotor_left->run(BACKWARD);
	delay(distance);
}

void move_backward(double distance = 0.0) {
	myMotor_right->run(FORWARD);
	myMotor_left->run(BACKWARD);
	delay(distance);
}

void initial_movement() {
	// Moves robot from white box to the correct direction on the line
	move_forward(3000);
	turn_right(3000);
}
#endif
