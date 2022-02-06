#include <Servo.h>

Servo servo1;
Servo servo2;

// two servos powered by PWM
// but also they don't get power until their MOSFET is activated

const int SERVO_1_PWM_PIN = 9;
const int SERVO_2_PWM_PIN = 10;
const int SERVO_POWER_PIN = 12;

void setup() {
  //pinMode(SERVO_1_PWM_PIN, OUTPUT);
  //pinMode(SERVO_2_PWM_PIN, OUTPUT);
  servo1.attach(SERVO_1_PWM_PIN);
  servo2.attach(SERVO_2_PWM_PIN);
  pinMode(SERVO_POWER_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Booting");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Waiting 5s before powering servos");
  delay(5000);
  
  Serial.println("Powering on servos, waiting 5s");
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(5000);

  Serial.println("Rotating servo1");
  rotateServoUp(servo1, 0, 179);
  Serial.println("Servo1 rotation complete");
  delay(1000);
  rotateServoUp(servo2, 0, 179);
  Serial.println("Servo2 rotation complete");
  
  delay(5000);
  Serial.println("Rotating servos back");

  Serial.println("Rotating servo1");
  rotateServoDown(servo1, 179, 0);
  Serial.println("Servo1 rotation complete");
  delay(1000);
  rotateServoDown(servo2, 179, 0);
  Serial.println("Servo2 rotation complete");
  delay(5000);

  Serial.println("Powering down servos");
  digitalWrite(SERVO_POWER_PIN, LOW);
  delay(5000);
  Serial.println("Starting over");
}

void rotateServoUp(Servo &theServo, int startAngle, int endAngle) {
  // https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/arduino-code-for-sweep
  startAngle = max(startAngle, 0);
  endAngle = min(endAngle, 179);
  for(int angle = startAngle; angle <= endAngle; angle++)  
  {                                  
    theServo.write(angle);               
    delay(15);                   
  }
}

void rotateServoDown(Servo &theServo, int startAngle, int endAngle) {
  // https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/arduino-code-for-sweep
  startAngle = min(startAngle, 179);
  endAngle = max(endAngle, 0);
  for(int angle = startAngle; angle >= endAngle; angle--)  
  {                                  
    theServo.write(angle);               
    delay(15);                   
  }
}
