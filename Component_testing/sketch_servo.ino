#include <Servo.h>;

Servo myservo;

void setup() {
  myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo.write(0);			// Rotate to 0 degrees
  delay(1500);
  myservo.write(90);			// Rotate to 90 degrees
  delay(1500);
  myservo.write(180);			// Rotate to 180 degrees
  delay(1500);

}
