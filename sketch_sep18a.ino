const int waterSensorPin = A0; // رقم المنفذ المتصل بحساس الماء
const int ledPins[] = {2, 3, 4, 5, 6}; // أرقام المنافذ المتصلة بـ LEDs
const int numLeds = 5; // عدد LEDs

void setup() {
  Serial.begin(9600); // بدء الاتصال التسلسلي
  // تعيين المنافذ كخرج
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // قراءة قيمة الحساس
  int sensorValue = analogRead(waterSensorPin);
  
  // طباعة القيمة للعرض
  Serial.print("قيمة الحساس: ");
  Serial.println(sensorValue);
  
  // تحديد عدد LEDs التي يجب تشغيلها بناءً على مستوى الماء
  int ledCount = map(sensorValue, 0, 1023, 0, numLeds); // تحويل القيمة إلى عدد LEDs
  
  // تأكد من أن ledCount لا تتجاوز عدد LEDs
  ledCount = constrain(ledCount, 0, numLeds);

  // تشغيل وإيقاف LEDs بناءً على مستوى الماء
  for (int i = 0; i < numLeds; i++) {
    if (i < ledCount) {
      digitalWrite(ledPins[i], HIGH); // تشغيل LED
    } else {
      digitalWrite(ledPins[i], LOW);  // إيقاف LED
    }
  }
  
  delay(1000); // الانتظار لثانية قبل القراءة التالية
}
