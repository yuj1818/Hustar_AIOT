#define PUMP 16

void setup() {
  pinMode(PUMP, OUTPUT);
}

void loop() {
//  digitalWrite(PUMP,HIGH);
//  delay(2000);
  digitalWrite(PUMP,LOW);
  delay(2000);
}
