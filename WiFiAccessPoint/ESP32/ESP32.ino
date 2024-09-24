#include <WiFi.h>
#include <WebServer.h>

const int ledPin1 = 23;        // دبوس اللمبة 1
const int ledPin2 = 22;        // دبوس اللمبة 2
const int ledPin3 = 21;        // دبوس اللمبة 3
const int waterSensorPin = 34; // دبوس حساس الماء
const int soilRHXPin1 = 35;    // دبوس حساس رطوبة التربة 1
const int soilRHXPin2 = 32;    // دبوس حساس رطوبة التربة 2

// إعدادات شبكة الواي فاي
const char *ssid = "osama32";
const char *password = "123123123";

WebServer server(80);

// حالة التحكم التلقائي
bool autoControlSoilState = false;
bool autoControlWaterState = false;

// إعداد الواي فاي
void setupWiFi() {
    WiFi.softAP(ssid, password);
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
        html += "<p><a href=\"/SensorValues\">View Sensor Values (JSON)</a></p>"; // رابط لعرض القيم بتنسيق JSON
        server.send(200, "text/html", html);
    });

    // إعداد نقاط التحكم التلقائي
    server.on("/autoControlSoil", HTTP_GET, []() {
        String state = server.arg("state");
                 if(state == "1"){
        autoControlSoilState = true;
        }else{
        autoControlSoilState = false;
        }
        server.send(200, "text/html", "<h1>Auto Control Soil is " + String(autoControlSoilState ? "Enabled" : "Disabled") + "</h1><a href=\"/\">Back</a>");
    });

    server.on("/autoControlWater", HTTP_GET, []() {
        String state = server.arg("state");
         if(state == "1"){
        autoControlWaterState = true;
        }else{
        autoControlWaterState = false;
        }
        server.send(200, "text/html", "<h1>Auto Control Water is " + String(autoControlWaterState ? "Enabled" : "Disabled") + "</h1><a href=\"/\">Back</a>");
    });

    // إعداد نقاط التحكم في اللمبات
    server.on("/H1", HTTP_GET, []() {
        digitalWrite(ledPin1, HIGH);
        server.send(200, "text/html", "<h1>LED 1 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L1", HTTP_GET, []() {
        digitalWrite(ledPin1, LOW);
        server.send(200, "text/html", "<h1>LED 1 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H2", HTTP_GET, []() {
        digitalWrite(ledPin2, HIGH);
        server.send(200, "text/html", "<h1>LED 2 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L2", HTTP_GET, []() {
        digitalWrite(ledPin2, LOW);
        server.send(200, "text/html", "<h1>LED 2 is OFF</h1><a href=\"/\">Back</a>");
    });

    server.on("/H3", HTTP_GET, []() {
        digitalWrite(ledPin3, HIGH);
        server.send(200, "text/html", "<h1>LED 3 is ON</h1><a href=\"/\">Back</a>");
    });

    server.on("/L3", HTTP_GET, []() {
        digitalWrite(ledPin3, LOW);
        server.send(200, "text/html", "<h1>LED 3 is OFF</h1><a href=\"/\">Back</a>");
    });

    // إعداد نقطة نهاية لقراءة قيم الحساسات بتنسيق JSON
    server.on("/SensorValues", HTTP_GET, []() {
        int soilHumidity1 = analogRead(soilRHXPin1); // قراءة رطوبة التربة 1
        int soilHumidity2 = analogRead(soilRHXPin2); // قراءة رطوبة التربة 2
        int waterLevel = analogRead(waterSensorPin);  // قراءة مستوى الماء

        // إنشاء استجابة JSON
        String sensorData = "{";
        sensorData += "\"soilHumidity1\": " + String(soilHumidity1) + ",";
        sensorData += "\"soilHumidity2\": " + String(soilHumidity2) + ",";
        sensorData += "\"waterLevel\": " + String(waterLevel);
        sensorData += "}";

        server.send(200, "application/json", sensorData); // إرسال استجابة JSON
    });

    server.begin();
    Serial.println("Server started");
}

// قراءة قيم الحساسات والتحكم في اللمبات
void sendSensorValues() {
    int soilHumidity1 = analogRead(soilRHXPin1); // قراءة رطوبة التربة 1
    int soilHumidity2 = analogRead(soilRHXPin2); // قراءة رطوبة التربة 2
    int waterLevel = analogRead(waterSensorPin);  // قراءة مستوى الماء

    Serial.print("autoControlSoilState: ");
    Serial.println(autoControlSoilState);
    Serial.print("autoControlWaterState: ");
    Serial.println(autoControlWaterState);

    // التحكم في اللمبات بناءً على الحساسات
    if (autoControlSoilState) {
        digitalWrite(ledPin1, (soilHumidity1 > 1200) ? HIGH : LOW); // تشغيل LED 1 بناءً على رطوبة التربة 1
        digitalWrite(ledPin2, (soilHumidity2 > 1200) ? HIGH : LOW); // تشغيل LED 2 بناءً على رطوبة التربة 2
    }
    if (autoControlWaterState) {
        digitalWrite(ledPin3, (waterLevel < 100) ? HIGH : LOW); // تشغيل LED 3 بناءً على مستوى الماء
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
}
