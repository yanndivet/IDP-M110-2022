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
int change_counter_RRS = 0;
int color_RRS = 1;         

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
  if(digitalRead(LS) && digitalRead(RS)) {
     move_forward();

    // For initial movement: use counter to determine when to turn right
    if ((digitalRead(RRS)) != color_RRS) {
      color_RRS = digitalRead(RRS);
      change_counter_RRS ++;
      Serial.println(change_counter_RRS);
      // Turn right at second intersection (when we detect four colour changes)
      if (change_counter_RRS == 4) {
        Serial.println("4 Colour Changes Detected");
        // Turn right using both wheels until right sensor reaches white line
        delay(1000);
        while (digitalRead(LS)) {
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
    if (hold_block == false){
      stop();
      delay(3000);
    }
    myservo.write(180); // move servo by 180 degrees for capture
    hold_block = true;
    // commented out below as electrical team automated LED process
    // magnetic = magnetic_detection(voltage);
    // if (magnetic == true) {
    //   digitalWrite(red, HIGH);
    // }
    // else {
    //   digitalWrite(green, HIGH);
    // }
  }

  if (hold_block == true) {
    block_drop(magnetic, change_counter_RRS);
  }
}
