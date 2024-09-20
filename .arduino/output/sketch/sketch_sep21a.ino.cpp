#include <Arduino.h>
#line 1 "H:\\sketch_sep18a\\New folder\\sketch_sep21a\\sketch_sep21a.ino"
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "osama zayed"; // استبدل بـ SSID الخاص بك
const char* password = "osama775561"; // استبدل بكلمة المرور الخاصة بك

const String apiKey = "2YM7M7O0JBS4DY9E"; // استبدل بمفتاح API الخاص بك
const String channelID = "2662884"; // قناة ThingSpeak
const String ledState = "1"; // 1 لتشغيل LED، 0 لإيقاف تشغيله

const int ledPin = 23; // رقم GPIO المتصل بالـ LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // الاتصال بشبكة WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // تشغيل الـ LED
  digitalWrite(ledPin, HIGH);
  
  // إرسال البيانات إلى ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + ledState;
    
    http.begin(url); // إعداد عنوان URL
    int httpResponseCode = http.GET(); // إجراء الطلب GET

    if (httpResponseCode > 0) {
      String response = http.getString(); // الحصول على الاستجابة
      Serial.println(httpResponseCode); // طباعة رمز الاستجابة
      Serial.println(response); // طباعة الاستجابة
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end(); // إنهاء الاتصال
  } else {
    Serial.println("WiFi Disconnected");
  }

  // إيقاف تشغيل الـ LED بعد 5 ثوانٍ
  delay(5000);
  digitalWrite(ledPin, LOW);

  // إرسال بيانات إيقاف تشغيل الـ LED
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=0"; // 0 لإيقاف تشغيل الـ LED
    
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // الانتظار لمدة 60 ثانية قبل التكرار
  delay(60000);
}

