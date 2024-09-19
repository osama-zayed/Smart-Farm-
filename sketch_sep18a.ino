const int waterSensorPin = A0; // رقم المنفذ المتصل بحساس الماء
const int pumpPin = 7; // رقم المنفذ المتصل بالترانزستور (أو الريليه)
const int threshold = 300; // القيمة الدنيا لتحفيز المضخة (تحتاج لتعديل هذه القيمة حسب حساس الماء)

void setup() {
  pinMode(pumpPin, OUTPUT); // تعيين المنفذ المتصل بالمضخة كمخرج
  Serial.begin(9600); // بدء الاتصال التسلسلي
}

void loop() {
  // قراءة قيمة الحساس
  int sensorValue = analogRead(waterSensorPin);
  
  // طباعة القيمة للعرض
  Serial.print("قيمة الحساس: ");
  Serial.println(sensorValue);
  
  // إذا كانت القيمة أقل من العتبة، قم بتشغيل المضخة
  if (sensorValue < threshold) {
    digitalWrite(pumpPin, HIGH); // تشغيل المضخة
  } else {
    digitalWrite(pumpPin, LOW); // إيقاف المضخة
  }
  
  delay(1000); // الانتظار لثانية قبل القراءة التالية
}
