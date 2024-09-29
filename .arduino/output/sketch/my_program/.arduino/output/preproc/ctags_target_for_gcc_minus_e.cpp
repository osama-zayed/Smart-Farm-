#line 1 "D:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\sketch_sep18a\\my_program\\.arduino\\output\\preproc\\ctags_target_for_gcc_minus_e.cpp"
# 1 "d:\\الابتوب الثاني\\برمجة\\الجامعة\\level 4\\سنه رابعه الفصل الاول\\انظمة مدمجة\\my_program\\LED Blinking\\7_Segment\\7_Segment.ino"
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