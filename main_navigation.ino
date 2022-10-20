#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <my_library.h>
#include <movement_library.h>

// LED lights
int red = 13;
int green = 12;

int threshold = 100;
int right_counter = 0;
bool hold_block = false;
bool magnetic;
double distance_thresh = 5.0;

Servo myservo;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

	// Define the ports used as inputs
	pinMode(LS, INPUT);
	pinMode(RS, INPUT);
	pinMode(LLS, INPUT);
	pinMode(RRS, INPUT);
	pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
	pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
	pinMode(red, OUTPUT);
	pinMode(green, OUTPUT);

  motor_setup();

  // Turn off both LEDs
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);

  // Servo connected to pin 9
  myservo.attach(9);
}

void loop() {
  // Put your main code here, to run repeatedly:
// ---------------------------------- LINE FOLLOWING ----------------------------------
// Move forward
  if((analogRead(LS) < threshold) && (analogRead(RS) < threshold)) {
    move_forward();

    // Checks if most right sensor is over white. Could also include most left sensor to make sure, but not necessary
    if ((analogRead(RS) > threshold)) {
      right_counter++;
      // Turn right at second intersection
      if (right_counter == 2) {
        // Turn right using both wheels until right sensor reaches white line
        while (analogRead(RS) < threshold) {
          turn_right();
          }
        }
      }
  }

// Turn left 
  else if((analogRead(LS) < threshold) && (analogRead(RS) > threshold)) {
    turn_left();
  }

// Turn right
  else if((analogRead(LS) > threshold) && (analogRead(RS) < threshold)) {
    turn_right();
  }


// ---------------------------------- ULTRASOUND AND HALL-EFFECT SENSORS ----------------------------------
  // for efficiency, maybe we can only start checking distance from ultrasensor after a certain amount of time? 
  ultrasound_setup();
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance in centimeter
  double distance = duration * duration_to_distance_coef; // Speed of sound wave divided by 2 (go and back)


  // read the value from hall effect sensor:
  int sensorValue = analogRead(HS);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  double voltage = sensorValue * anal_to_voltage_coef;


// ---------------------------------- BLOCK COLLECTION AND RETURN ----------------------------------
  if (distance < distance_thresh) {
    hold_block = true;
    myservo.write(180); // move servo by 180 degrees
    magnetic = magnetic_detection(voltage);
    if (magnetic == true) {
      digitalWrite(red, HIGH);
    }
    else {
      digitalWrite(green, HIGH);
    }
  }

  if (hold_block == true) {
    block_drop(magnetic, right_counter);
  }
}
