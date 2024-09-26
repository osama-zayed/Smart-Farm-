#include <Arduino.h>
#line 1 "H:\\sketch_sep18a\\WiFiAccessPoint\\ESP32\\ESP32.ino"
#include <WiFi.h>
#include <WebServer.h>

const int ledPin1 = 23;        // دبوس اللمبة 1
const int ledPin2 = 22;        // دبوس اللمبة 2
const int ledPin3 = 21;        // دبوس اللمبة 3
const int waterSensorPin = 34; // دبوس حساس الماء
const int soilRHXPin1 = 35;    // دبوس حساس رطوبة التربة 1
const int soilRHXPin2 = 32;    // دبوس حساس رطوبة التربة 2

// مصفوفات لتخزين آخر 10 قيم
const int maxValues = 10;
int soilHumidity1Values[maxValues] = {0};
int soilHumidity2Values[maxValues] = {0};
int waterLevelValues[maxValues] = {0};
int sensorIndex = 0;

// إعدادات شبكة الواي فاي
const char *ssid = "osama";         // اسم الشبكة
const char *password = "123123123"; // كلمة مرور الشبكة

WebServer server(80);

bool autoControlSoilState = false;
bool autoControlWaterState = false;

// حالة اللمبات
bool led1State = false;
bool led2State = false;
bool led3State = false;

// إعداد الواي فاي
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

// إعداد السيرفر
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

    // إعداد نقاط التحكم التلقائي
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

    // إعداد نقاط التحكم في اللمبات
    server.on("/H1", HTTP_GET, []() {
        led1State = true; // تحديث الحالة
        digitalWrite(ledPin1, HIGH);
        server.send(200, "text/html", "<h1>LED 1 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L1", HTTP_GET, []() {
        led1State = false; // تحديث الحالة
        digitalWrite(ledPin1, LOW);
        server.send(200, "text/html", "<h1>LED 1 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H2", HTTP_GET, []() {
        led2State = true; // تحديث الحالة
        digitalWrite(ledPin2, HIGH);
        server.send(200, "text/html", "<h1>LED 2 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L2", HTTP_GET, []() {
        led2State = false; // تحديث الحالة
        digitalWrite(ledPin2, LOW);
        server.send(200, "text/html", "<h1>LED 2 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H3", HTTP_GET, []() {
        led3State = true; // تحديث الحالة
        digitalWrite(ledPin3, HIGH);
        server.send(200, "text/html", "<h1>LED 3 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L3", HTTP_GET, []() {
        led3State = false; // تحديث الحالة
        digitalWrite(ledPin3, LOW);
        server.send(200, "text/html", "<h1>LED 3 is OFF</h1><a href=\"/\">Back</a>");
    });

    // إعداد نقطة نهاية لقراءة قيم الحساسات بتنسيق JSON
    server.on("/SensorValues", HTTP_GET, []() {
        // قراءة القيم
        int soilHumidity1 = analogRead(soilRHXPin1);
        int soilHumidity2 = analogRead(soilRHXPin2);
        int waterLevel = analogRead(waterSensorPin);

        // تخزين القيم في المصفوفات
        soilHumidity1Values[sensorIndex] = soilHumidity1;
        soilHumidity2Values[sensorIndex] = soilHumidity2;
        waterLevelValues[sensorIndex] = waterLevel;

        // تحديث الفهرس
        sensorIndex = (sensorIndex + 1) % maxValues;

        String sensorData = "{";
        sensorData += "\"pump1\": " + String(led1State) + ",";
        sensorData += "\"pump2\": " + String(led2State) + ",";
        sensorData += "\"pump3\": " + String(led3State) + ",";
         sensorData += "\"autoControlSoilState\": " + String(autoControlSoilState) + ",";
         sensorData += "\"autoControlWaterState\": " + String(autoControlWaterState) + ",";
        sensorData += "\"data\": [";

        // إضافة بيانات الحساسات إلى المتغير sensorData
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

// قراءة قيم الحساسات والتحكم في اللمبات
void sendSensorValues() {
    int soilHumidity1 = analogRead(soilRHXPin1);
    int soilHumidity2 = analogRead(soilRHXPin2);
    int waterLevel = analogRead(waterSensorPin);

    Serial.print("autoControlSoilState: ");
    Serial.println(autoControlSoilState);
    Serial.print("autoControlWaterState: ");
    Serial.println(autoControlWaterState);

    // التحكم في اللمبات بناءً على الحساسات
    if (autoControlSoilState) {
        led1State = (soilHumidity1 > 1500); // تحديث الحالة
        digitalWrite(ledPin1, led1State ? HIGH : LOW);
        
        led2State = (soilHumidity2 > 1500); // تحديث الحالة
        digitalWrite(ledPin2, led2State ? HIGH : LOW);
    }
    if (autoControlWaterState) {
        led3State = (waterLevel < 100); // تحديث الحالة
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
    server.handleClient(); // التعامل مع الطلبات الواردة
    sendSensorValues();    // إرسال قيم الحساسات والتحكم في اللمبات
    delay(100);
}

