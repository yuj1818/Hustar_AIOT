#include <U8g2lib.h>
#include "DHT.h"
#include <SoftPWM.h>
#include <VitconBrokerComm.h>
using namespace vitcon;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define PUMP 16

SOFTPWM_DEFINE_CHANNEL(A3);
DHT dht(DHTPIN, DHTTYPE);

bool fan_out_status = false;
bool pump_out_status = false;

void fan_out(bool val){
  fan_out_status = val;
}

void pump_out(bool val){
  pump_out_status = val;
}

#define ITEM_COUNT 7

IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt soilhumi;
IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);
IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);

IOTItem *items[ITEM_COUNT] = {&dht22_temp, &dht22_humi, &soilhumi,
                              &FanStatus, &Fan,
                              &PumpStatus, &Pump
                              };
                              
int Soilhumi = 0;
int PWM = 0;
float Temp;
float Humi;
String Pump_Status = "OFF";

const char device_id[] = "05009e264ebe0020892ef36583d73129";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  TempMeasure();
  SoilHumiMeasure();

  //확인용
  //OLEDdraw();

  FanStatus.Set(fan_out_status);
  PumpStatus.Set(digitalRead(PUMP));
  comm.Run();
}

void TempMeasure(){
  
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  
  if (isnan(Humi) || isnan(Temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  PWM = 0;
  if(fan_out_status == true){
    if (Temp>=29){
      PWM = 100;
    } else if (Temp>20) {
      PWM = 65;
    }
  }
  SoftPWM.set(PWM);
  
}

void SoilHumiMeasure(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  soilhumi.Set(Soilhumi);
  if(pump_out_status == true){
    if (Soilhumi<=30){
      digitalWrite(PUMP,HIGH);
      Pump_Status = "ON";
    } else if (Soilhumi>=60){
      digitalWrite(PUMP,LOW);
      Pump_Status = "OFF";
    }
  }else{
    digitalWrite(PUMP, LOW);
    Pump_Status = "OFF";
  }
}

//확인용
//void OLEDdraw() {
//  u8g2.clearBuffer();
//
//  u8g2.setFont(u8g2_font_ncenB08_te);
//
//  u8g2.drawStr(15, 10, "Temp. ");
//  u8g2.setCursor(85, 10);
//  u8g2.print(Temp);
//  u8g2.drawStr(114, 10, "\xb0");
//  u8g2.drawStr(119, 10, "C");
//
//  u8g2.drawStr(15, 20, "Humidity");
//  u8g2.setCursor(85, 20); u8g2.print(Humi);
//  u8g2.drawStr(116, 20, "%");
//
//  u8g2.drawStr(15, 30, "SoilHumi");
//  u8g2.setCursor(85, 30); u8g2.print(Soilhumi);
//  u8g2.drawStr(116, 30, "%");
//
//  u8g2.drawStr(15, 40, "FAN/PUMP");
//  u8g2.setCursor(40, 50); u8g2.print(PWM);u8g2.print("/");u8g2.print(Pump_Status);
//
//  u8g2.setCursor(40, 60); u8g2.print(fan_out_status);
// 
//  u8g2.sendBuffer();
//}
