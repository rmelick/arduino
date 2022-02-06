#include <Servo.h>

Servo myServo;

int const POT_PIN = A0;
int const SERVO_PIN = 9;

int potVal;
int angle;

void setup() {
  myServo.attach(SERVO_PIN);

  Serial.begin(9600);
}

void loop() {
  potVal = analogRead(POT_PIN);
  Serial.print("potVal: ");
  Serial.print(potVal);
  angle = map(potVal, 0, 1023, 0, 179);
  Serial.print(". angle: ");
  Serial.println(angle);

  myServo.write(angle);
  delay(15); // let servo move to new position
}
