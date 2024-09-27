const int sensorPin = A1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
 
  float voltage = sensorValue * (5.0 / 1023);
  
  float temperature = voltage * 100; 
  
  Serial.print(temperature);
  Serial.println(" °C");
  
  delay(1000);
}