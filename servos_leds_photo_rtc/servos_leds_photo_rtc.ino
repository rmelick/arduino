#include <Servo.h>

Servo servo1;
Servo servo2;

// two servos powered by PWM
// but also they don't get power until their MOSFET is activated

const int SERVO_1_PWM_PIN = 9;
const int SERVO_2_PWM_PIN = 10;
const int SERVO_POWER_PIN = 12;

const int SERVO_1_RESTING_ANGLE = 0;
const int SERVO_2_RESTING_ANGLE = 0;
const int SERVO_1_DEPLOYED_ANGLE = 179;
const int SERVO_2_DEPLOYED_ANGLE = 179;


// LEDs are also connected to a MOSFET
const int LEDS_POWER_AND_PWM_PIN = 6;


void setup() {
  servo1.attach(SERVO_1_PWM_PIN);
  servo2.attach(SERVO_2_PWM_PIN);
  pinMode(SERVO_POWER_PIN, OUTPUT);

  pinMode(LEDS_POWER_AND_PWM_PIN, OUTPUT);

  // get orange light on pin 13 to turn off
  // https://arduino.stackexchange.com/questions/3269/what-purpose-does-the-yellow-and-green-leds-have-on-the-arduino/3275#:~:text=The%20green%20LED%20is%20marked,just%20connected%20to%20pin%2013.&text=When%20the%20green%20LED%20stops,light%20the%20LED(s).
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  Serial.println("Booting");
}

void loop() {
  // Boot up
  Serial.println("Waiting 5s before alarming");
  delay(5000);

  // Move servos into position to expose the eyes  
  Serial.println("Powering on servos, waiting 5s");
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(5000);

  Serial.println("Rotating servo1");
  rotateServo(servo1, SERVO_1_RESTING_ANGLE, SERVO_1_DEPLOYED_ANGLE);
  Serial.println("Servo1 rotation complete");
  delay(1000);
  rotateServo(servo2, SERVO_2_RESTING_ANGLE, SERVO_2_DEPLOYED_ANGLE);
  Serial.println("Servo2 rotation complete");
  delay(1000);
  
  // Shut down servos to save power
  digitalWrite(SERVO_POWER_PIN, LOW);
  delay(1000);

  // Flash LEDs for a while
  Serial.println("Flashing LEDS");
  flashLed(LEDS_POWER_AND_PWM_PIN, 20, 0.25);

  Serial.println("Shutting down LEDS");
  digitalWrite(LEDS_POWER_AND_PWM_PIN, LOW);

  // Turn servos back on
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(1000);

  // Move servos back to hidden
  Serial.println("Rotating servos back");
  Serial.println("Rotating servo1");
  rotateServo(servo1, SERVO_1_DEPLOYED_ANGLE, SERVO_1_RESTING_ANGLE);
  Serial.println("Servo1 rotation complete");
  delay(1000);
  rotateServo(servo2, SERVO_1_DEPLOYED_ANGLE, SERVO_2_RESTING_ANGLE);
  Serial.println("Servo2 rotation complete");
  delay(1000);

  Serial.println("Powering down servos");
  digitalWrite(SERVO_POWER_PIN, LOW);
  delay(5000);
  Serial.println("Starting over");


}

void flashLed(int ledPin, float secondsToBlink, float secondsPerBlink) {
  int numberOfBlinks = secondsToBlink / secondsPerBlink;
  for (int blinkCount = 0; blinkCount < numberOfBlinks; blinkCount++) {
    digitalWrite(ledPin, HIGH);
    delay(secondsPerBlink * 500); // spend half the milliseconds on, and half off
    digitalWrite(ledPin, LOW);
    delay(secondsPerBlink * 500);
  }
}


void rotateServo(Servo &theServo, int startAngle, int endAngle) {
  if (startAngle > endAngle) {
    rotateServoDown(theServo, startAngle, endAngle);
  } else {
    rotateServoUp(theServo, startAngle, endAngle);
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
