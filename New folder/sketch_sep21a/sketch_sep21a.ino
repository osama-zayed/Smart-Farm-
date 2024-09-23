#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// إعدادات الشبكة
const char* ssid = "osama zayed";
const char* password = "osama775561";

// إعدادات ThingSpeak
const String writeApiKey = "XBJ3H2MOY9WC3KZA";
const String readApiKey = "2YM7M7O0JBS4DY9E";
const String writeBaseURL = "http://api.thingspeak.com/update";
const String readBaseURL = "http://api.thingspeak.com/channels/2662884/feeds.json";

// إعدادات GPIO
const int ledPin1 = 23;
const int ledPin2 = 22;
const int ledPin3 = 21;
const int waterSensorPin = 34;
const int soilRHXPin1 = 35;
const int soilRHXPin2 = 32;

int autoControlSoilState = -1;
int autoControlWaterState = -1;
int lastLedState1 = -1;
int lastLedState2 = -1;
int lastLedState3 = -1;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;

void setup() {
    Serial.begin(115200);
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(waterSensorPin, INPUT);
    pinMode(soilRHXPin1, INPUT);
    pinMode(soilRHXPin2, INPUT);
    connectToWiFi();
}

void loop() {
    unsigned long currentMillis = millis();
    int waterLevel = analogRead(waterSensorPin);
    int soilHumidity1 = analogRead(soilRHXPin1);
    int soilHumidity2 = analogRead(soilRHXPin2);

    controlLEDs(soilHumidity1, soilHumidity2, waterLevel);

    if (currentMillis - lastSendTime >= sendInterval) {
        lastSendTime = currentMillis;
        sendToThingSpeak(soilHumidity1, soilHumidity2, waterLevel);
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void sendToThingSpeak(int soilHumidity1, int soilHumidity2, int waterLevel) {
    HTTPClient http;
    String url = writeBaseURL + "?api_key=" + writeApiKey +
                 "&field4=" + String(soilHumidity1) +
                 "&field5=" + String(soilHumidity2) +
                 "&field6=" + String(waterLevel);
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

int readFieldFromThingSpeak(int fieldNumber) {
    HTTPClient http;
    String url = readBaseURL + "?api_key=" + readApiKey + "&results=1";
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Response: " + response);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);
        
        if (error) {
            Serial.print("DeserializeJson failed: ");
            Serial.println(error.f_str());
            return -1;
        }

        if (doc["feeds"].size() == 0 || doc["feeds"][0]["field" + String(fieldNumber)].isNull()) {
            return -1;  // أو أي قيمة تمثل عدم وجود قيمة
        } else {
            return doc["feeds"][0]["field" + String(fieldNumber)].as<int>();
        }
    } else {
        Serial.print("Error on HTTP request: ");
        Serial.println(httpResponseCode);
        return -1;  // أو أي قيمة تمثل عدم وجود قيمة
    }
    http.end();
}

void controlLEDs(int soilHumidity1, int soilHumidity2, int waterLevel) {
    int led1 = readFieldFromThingSpeak(1);
    int led2 = readFieldFromThingSpeak(2);
    int led3 = readFieldFromThingSpeak(3);
    autoControlSoilState = readFieldFromThingSpeak(7);
    autoControlWaterState = readFieldFromThingSpeak(8);

    Serial.print("Auto Control Soil: ");
    Serial.println(autoControlSoilState);
    Serial.print("Auto Control Water: ");
    Serial.println(autoControlWaterState);

    // التحكم في اللمبة الأولى
    int state1 = (led1 != -1) ? led1 : lastLedState1;
    if (state1 != lastLedState1) {
        lastLedState1 = state1;
        digitalWrite(ledPin1, (autoControlSoilState && (soilHumidity1 < 300)) ? HIGH : (state1 == 1 ? HIGH : LOW));
        Serial.println((state1 == 1) ? "LED 1 is ON" : "LED 1 is OFF");
    }

    // التحكم في اللمبة الثانية
    int state2 = (led2 != -1) ? led2 : lastLedState2;
    if (state2 != lastLedState2) {
        lastLedState2 = state2;
        digitalWrite(ledPin2, (autoControlSoilState && (soilHumidity2 < 300)) ? HIGH : (state2 == 1 ? HIGH : LOW));
        Serial.println((state2 == 1) ? "LED 2 is ON" : "LED 2 is OFF");
    }

    // التحكم في اللمبة الثالثة
    int state3 = (led3 != -1) ? led3 : lastLedState3;
    if (state3 != lastLedState3) {
        lastLedState3 = state3;
        digitalWrite(ledPin3, (autoControlWaterState && (waterLevel > 50)) ? HIGH : (state3 == 1 ? HIGH : LOW));
        Serial.println((state3 == 1) ? "LED 3 is ON" : "LED 3 is OFF");
    }
}
