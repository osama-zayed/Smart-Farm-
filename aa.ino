const int potPin = A3; 
const int ledPin = 3; 

void setup() {
    Serial.begin(9600); 
    pinMode(ledPin, OUTPUT);
}

void loop() {
    int sensorValue = analogRead(potPin); 
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue); 

    int dutyCycle = map(sensorValue, 0, 1023, 0, 255);

    analogWrite(ledPin, dutyCycle); 

    delay(100);
}