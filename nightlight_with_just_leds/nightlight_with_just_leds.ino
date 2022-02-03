#include <math.h>

const int RED_LED_1_PIN = 9;
const int GREEN_LED_1_PIN = 10;
const int BLUE_LED_1_PIN = 11;

const int RED_LED_2_PIN = 6;
const int GREEN_LED_2_PIN = 5;
const int BLUE_LED_2_PIN = 3;

const int PHOTO_SENSOR_PIN = A0;

const int DARKNESS_LEVEL = 5;//5;
const bool DEBUG = false;

int photoSensorValue = 0;
  
void setup() {
  if (DEBUG) {
    Serial.begin(9600);
  }

  pinMode(RED_LED_1_PIN, OUTPUT);
  pinMode(GREEN_LED_1_PIN, OUTPUT);
  pinMode(BLUE_LED_1_PIN, OUTPUT);

  
  pinMode(RED_LED_2_PIN, OUTPUT);
  pinMode(GREEN_LED_2_PIN, OUTPUT);
  pinMode(BLUE_LED_2_PIN, OUTPUT);

  // get orange light to turn off
  // https://arduino.stackexchange.com/questions/3269/what-purpose-does-the-yellow-and-green-leds-have-on-the-arduino/3275#:~:text=The%20green%20LED%20is%20marked,just%20connected%20to%20pin%2013.&text=When%20the%20green%20LED%20stops,light%20the%20LED(s).
  pinMode(13, OUTPUT);

  write_both_leds(0, 0, 0);
}

void loop() {
  // read sensor values.  Note the 5ms delay, so the ADC can catch up
  photoSensorValue = analogRead(PHOTO_SENSOR_PIN);
  delay(5);

  if (DEBUG) {
    // print sensor values for debugging
    Serial.print("Raw Photo Sensor Value: ");
    Serial.print(photoSensorValue);
    Serial.print("\t");
  }

  // photo sensor values should be between 0-1023
  if (photoSensorValue < DARKNESS_LEVEL) {
    if (DEBUG) {
      Serial.print("Low light level detected, enabling LEDs \n");
    }

    int pattern = random(0, 2);
    if (pattern == 0) {
      breathe_red(1);
    } else if (pattern == 1) {
      breathe_orange(1);
    } else {
      Serial.print("Unknown random ");
      Serial.print(pattern);
      Serial.print(" skipping\n");
    }
    // finally, actually set the values on the LEDs
    breathe_red(3);
    breathe_orange(3);
  } else {
    if (DEBUG) {
      Serial.print("High light level detected, disabling LEDs\n");
    }
    
    write_both_leds(LOW, LOW, LOW);
  }
}

const float TIME_ZER0 = (1.0 - EULER) * 1000;
const float PERIOD_SECS = 16;
const float PERIOD_MODIFIER = (2 * PI) / PERIOD_SECS;

/*
 * Just red breathing
 */
void breathe_red(int breaths_to_take) {
  int start_millis = millis();
  int elapsed_millis = 0;
  int millis_to_breathe = breaths_to_take * PERIOD_SECS * 1000;
  float redValue;
  while (elapsed_millis <= millis_to_breathe) {
    redValue = single_breathing_value(TIME_ZER0 + elapsed_millis, 10.0, 255.0);

    write_both_leds(redValue, 0, 0);
    
    elapsed_millis = millis() - start_millis;
  }
}

/*
 * Always keep the red on, but keep the green a lower amplitude
 */

void breathe_orange(int breaths_to_take) {
  int start_millis = millis();
  int elapsed_millis = 0;
  int millis_to_breathe = breaths_to_take * PERIOD_SECS * 1000;
  float redValue;
  float greenValue;
  while (elapsed_millis <= millis_to_breathe) {
    redValue = single_breathing_value(TIME_ZER0 + elapsed_millis, 30.0, 255.0);
    greenValue = single_breathing_value(TIME_ZER0 + elapsed_millis, 0.0, 5.0);

    write_both_leds(redValue, greenValue, 0);
    
    elapsed_millis = millis() - start_millis;
  }
}

void write_both_leds(float redValue, float greenValue, float blueValue) {
    analogWrite(RED_LED_1_PIN, redValue);
    analogWrite(GREEN_LED_1_PIN, greenValue);
    analogWrite(BLUE_LED_1_PIN, blueValue);
    analogWrite(RED_LED_2_PIN, redValue);
    analogWrite(GREEN_LED_2_PIN, greenValue);
    analogWrite(BLUE_LED_2_PIN, blueValue);
}



const float CURVE_MIN = 1 / EULER;
const float CURVE_MAX = EULER;
const float PWM_MIN = 10; // experiments show it turns off below this
const float PWM_MAX = 255;

float single_breathing_value(int current_millis, float min_output, float max_output) {
  // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
  
  float current_secs = current_millis / 1000.0;
  float curve = exp(sin(current_secs * PERIOD_MODIFIER));
  float curve_mapped_to_pwm = map_float(curve, CURVE_MIN, CURVE_MAX, min_output, max_output);
  // without using map(), the blog used this function
  // curve_mapped_to_pwm = (exp(sin(current_secs * period)) - 0.36787944)*108.0;
  return curve_mapped_to_pwm;
}


float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  // built in map() only works on integers
  // https://www.arduino.cc/reference/en/language/functions/math/map/
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
