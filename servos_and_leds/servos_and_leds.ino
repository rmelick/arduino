#include <Servo.h>

Servo servo1;
Servo servo2;

// two servos powered by PWM
// but also they don't get power until their MOSFET is activated

const int SERVO_1_PWM_PIN = 9;
const int SERVO_2_PWM_PIN = 10;
const int SERVO_POWER_PIN = 12;

const int LEDS_POWER_AND_PWM_PIN = 6;

void setup() {
  servo1.attach(SERVO_1_PWM_PIN);
  servo2.attach(SERVO_2_PWM_PIN);
  pinMode(SERVO_POWER_PIN, OUTPUT);

  pinMode(LEDS_POWER_AND_PWM_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Booting");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Waiting 5s before powering LEDs");
  delay(5000);

  Serial.println("Powering LEDs");
  digitalWrite(LEDS_POWER_AND_PWM_PIN, HIGH);
  Serial.println("Waiting 5s before flashing LEDs");
  delay(5000);

  Serial.println("Flashing LEDS");
  flashLed(LEDS_POWER_AND_PWM_PIN, 30, 2);

  Serial.println("Shutting down LEDS");
  digitalWrite(LEDS_POWER_AND_PWM_PIN, LOW);
}

void flashLed(int ledPin, int secondsToBlink, int secondsPerBlink) {
  int numberOfBlinks = secondsToBlink / secondsPerBlink;
  for (int blinkCount = 0; blinkCount < numberOfBlinks; blinkCount++) {
    digitalWrite(ledPin, HIGH);
    delay(secondsPerBlink * 500); // spend half the milliseconds on, and half off
    digitalWrite(ledPin, LOW);
    delay(secondsPerBlink * 500);
  }
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
