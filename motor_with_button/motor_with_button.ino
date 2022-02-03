
const int SWITCH_PIN = 2;
const int MOTOR_PIN = 9;

int switchState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH) {
    digitalWrite(MOTOR_PIN, HIGH);
  }
  else {
    digitalWrite(MOTOR_PIN, LOW);
  }
}
