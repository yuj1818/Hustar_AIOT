#include <U8g2lib.h>
#include "DHT.h"
#include <SoftPWM.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define LAMP 17
#define PUMP 16

SOFTPWM_DEFINE_CHANNEL(A3);
DHT dht(DHTPIN, DHTTYPE);

int Soilhumi = 0;
uint32_t DataCaptureDelay = 2000;
uint32_t DataCapture_ST = 0;
int PWM = 0;
float Temp;
float Humi;
String Pump = "OFF";
uint32_t TimeCompare;
uint32_t StartTime = 0;
uint32_t TimeSum;
int Hour = 0;
int Minute = 1;
String LED = "OFF";

void setup() {
  Serial.begin(9600);
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);
  pinMode(SOILHUMI, INPUT);
  DataCapture_ST = millis();
  pinMode(LAMP, OUTPUT);
  TimeSum = (uint32_t)(Hour*60+Minute)*60*1000;
  StartTime = millis();
}

void loop() {
  TempMeasure();
  SoilHumiMeasure();
  LampAct();
}

void TempMeasure(){
  if ((millis() - DataCapture_ST) > DataCaptureDelay) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();

    if (isnan(Humi) || isnan(Temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  
    DataCapture_ST=millis();
  }

  if (Temp>=29){
    PWM = 100;
  } else if (Temp>20) {
    PWM = 65;
  } else {
    PWM = 0;
  }
  SoftPWM.set(PWM);
  OLEDdraw();
}

void SoilHumiMeasure(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  delay(3000);
  if (Soilhumi<=30){
    digitalWrite(PUMP,HIGH);
    Pump = "ON";
  } else if (Soilhumi>=60){
    digitalWrite(PUMP,LOW);
    Pump = "OFF";
  }
  OLEDdraw();
}

void LampAct(){
  TimeCompare = (millis()-StartTime)/TimeSum;

  if(TimeCompare%2==0){
    digitalWrite(LAMP,LOW);
    LED = "OFF";
  }else if(TimeCompare%2==1){
    digitalWrite(LAMP,HIGH);
    LED = "ON";
  }
  OLEDdraw();
}
void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);

  u8g2.drawStr(15, 10, "Temp. ");
  u8g2.setCursor(85, 10);
  u8g2.print(Temp);
  u8g2.drawStr(114, 10, "\xb0");
  u8g2.drawStr(119, 10, "C");

  u8g2.drawStr(15, 20, "Humidity");
  u8g2.setCursor(85, 20); u8g2.print(Humi);
  u8g2.drawStr(116, 20, "%");

  u8g2.drawStr(15, 30, "SoilHumi");
  u8g2.setCursor(85, 30); u8g2.print(Soilhumi);
  u8g2.drawStr(116, 30, "%");

  u8g2.drawStr(15, 40, "FAN/PUMP/LED");
  u8g2.setCursor(40, 50); u8g2.print(PWM);u8g2.print("/");u8g2.print(Pump);u8g2.print("/");u8g2.print(LED);

  u8g2.drawStr(15, 60, "LED interval");
  u8g2.setCursor(95, 60); u8g2.print(TimeSum);
  
  u8g2.sendBuffer();
}
