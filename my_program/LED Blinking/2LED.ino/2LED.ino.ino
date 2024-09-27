const int led1 = 8;
const int led2 = 9;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= 200) {
    digitalWrite(led1, !digitalRead(led1));
    previousMillis1 = currentMillis;
  }

  if (currentMillis - previousMillis2 >= 1000) {
    digitalWrite(led2, !digitalRead(led2));
    previousMillis2 = currentMillis;
  }
}
