#define LAMP 17

uint32_t TimeCompare;
uint32_t StartTime = 0;

uint32_t TimeSum;
int Hour = 0;
int Minute = 1;

void setup() {
  pinMode(LAMP, OUTPUT);
  TimeSum = (uint32_t)(Hour*60+Minute)*60*100;
  StartTime = millis();
}

void loop() {
  TimeCompare = (millis()-StartTime)/TimeSum;

  if(TimeCompare%2==0){
    digitalWrite(LAMP,LOW);
  }else if(TimeCompare%2==1){
    digitalWrite(LAMP,HIGH);
  }
}
