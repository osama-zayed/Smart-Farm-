#include <Arduino.h>
#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\WiFiAccessPoint\\ESP32\\ESP32.ino"
#include <WiFi.h>
#include <WebServer.h>

const int pump1 = 23;
const int pump2 = 22;  
const int pump3 = 21;    
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

bool pump1State = false;
bool pump2State = false;
bool pump3State = false;

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
        String html = "<h1>Control pumps</h1>";
        html += "<p>Click <a href=\"/H1\">Turn ON pump 1</a></p>";
        html += "<p>Click <a href=\"/L1\">Turn OFF pump 1</a></p>";
        html += "<p>Click <a href=\"/H2\">Turn ON pump 2</a></p>";
        html += "<p>Click <a href=\"/L2\">Turn OFF pump 2</a></p>";
        html += "<p>Click <a href=\"/H3\">Turn ON pump 3</a></p>";
        html += "<p>Click <a href=\"/L3\">Turn OFF pump 3</a></p>";
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
        server.send(200, "text/html", "<h1>Auto Control Soil is " + String(autoControlSoilState ? "Enabpump" : "Disabpump") + "</h1><a href=\"/\">Back</a>");
    });

    server.on("/autoControlWater", HTTP_GET, []() {
        String state = server.arg("state");
        autoControlWaterState = (state == "1");
        server.send(200, "text/html", "<h1>Auto Control Water is " + String(autoControlWaterState ? "Enabpump" : "Disabpump") + "</h1><a href=\"/\">Back</a>");
    });

    server.on("/H1", HTTP_GET, []() {
        pump1State = true;
        digitalWrite(pump1, HIGH);
        server.send(200, "text/html", "<h1>pump 1 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L1", HTTP_GET, []() {
        pump1State = false; 
        digitalWrite(pump1, LOW);
        server.send(200, "text/html", "<h1>pump 1 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H2", HTTP_GET, []() {
        pump2State = true; 
        digitalWrite(pump2, HIGH);
        server.send(200, "text/html", "<h1>pump 2 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L2", HTTP_GET, []() {
        pump2State = false; 
        digitalWrite(pump2, LOW);
        server.send(200, "text/html", "<h1>pump 2 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H3", HTTP_GET, []() {
        pump3State = true; 
        digitalWrite(pump3, HIGH);
        server.send(200, "text/html", "<h1>pump 3 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L3", HTTP_GET, []() {
        pump3State = false; 
        digitalWrite(pump3, LOW);
        server.send(200, "text/html", "<h1>pump 3 is OFF</h1><a href=\"/\">Back</a>");
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
        sensorData += "\"pump1\": " + String(pump1State) + ",";
        sensorData += "\"pump2\": " + String(pump2State) + ",";
        sensorData += "\"pump3\": " + String(pump3State) + ",";
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
        pump1State = (soilHumidity1 > 1500); 
        digitalWrite(pump1, pump1State ? HIGH : LOW);
        
        pump2State = (soilHumidity2 > 1500);
        digitalWrite(pump2, pump2State ? HIGH : LOW);
    }
    if (autoControlWaterState) {
        pump3State = (waterLevel < 100); 
        digitalWrite(pump3, pump3State ? HIGH : LOW);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(pump1, OUTPUT);
    pinMode(pump2, OUTPUT);
    pinMode(pump3, OUTPUT);
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

