
const int SWITCH_PIN = 2;
const int MOTOR_PIN = 9;
const int POT_PIN = A0;

int switchState = 0;
int potVal;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  // why didn't the example project 5 need us to enable pinMode?  Is that just for digital?
  //pinMode(POT_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH) {
    potVal = analogRead(POT_PIN);
    int desiredMotorSpeed = map(potVal, 0, 1023, 0, 255);
    Serial.print("Raw potVal: ");
    Serial.print(potVal);
    Serial.print("\t desired speed: ");
    Serial.print(desiredMotorSpeed);
    Serial.print("\n");
    
    // instead of digitalWrite(HIGH) (max speed), write out the PWM
    //digitalWrite(MOTOR_PIN, HIGH);
    analogWrite(MOTOR_PIN, desiredMotorSpeed);
  }
  else {
    digitalWrite(MOTOR_PIN, LOW);
  }
}
