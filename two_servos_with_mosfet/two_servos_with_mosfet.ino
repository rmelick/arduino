#include <Servo.h>

Servo servo1;
Servo servo2;

// two servos powered by PWM
// but also they don't get power until their MOSFET is activated

const int SERVO_1_PWM_PIN = 3;
const int SERVO_2_PWM_PIN = 5;
const int SERVO_POWER_PIN = 7;

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
  Serial.println("Waiting 2s before powering servos");
  delay(2000);
  
  Serial.println("Powering on servos, waiting 10s");
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(10000);

  Serial.println("Rotating servos, waiting 1s between moves");
  servo1.write(1);
  servo2.write(1);
  delay(1000);
  servo1.write(45);
  servo2.write(45);
  delay(1000);
  servo1.write(90);
  servo2.write(90);
  delay(1000);
  servo1.write(135);
  servo2.write(135);
  delay(1000);
  servo1.write(178);
  servo2.write(178);

  Serial.println("Rotations complete, delaying 10s");
  delay(10000);

  Serial.println("Powering down servos");
  digitalWrite(SERVO_POWER_PIN, LOW);
  delay(2000);
  Serial.println("Starting over");
}
