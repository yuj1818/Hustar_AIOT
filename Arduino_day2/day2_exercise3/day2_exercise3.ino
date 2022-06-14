#define PUMP 16
#define SOILHUMI A6

int Soilhumi = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);

  if (Soilhumi<=30){
    digitalWrite(PUMP,HIGH);
  } else if (Soilhumi>=60){
    digitalWrite(PUMP,LOW);
  }
}
