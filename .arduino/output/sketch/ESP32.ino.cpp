#include <Arduino.h>
#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\WiFiAccessPoint\\ESP32\\ESP32.ino"
#include <WiFi.h>
#include <WebServer.h>

const int ledPin1 = 23;
const int ledPin2 = 22;  
const int ledPin3 = 21;    
const int waterSensorPin = 34;
const int soilRHXPin1 = 35; 
const int soilRHXPin2 = 32; 

const int maxValues = 10;
int soilHumidity1Values[maxValues] = {0};
int soilHumidity2Values[maxValues] = {0};
int waterLevelValues[maxValues] = {0};
int sensorIndex = 0;

const char *ssid = "osama";
const char *password = "123123123"; 

WebServer server(80);

bool autoControlSoilState = false;
bool autoControlWaterState = false;

bool led1State = false;
bool led2State = false;
bool led3State = false;

void setupWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");

    WiFi.softAP("osama32", "123123123");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void setupServer() {
    server.on("/", HTTP_GET, []() {
        String html = "<h1>Control LEDs</h1>";
        html += "<p>Click <a href=\"/H1\">Turn ON LED 1</a></p>";
        html += "<p>Click <a href=\"/L1\">Turn OFF LED 1</a></p>";
        html += "<p>Click <a href=\"/H2\">Turn ON LED 2</a></p>";
        html += "<p>Click <a href=\"/L2\">Turn OFF LED 2</a></p>";
        html += "<p>Click <a href=\"/H3\">Turn ON LED 3</a></p>";
        html += "<p>Click <a href=\"/L3\">Turn OFF LED 3</a></p>";
        html += "<p><a href=\"/autoControlSoil?state=1\">Enable Auto Control Soil</a></p>";
        html += "<p><a href=\"/autoControlSoil?state=0\">Disable Auto Control Soil</a></p>";
        html += "<p><a href=\"/autoControlWater?state=1\">Enable Auto Control Water</a></p>";
        html += "<p><a href=\"/autoControlWater?state=0\">Disable Auto Control Water</a></p>";
        html += "<p><a href=\"/SensorValues\">View Sensor Values (JSON)</a></p>";
        server.send(200, "text/html", html);
    });

    server.on("/autoControlSoil", HTTP_GET, []() {
        String state = server.arg("state");
        autoControlSoilState = (state == "1");
        server.send(200, "text/html", "<h1>Auto Control Soil is " + String(autoControlSoilState ? "Enabled" : "Disabled") + "</h1><a href=\"/\">Back</a>");
    });

    server.on("/autoControlWater", HTTP_GET, []() {
        String state = server.arg("state");
        autoControlWaterState = (state == "1");
        server.send(200, "text/html", "<h1>Auto Control Water is " + String(autoControlWaterState ? "Enabled" : "Disabled") + "</h1><a href=\"/\">Back</a>");
    });

    server.on("/H1", HTTP_GET, []() {
        led1State = true;
        digitalWrite(ledPin1, HIGH);
        server.send(200, "text/html", "<h1>LED 1 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L1", HTTP_GET, []() {
        led1State = false; 
        digitalWrite(ledPin1, LOW);
        server.send(200, "text/html", "<h1>LED 1 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H2", HTTP_GET, []() {
        led2State = true; 
        digitalWrite(ledPin2, HIGH);
        server.send(200, "text/html", "<h1>LED 2 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L2", HTTP_GET, []() {
        led2State = false; 
        digitalWrite(ledPin2, LOW);
        server.send(200, "text/html", "<h1>LED 2 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H3", HTTP_GET, []() {
        led3State = true; 
        digitalWrite(ledPin3, HIGH);
        server.send(200, "text/html", "<h1>LED 3 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L3", HTTP_GET, []() {
        led3State = false; 
        digitalWrite(ledPin3, LOW);
        server.send(200, "text/html", "<h1>LED 3 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/SensorValues", HTTP_GET, []() {
        int soilHumidity1 = analogRead(soilRHXPin1);
        int soilHumidity2 = analogRead(soilRHXPin2);
        int waterLevel = analogRead(waterSensorPin);

        soilHumidity1Values[sensorIndex] = soilHumidity1;
        soilHumidity2Values[sensorIndex] = soilHumidity2;
        waterLevelValues[sensorIndex] = waterLevel;

        sensorIndex = (sensorIndex + 1) % maxValues;

        String sensorData = "{";
        sensorData += "\"pump1\": " + String(led1State) + ",";
        sensorData += "\"pump2\": " + String(led2State) + ",";
        sensorData += "\"pump3\": " + String(led3State) + ",";
         sensorData += "\"autoControlSoilState\": " + String(autoControlSoilState) + ",";
         sensorData += "\"autoControlWaterState\": " + String(autoControlWaterState) + ",";
        sensorData += "\"data\": [";

        for (int i = 0; i < maxValues; i++) {
            sensorData += "{";
            sensorData += "\"soilHumidity1\": " + String(soilHumidity1Values[i]) + ",";
            sensorData += "\"soilHumidity2\": " + String(soilHumidity2Values[i]) + ",";
            sensorData += "\"waterLevel\": " + String(waterLevelValues[i]);
            sensorData += "}";
            if (i < maxValues - 1) {
                sensorData += ",";
            }
        }
        sensorData += "]";
        sensorData += "}";

        server.send(200, "application/json", sensorData);
    });

    server.begin();
    Serial.println("Server started");
}

void sendSensorValues() {
    int soilHumidity1 = analogRead(soilRHXPin1);
    int soilHumidity2 = analogRead(soilRHXPin2);
    int waterLevel = analogRead(waterSensorPin);

    Serial.print("autoControlSoilState: ");
    Serial.println(autoControlSoilState);
    Serial.print("autoControlWaterState: ");
    Serial.println(autoControlWaterState);

    if (autoControlSoilState) {
        led1State = (soilHumidity1 > 1500); 
        digitalWrite(ledPin1, led1State ? HIGH : LOW);
        
        led2State = (soilHumidity2 > 1500);
        digitalWrite(ledPin2, led2State ? HIGH : LOW);
    }
    if (autoControlWaterState) {
        led3State = (waterLevel < 100); 
        digitalWrite(ledPin3, led3State ? HIGH : LOW);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(waterSensorPin, INPUT);
    pinMode(soilRHXPin1, INPUT);
    pinMode(soilRHXPin2, INPUT);
    setupWiFi();
    setupServer();
}

void loop() {
    server.handleClient(); 
    sendSensorValues();   
    delay(100);
}

