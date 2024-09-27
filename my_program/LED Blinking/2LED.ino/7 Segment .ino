void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
}

void loop()
{
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
}


















// void setup() {
//   pinMode(2, OUTPUT); // A
//   pinMode(3, OUTPUT); // B
//   pinMode(4, OUTPUT); // C
//   pinMode(5, OUTPUT); // D
//   pinMode(6, OUTPUT); // E
//   pinMode(7, OUTPUT); // F
//   pinMode(8, OUTPUT); // G
// }

// void loop() {
//   for (int i = 1; i <= 9; i++) {
//     displayNumber(i);
//     delay(1000); // Display each number for 1 second
//   }
// }

// void displayNumber(int num) {
//   switch (num) {
//     case 1:
//       displayDigit(0, 1, 1, 0, 0, 0, 0);
//       break;
//     case 2:
//       displayDigit(1, 1, 0, 1, 1, 0, 1);
//       break;
//     case 3:
//       displayDigit(1, 1, 1, 1, 0, 0, 1);
//       break;
//     case 4:
//       displayDigit(0, 1, 1, 0, 0, 1, 1);
//       break;
//     case 5:
//       displayDigit(1, 0, 1, 1, 0, 1, 1);
//       break;
//     case 6:
//       displayDigit(1, 0, 1, 1, 1, 1, 1);
//       break;
//     case 7:
//       displayDigit(1, 1, 1, 0, 0, 0, 0);
//       break;
//     case 8:
//       displayDigit(1, 1, 1, 1, 1, 1, 1);
//       break;
//     case 9:
//       displayDigit(1, 1, 1, 1, 0, 1, 1);
//       break;
//   }
// }

// void displayDigit(int a, int b, int c, int d, int e, int f, int g) {
//   digitalWrite(2, a);
//   digitalWrite(3, b);
//   digitalWrite(4, c);
//   digitalWrite(5, d);
//   digitalWrite(6, e);
//   digitalWrite(7, f);
//   digitalWrite(8, g);
// }





// void setup() {
//   pinMode(2, OUTPUT); // A
//   pinMode(3, OUTPUT); // B 
//   pinMode(4, OUTPUT); // C
//   pinMode(5, OUTPUT); // D
//   pinMode(6, OUTPUT); // E
//   pinMode(7, OUTPUT); // F
//   pinMode(8, OUTPUT); // G
// }

// void loop() {
//   for (int i = 1; i <= 9; i++) {
//     displayNumber(i);
//     delay(1000); // Display each number for 1 second
//     clearDisplay(); // Turn off all segments
//   }
// }

// void displayNumber(int num) {
//   switch (num) {
//     case 1:
//       displayDigit(0, 1, 1, 0, 0, 0, 0);
//       break;
//     case 2:
//       displayDigit(1, 1, 0, 1, 1, 0, 1);
//       break;
//     case 3:
//       displayDigit(1, 1, 1, 1, 0, 0, 1);
//       break;
//     case 4:
//       displayDigit(0, 1, 1, 0, 0, 1, 1);
//       break;
//     case 5:
//       displayDigit(1, 0, 1, 1, 0, 1, 1);
//       break;
//     case 6:
//       displayDigit(1, 0, 1, 1, 1, 1, 1);
//       break;
//     case 7:
//       displayDigit(1, 1, 1, 0, 0, 0, 0);
//       break;
//     case 8:
//       displayDigit(1, 1, 1, 1, 1, 1, 1);
//       break;
//     case 9:
//       displayDigit(1, 1, 1, 1, 0, 1, 1);
//       break;
//   }
// }

// void displayDigit(int a, int b, int c, int d, int e, int f, int g) {
//   digitalWrite(2, a);
//   digitalWrite(3, b);
//   digitalWrite(4, c);
//   digitalWrite(5, d);
//   digitalWrite(6, e);
//   digitalWrite(7, f);
//   digitalWrite(8, g);
// }

// void clearDisplay() {
//   digitalWrite(2, LOW);
//   digitalWrite(3, LOW);
//   digitalWrite(4, LOW);
//   digitalWrite(5, LOW);
//   digitalWrite(6, LOW);
//   digitalWrite(7, LOW);
//   digitalWrite(8, LOW);
// }