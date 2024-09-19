#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Zaid"; // استبدل بـ SSID الخاص بك
const char* password = "12345678Z"; // استبدل بكلمة المرور الخاصة بك

WebServer server(80); // إنشاء خادم على المنفذ 80
const int ledPin = 23; // رقم المنفذ المتصل بـ LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  server.on("/led/on", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });

  server.on("/led/off", HTTP_GET, []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED is OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
