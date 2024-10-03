#include <Arduino.h>
#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\sketch_sep18a.ino"
const int waterSensorPin = A0; // رقم المنفذ المتصل بحساس الماء
const int pumpPin = 7; // رقم المنفذ المتصل بالترانزستور (أو الريليه)
const int threshold = 300; // القيمة الدنيا لتحفيز المضخة (تحتاج لتعديل هذه القيمة حسب حساس الماء)

#line 5 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\sketch_sep18a.ino"
void setup();
#line 10 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\sketch_sep18a.ino"
void loop();
#line 5 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\sketch_sep18a.ino"
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

#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\aa.ino"
const int potPin = A3; 
const int ledPin = 3;

void setup() {
    Serial.begin(9600); 
    pinMode(ledPin, OUTPUT);
}

void loop() {
    int sensorValue = analogRead(potPin); 
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue); 

    int dutyCycle = map(sensorValue, 0, 1023, 0, 255);

    analogWrite(ledPin, dutyCycle); 

    delay(100);
}
#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\sketch_sep17a.ino"
const int sensorPin = A1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
 
  float voltage = sensorValue * (5.0 / 1023);
  
  float temperature = voltage * 100; 
  
  Serial.print(temperature);
  Serial.println(" °C");
  
  delay(1000);
}

