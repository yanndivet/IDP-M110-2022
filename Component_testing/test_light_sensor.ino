/* 
 Demo code for using IR_sensors.

 Datasheet for TCRT5000
 *  
 *  
 *   
 */

#define IR A2

//uses A2 as in

void setup() {
  // put your setup code here, to run once:
  pinMode(IR, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("\n:");
  Serial.println(analogRead(IR));
  delay(1000);

}
