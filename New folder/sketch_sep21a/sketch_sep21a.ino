#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "osama zayed"; // استبدل بـ SSID الخاص بك
const char* password = "osama775561"; // استبدل بكلمة المرور الخاصة بك

const String apiKey = "2YM7M7O0JBS4DY9E"; // مفتاح API الخاص بالقناة
const String channelID = "2662884"; // قناة ThingSpeak
const int ledPin1 = 23; // رقم GPIO المتصل بالـ LED للحقل الأول
const int ledPin2 = 22; // رقم GPIO المتصل بالـ LED للحقل الثاني

int lastLedState1 = -1; // لتتبع حالة اللمبة للحقل الأول
int lastLedState2 = -1; // لتتبع حالة اللمبة للحقل الثاني

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // الاتصال بشبكة WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  // التحقق من الاتصال بالشبكة
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // إعداد عنوان URL لقراءة البيانات من ThingSpeak
    String url = "http://api.thingspeak.com/channels/" + channelID + "/feeds.json?api_key=" + apiKey + "&results=1";
    
    http.begin(url); // إعداد عنوان URL
    int httpResponseCode = http.GET(); // إجراء الطلب GET

    if (httpResponseCode > 0) {
      String response = http.getString(); // الحصول على الاستجابة
      Serial.println(httpResponseCode); // طباعة رمز الاستجابة
      Serial.println(response); // طباعة الاستجابة
      
      // تحليل JSON لاستخراج قيمة الحقول
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      
      // قراءة حالة اللمبات من الحقول
      String ledState1 = doc["feeds"][0]["field1"]; // الحصول على قيمة الحقل الأول
      String ledState2 = doc["feeds"][0]["field2"]; // الحصول على قيمة الحقل الثاني

      // تحويل القيم إلى int
      int state1 = (ledState1 != "null") ? ledState1.toInt() : lastLedState1;
      int state2 = (ledState2 != "null") ? ledState2.toInt() : lastLedState2;

      // طباعة القيم
      Serial.print("LED 1 State: ");
      Serial.println(state1);
      Serial.print("LED 2 State: ");
      Serial.println(state2);

      // التحكم في LED للحقل الأول
      if (state1 != lastLedState1) {
        lastLedState1 = state1;

        if (state1 == 1) {
          digitalWrite(ledPin1, HIGH);
          Serial.println("LED 1 is ON");
        } else {
          digitalWrite(ledPin1, LOW);
          Serial.println("LED 1 is OFF");
        }
      }

      // التحكم في LED للحقل الثاني
      if (state2 != lastLedState2) {
        lastLedState2 = state2;

        if (state2 == 1) {
          digitalWrite(ledPin2, HIGH);
          Serial.println("LED 2 is ON");
        } else {
          digitalWrite(ledPin2, LOW);
          Serial.println("LED 2 is OFF");
        }
      }

    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end(); // إنهاء الاتصال
  } else {
    Serial.println("WiFi Disconnected");
  }

  // الانتظار لمدة 5 ثوانٍ قبل التكرار
  delay(5000); 
}
