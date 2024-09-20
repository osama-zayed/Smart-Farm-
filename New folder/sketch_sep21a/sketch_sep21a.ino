#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // إضافة المكتبة هنا

const char* ssid = "osama zayed"; // استبدل بـ SSID الخاص بك
const char* password = "osama775561"; // استبدل بكلمة المرور الخاصة بك

const String apiKey = "2YM7M7O0JBS4DY9E"; // مفتاح API الخاص بالقراءة
const String channelID = "2662884"; // قناة ThingSpeak
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
  // التحقق من الاتصال بالشبكة
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // إعداد عنوان URL لقراءة البيانات من ThingSpeak
    String url = "http://api.thingspeak.com/channels/" + channelID + "/fields/1.json?api_key=" + apiKey + "&results=1";
    
    http.begin(url); // إعداد عنوان URL
    int httpResponseCode = http.GET(); // إجراء الطلب GET

    if (httpResponseCode > 0) {
      String response = http.getString(); // الحصول على الاستجابة
      Serial.println(httpResponseCode); // طباعة رمز الاستجابة
      Serial.println(response); // طباعة الاستجابة
      
      // تحليل JSON لاستخراج قيمة الحقل
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      int ledState = doc["feeds"][0]["field1"]; // الحصول على قيمة الحقل

      // تشغيل أو إيقاف تشغيل LED بناءً على القيمة
      if (ledState == 1) {
        digitalWrite(ledPin, HIGH); // تشغيل LED
        Serial.println("LED is ON");
      } else {
        digitalWrite(ledPin, LOW); // إيقاف تشغيل LED
        Serial.println("LED is OFF");
      }

    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end(); // إنهاء الاتصال
  } else {
    Serial.println("WiFi Disconnected");
  }

  // الانتظار لمدة 15 ثانية قبل التكرار
  delay(15000); // الانتظار بين الاستعلامات
}
