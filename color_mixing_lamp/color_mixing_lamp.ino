const int RED_LED_PIN = 11;
const int GREEN_LED_PIN = 9;
const int BLUE_LED_PIN = 10;

const int RED_SENSOR_PIN = A0;
const int GREEN_SENSOR_PIN = A1;
const int BLUE_SENSOR_PIN = A2;

// desired light level for each LED
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

// reading from each photo sensor
int redSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;
  
void setup() {
  Serial.begin(9600);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

}

void loop() {
  // read sensor values.  Note the 5ms delay, so the ADC can catch up
  redSensorValue = analogRead(RED_SENSOR_PIN);
  delay(5);
  greenSensorValue = analogRead(GREEN_SENSOR_PIN);
  delay(5);
  blueSensorValue = analogRead(BLUE_SENSOR_PIN);

  // print sensor values for debugging
  Serial.print("Raw Sensor Values \t red: ");
  Serial.print(redSensorValue);
  Serial.print("\t green ");
  Serial.print(greenSensorValue);
  Serial.print("\t blue ");
  Serial.print(blueSensorValue);
  Serial.print("\t");

  // sensor values should be between 0-1023, so divide by 4 to be between 0-255, which matches
  // the range of analogWrite
  redValue = redSensorValue / 4;
  greenValue = greenSensorValue / 4;
  blueValue = blueSensorValue / 4;

  // print led values for debugging
  Serial.print("Mapped Sensor Values \t red: ");
  Serial.print(redValue);
  Serial.print("\t green ");
  Serial.print(greenValue);
  Serial.print("\t blue ");
  Serial.print(blueValue);
  Serial.print("\n");

  // finally, actually set the values on the led
  analogWrite(RED_LED_PIN, redValue);
  analogWrite(GREEN_LED_PIN, greenValue);
  analogWrite(BLUE_LED_PIN, blueValue);

}
