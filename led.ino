const int pumpPin = 7; 

void setup()
{
    pinMode(pumpPin, OUTPUT);
}

void loop()
{

    digitalWrite(pumpPin, HIGH);
    delay(1000);

    digitalWrite(pumpPin, LOW);

    delay(1000);
}
