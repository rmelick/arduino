#include <Servo.h>
#include "RTClib.h"

// two servos powered by PWM
// but also they don't get power until their MOSFET is activated
Servo servo1;
Servo servo2;

const int SERVO_1_PWM_PIN = 9;
const int SERVO_2_PWM_PIN = 10;
const int SERVO_POWER_PIN = 12;

const int SERVO_1_RESTING_ANGLE = 45; // 0
const int SERVO_2_RESTING_ANGLE = 15; // 0
const int SERVO_1_DEPLOYED_ANGLE = 135; // 179
const int SERVO_2_DEPLOYED_ANGLE = 135; // 179


// LEDs are also connected to a MOSFET
const int LEDS_POWER_AND_PWM_PIN = 6;

// only alarm if the lights are still on
const int PHOTO_SENSOR_PIN = A0;
const int DARKNESS_LEVEL = 250;

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
RTC_DS1307 rtc;
const char DAYS_OF_THE_WEEK[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(9600);
  
  ensureIntializedRTC();
  
  servo1.attach(SERVO_1_PWM_PIN);
  servo2.attach(SERVO_2_PWM_PIN);
  pinMode(SERVO_POWER_PIN, OUTPUT);

  pinMode(LEDS_POWER_AND_PWM_PIN, OUTPUT);

  // get orange light on pin 13 to turn off
  // https://arduino.stackexchange.com/questions/3269/what-purpose-does-the-yellow-and-green-leds-have-on-the-arduino/3275#:~:text=The%20green%20LED%20is%20marked,just%20connected%20to%20pin%2013.&text=When%20the%20green%20LED%20stops,light%20the%20LED(s).
  pinMode(13, OUTPUT);
  
  Serial.println("Booting");
}

void ensureIntializedRTC() {
  if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }
 
 if (! rtc.isrunning()) {
   Serial.println("RTC is NOT running!");
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // This line sets the RTC with an explicit date & time, for example to set
   // January 21, 2014 at 3am you would call:
   // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 }
}

void loop() {
  int brightness = analogRead(PHOTO_SENSOR_PIN);
  bool pastBedtime = isItPastBedtime();
  if (pastBedtime && brightness > DARKNESS_LEVEL) {
    Serial.println("Go to sleep already!  Starting motivational sequence.");  
    // move servos so LEDs are visible
    deployEyes();
  
    // Flash LEDs until the photo sensor goes dark
    flashEyesUntilDark();
    
    // move servos so it looks like a normal clock again
    hideEyes();
  }

  delay(1000);
  Serial.println("Starting over");
}

bool isItPastBedtime() {
  printCurrentTime();
  return true;
}

void printCurrentTime() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(DAYS_OF_THE_WEEK[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");
}

void deployEyes() {
  // Move servos into position to expose the eyes  
  Serial.println("Powering on servos, waiting 1s");
  servo1.write(SERVO_1_RESTING_ANGLE);
  servo2.write(SERVO_2_RESTING_ANGLE);
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(1000);

  Serial.println("Rotating servo1");
  rotateServo(servo1, SERVO_1_RESTING_ANGLE, SERVO_1_DEPLOYED_ANGLE);
  Serial.println("Servo1 rotation complete");
  delay(1000);
  rotateServo(servo2, SERVO_2_RESTING_ANGLE, SERVO_2_DEPLOYED_ANGLE);
  Serial.println("Servo2 rotation complete");
  delay(1000);
  
  // Shut down servos to save power
  Serial.println("Shutting down servos");
  digitalWrite(SERVO_POWER_PIN, LOW);
}

void hideEyes() {
  servo1.write(SERVO_1_DEPLOYED_ANGLE);
  servo2.write(SERVO_2_DEPLOYED_ANGLE);
  digitalWrite(SERVO_POWER_PIN, HIGH);
  delay(1000);

  // Move servos back to hidden
  Serial.println("Rotating servos back");
  delay(1000);
  rotateServo(servo2, SERVO_1_DEPLOYED_ANGLE, SERVO_2_RESTING_ANGLE);
  Serial.println("Servo2 rotation complete");
  delay(1000);
  Serial.println("Rotating servo1");
  rotateServo(servo1, SERVO_1_DEPLOYED_ANGLE, SERVO_1_RESTING_ANGLE);
  Serial.println("Servo1 rotation complete");

  Serial.println("Powering down servos");
  digitalWrite(SERVO_POWER_PIN, LOW);
  delay(1000);
}

void flashEyes() {
  Serial.println("Flashing LEDS");
  flashLed(LEDS_POWER_AND_PWM_PIN, 20, 0.25);
}

void flashEyesUntilDark() {
  // blink LEDs until the photoresistor reads dark
  int brightness = analogRead(PHOTO_SENSOR_PIN);
  while (brightness > DARKNESS_LEVEL) {
    Serial.print("Current photo reading ");
    Serial.println(brightness);
    singleLedFlash(LEDS_POWER_AND_PWM_PIN, 0.25);
    brightness = analogRead(PHOTO_SENSOR_PIN);
  }
}

void singleLedFlash(int ledPin, float secondsPerBlink) {
    digitalWrite(ledPin, HIGH);
    delay(secondsPerBlink * 500); // spend half the milliseconds on, and half off
    digitalWrite(ledPin, LOW);
    delay(secondsPerBlink * 500);
}

void flashLed(int ledPin, float secondsToBlink, float secondsPerBlink) {
  int numberOfBlinks = secondsToBlink / secondsPerBlink;
  for (int blinkCount = 0; blinkCount < numberOfBlinks; blinkCount++) {
    digitalWrite(ledPin, HIGH);
    delay(secondsPerBlink * 500); // spend half the milliseconds on, and half off
    digitalWrite(ledPin, LOW);
    delay(secondsPerBlink * 500);
  }
  
  Serial.println("Shutting down LEDS");
  digitalWrite(LEDS_POWER_AND_PWM_PIN, LOW);
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
  for(int angle = startAngle; angle <= endAngle; angle = angle+2)  
  {                                  
    theServo.write(angle);               
    delay(8);                   
  }
}

void rotateServoDown(Servo &theServo, int startAngle, int endAngle) {
  // https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/arduino-code-for-sweep
  startAngle = min(startAngle, 179);
  endAngle = max(endAngle, 0);
  for(int angle = startAngle; angle >= endAngle; angle = angle-2)  
  {                                  
    theServo.write(angle);               
    delay(8);                   
  }
}
