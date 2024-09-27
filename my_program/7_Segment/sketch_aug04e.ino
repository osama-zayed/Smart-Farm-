void setup() {
pinMode(2,OUTPUT);//A
pinMode(3,OUTPUT);//B
pinMode(4,OUTPUT);//C
pinMode(5,OUTPUT);//D
pinMode(6,OUTPUT);//E
pinMode(7,OUTPUT);//F
pinMode(8,OUTPUT);//G
}

void loop() {
 for (int i = 0; i <= 9; i++) {
    displayNumber(i);
    delay(1000);
  displayDigit(HIGH ,HIGH,HIGH,HIGH,HIGH,HIGH ,HIGH);
  }
}

void displayDigit(int a, int b, int c, int d, int e, int f, int g) {
  digitalWrite(2, a);
  digitalWrite(3, b);
  digitalWrite(4, c);
  digitalWrite(5, d);
  digitalWrite(6, e);
  digitalWrite(7, f);
  digitalWrite(8, g);
}

void displayNumber(int i){
    if(i == 0){
    displayDigit(LOW ,LOW,LOW,LOW,LOW,LOW ,HIGH);
  }
  if(i == 1){
    displayDigit(HIGH ,HIGH,HIGH,HIGH,LOW,LOW ,HIGH);
  }
   if(i == 2){
    displayDigit(LOW ,LOW,HIGH,LOW,LOW,HIGH ,LOW);
  }
 if(i == 3){
    displayDigit(LOW ,LOW,LOW,LOW,HIGH,HIGH ,LOW);
  } if(i ==4){
    displayDigit(HIGH ,LOW,LOW,HIGH,HIGH,LOW ,LOW);
  } if(i == 5){
    displayDigit(LOW ,HIGH,LOW,LOW,HIGH,LOW ,LOW);
  } if(i == 6){
    displayDigit(LOW ,HIGH,LOW,LOW,LOW,LOW ,LOW);
  } if(i == 7){
    displayDigit(LOW ,LOW,LOW,HIGH,HIGH,HIGH ,HIGH);
  } if(i == 8){
    displayDigit(LOW ,LOW,LOW,LOW,LOW,LOW ,LOW);
  } if(i == 9){
    displayDigit(LOW ,LOW,LOW,LOW,HIGH,LOW ,LOW);
  }

}
