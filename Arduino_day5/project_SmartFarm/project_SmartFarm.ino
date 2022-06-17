#include <VitconBrokerComm.h>
#include <SoftPWM.h>
#include <U8g2lib.h>
#include "DHT.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

using namespace vitcon;
SOFTPWM_DEFINE_CHANNEL(A3);

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define PUMP 16
#define LAMP 17

DHT dht(DHTPIN, DHTTYPE);

bool fan_onoff_status = false;
int fan_timer_status = 0;
bool fan_AUTO_status = false;
int fan_AUTO_th1_status = 27;
int fan_AUTO_th2_status = 29;
bool fan_AUTO_th1_P_status = false;
bool fan_AUTO_th2_P_status = false;
bool fan_AUTO_RST_status = false;
bool pmp_onoff_status = false;
int32_t pmp_timer_status = 0;
bool pmp_AUTO_status = false;
int32_t pmp_AUTO_start_status = 10;
int32_t pmp_AUTO_stop_status = 30;
bool pmp_AUTO_interval_status = false;
bool pmp_AUTO_interval_onoff_status = false;
bool led_onoff_status = false;
int32_t led_timer_status = 0;
bool led_AUTO_status = false;
bool led_AUTO_H_status = false;
bool led_AUTO_M_status = false;
bool led_AUTO_S_status = false;
bool led_AUTO_SET_status = false;
bool led_AUTO_RST_status = false;

bool timeset = false;

int Hour = 0;
int Minute = 0;
int Sec = 1;
int _hour = 0;
int _minute = 0;
int _sec = 0;
int p_sec = 0;
uint32_t TimeSum = 0;
uint32_t TimeCompare;
uint32_t TimerStartTime = 0;

uint32_t pTimeSum = 0;
uint32_t pTimeCompare;
uint32_t pTimerStartTime = 0;

int Soilhumi = 0;
float Temp;
float Humi;

int PWM_th1 = 40;
int PWM_th2 = 40;
int PWM = 0;
bool Pump_OnOff = LOW;
bool led_OnOff = LOW;

int fan_runtime = 0;
uint32_t fan_uptime = 0;
int pump_runtime = 0;
uint32_t pmp_uptime = 0;
int led_runtime = 0;
uint32_t led_uptime = 0;

uint32_t pmp_interval_time = 0;
bool pmp_interval_flag = false;

String f_auto_str = "_";
String p_auto_str = "_";
String l_auto_str = "_";

String f_onoff_str = "X";
String p_onoff_str = "X";
String l_onoff_str = "X";

void fanOnOff(bool val);
void fanTimer(int32_t val);
void fanAuto(bool val);
void fanAutoTh1(int32_t val);
void fanAutoTh2(int32_t val);
void fanAutoTh1P(bool val);
void fanAutoTh2P(bool val);
void fanAutoRST(bool val);
void pmpOnOff(bool val);
void pmpTimer(int32_t val);
void pmpAuto(bool val);
void pmpAutoStart(int32_t val);
void pmpAutoStop(int32_t val);
void pmpAutoInterval(bool val);
void pmpAutoIntervalOnOff(bool val);
void ledOnOff(bool val);
void ledTimer(int32_t val);
void ledAuto(bool val);
void ledAutoH(bool val);
void ledAutoM(bool val);
void ledAutoS(bool val);
void ledAutoSET(bool val);
void ledAutoRST(bool val);

IOTItemFlo temp_label;
IOTItemFlo humi_label;
IOTItemInt soilhumi_label;
IOTItemBin FAN_led;
IOTItemBin FAN_onoff(fanOnOff);
IOTItemInt FAN_timer; 
IOTItemInt FAN_timer_value(fanTimer);
IOTItemBin FAN_AUTO;
IOTItemBin FAN_AUTO_status(fanAuto);
IOTItemInt FAN_AUTO_th1;
IOTItemInt FAN_AUTO_th1_value(fanAutoTh1);
IOTItemInt FAN_AUTO_th1_P;
IOTItemBin FAN_AUTO_th1_P_value(fanAutoTh1P);
IOTItemInt FAN_AUTO_th2;
IOTItemInt FAN_AUTO_th2_value(fanAutoTh2);
IOTItemInt FAN_AUTO_th2_P;
IOTItemBin FAN_AUTO_th2_P_value(fanAutoTh2P);
IOTItemBin FAN_AUTO_RST(fanAutoRST);
IOTItemBin PMP_led;
IOTItemBin PMP_onoff(pmpOnOff);
IOTItemInt PMP_timer;
IOTItemInt PMP_timer_value(pmpTimer);
IOTItemBin PMP_AUTO;
IOTItemBin PMP_AUTO_status(pmpAuto);
IOTItemInt PMP_AUTO_start;
IOTItemInt PMP_AUTO_start_value(pmpAutoStart);
IOTItemInt PMP_AUTO_stop;
IOTItemInt PMP_AUTO_stop_value(pmpAutoStop);
IOTItemBin PMP_AUTO_interval_btn(pmpAutoInterval);
IOTItemInt PMP_AUTO_interval;
IOTItemBin PMP_AUTO_interval_onoff;
IOTItemBin PMP_AUTO_interval_onoff_value(pmpAutoIntervalOnOff);
IOTItemBin LED_led;
IOTItemBin LED_onoff(ledOnOff);
IOTItemInt LED_timer;
IOTItemInt LED_timer_value(ledTimer);
IOTItemBin LED_AUTO;
IOTItemBin LED_AUTO_status(ledAuto);
IOTItemInt LED_AUTO_H_label;
IOTItemInt LED_AUTO_M_label;
IOTItemInt LED_AUTO_S_label;
IOTItemBin LED_AUTO_H_btn(ledAutoH);
IOTItemBin LED_AUTO_M_btn(ledAutoM);
IOTItemBin LED_AUTO_S_btn(ledAutoS);
IOTItemBin LED_AUTO_SET(ledAutoSET);
IOTItemBin LED_AUTO_RST(ledAutoRST);

#define ITEM_COUNT 46

IOTItem *items[ITEM_COUNT] = {&temp_label, &humi_label, &soilhumi_label,
                              &FAN_led, &FAN_onoff, &FAN_timer, &FAN_timer_value,
                              &FAN_AUTO, &FAN_AUTO_status,
                              &FAN_AUTO_th1, &FAN_AUTO_th1_value, &FAN_AUTO_th1_P, &FAN_AUTO_th1_P_value,
                              &FAN_AUTO_th2, &FAN_AUTO_th2_value, &FAN_AUTO_th2_P, &FAN_AUTO_th2_P_value,
                              &FAN_AUTO_RST,
                              &PMP_led, &PMP_onoff, &PMP_timer, &PMP_timer_value,
                              &PMP_AUTO, &PMP_AUTO_status, 
                              &PMP_AUTO_start, &PMP_AUTO_start_value,
                              &PMP_AUTO_stop, &PMP_AUTO_stop_value,
                              &PMP_AUTO_interval_btn, &PMP_AUTO_interval,
                              &PMP_AUTO_interval_onoff, &PMP_AUTO_interval_onoff_value,
                              &LED_led, &LED_onoff, &LED_timer, &LED_timer_value,
                              &LED_AUTO, &LED_AUTO_status,
                              &LED_AUTO_H_label, &LED_AUTO_M_label, &LED_AUTO_S_label,
                              &LED_AUTO_H_btn, &LED_AUTO_M_btn, &LED_AUTO_S_btn,
                              &LED_AUTO_SET, &LED_AUTO_RST
                              };
                            
const char device_id[]="05009e264ebe0020892ef36583d73129";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LAMP, OUTPUT);
}

void loop() {
  IntervalSet(led_AUTO_status);
  PumpIntervalSet(pmp_AUTO_interval_onoff_status);
  TempMeasure();
  SoilHumiMeasure();
  LampControl();

  FAN_led.Set(PWM);
  FAN_AUTO_th1.Set(fan_AUTO_th1_status);
  FAN_AUTO_th2.Set(fan_AUTO_th2_status);
  PMP_led.Set(digitalRead(PUMP));
  PMP_AUTO_start.Set(pmp_AUTO_start_status);
  PMP_AUTO_stop.Set(pmp_AUTO_stop_status);
  LED_led.Set(digitalRead(LAMP));
  LED_AUTO_H_label.Set(_hour);
  LED_AUTO_M_label.Set(_minute);
  LED_AUTO_S_label.Set(_sec);
  
  OLEDdraw();
  comm.Run();
}


void FanAuto(){
  if (Temp>=fan_AUTO_th1_status && Temp < fan_AUTO_th2_status){
    PWM = PWM_th1;
  } else if (Temp>=fan_AUTO_th2_status) {
    PWM = PWM_th2;
  }
  f_auto_str = "A";
}

void TempMeasure(){
  
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();

  temp_label.Set(Temp);
  humi_label.Set(Humi);
  
  if (isnan(Humi) || isnan(Temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  PWM = 0;
  if(fan_AUTO_status == true) FanAuto();
  else{
    if(fan_onoff_status == true){
      if((millis() - fan_uptime) < fan_runtime * 1000) PWM = 70;
    }
    f_auto_str = "_";
  }

  if(PWM != 0){
    f_onoff_str = "O";
  }else{
    f_onoff_str = "X";
  }
  
  SoftPWM.set(PWM);
  
}

void PumpIntervalSet(bool pmp_AUTO_interval_onoff_status){
  if(!pmp_AUTO_interval_onoff_status){
    pTimeSum = (uint32_t)(p_sec)*1000;
    pTimerStartTime = millis();
  }
  else if(pmp_AUTO_interval_onoff_status){
    pTimeCompare = (millis()-pTimerStartTime)/pTimeSum;
  }
}

void PumpAuto(){
  if (pmp_AUTO_interval_onoff_status == true){
//    if(((millis() - pmp_interval_time) < p_sec * 1000)){
//      if(pmp_interval_flag) Pump_OnOff = HIGH;
//    }else{
//      pmp_interval_flag = !pmp_interval_flag;
//      pmp_interval_time = millis();
//    }
    if(pTimeCompare % 2 == 0){
      Pump_OnOff = LOW;
    }
    else if(pTimeCompare % 2 == 1){
      Pump_OnOff = HIGH;
    }
  }else{
    if (Soilhumi<=pmp_AUTO_start_status){
      Pump_OnOff = HIGH;
    } else if (Soilhumi>=pmp_AUTO_stop_status){
      Pump_OnOff = LOW;
    } 
  }
  p_auto_str = "A";
}

void SoilHumiMeasure(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  soilhumi_label.Set(Soilhumi);

  Pump_OnOff = LOW;
  if(pmp_AUTO_status == true) PumpAuto();
  else{
    if(pmp_onoff_status == true){
      if((millis() - pmp_uptime) < pump_runtime * 1000) Pump_OnOff = HIGH;
    }
    p_auto_str = "_";
  }
  
  if(Pump_OnOff == HIGH){
    p_onoff_str = "O";
  }else{
    p_onoff_str = "X";
  }
  digitalWrite(PUMP,Pump_OnOff);
}

void IntervalSet(bool led_AUTO_status){
  if(!led_AUTO_status){
    TimeSum = (uint32_t)(Hour*360 + Minute*60+ Sec)*1000;
    TimerStartTime = millis();
  }
  else if(led_AUTO_status){
    TimeCompare = (millis()-TimerStartTime)/TimeSum;
  }
}

void LampTimer(){
  if(led_AUTO_status){
    if(TimeCompare % 2 == 0){
      led_OnOff = LOW;
    }
    else if(TimeCompare % 2 == 1){
      led_OnOff = HIGH;
    }
  }
  else if(!led_AUTO_status){
    led_OnOff = LOW;
  }

  l_auto_str = "A";
}

void LampControl(){
  led_OnOff = LOW;
  if(led_AUTO_status == true) LampTimer();
  else{
    if(led_onoff_status == true){
      if((millis() - led_uptime) < led_runtime * 1000) led_OnOff = HIGH;
    }
    l_auto_str = "_";
  }
  
  if(led_OnOff == HIGH){
    l_onoff_str = "O";
  }else{
    l_onoff_str = "X";
  }

  digitalWrite(LAMP,led_OnOff);
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

  u8g2.setCursor(15, 50); u8g2.print(f_auto_str);u8g2.print(" FAN ");u8g2.print(f_onoff_str);

  u8g2.setCursor(15, 60); u8g2.print(p_auto_str);u8g2.print(" PMP ");u8g2.print(p_onoff_str);
  u8g2.print(" / ");u8g2.print(l_auto_str);u8g2.print(" LED ");u8g2.print(l_onoff_str);
  
  u8g2.sendBuffer();
}

void fanOnOff(bool val){
  if(val){
    fan_onoff_status = !fan_onoff_status;
    fan_runtime = fan_timer_status;
    fan_uptime = millis();
  }
}

void fanTimer(int32_t val){
  fan_timer_status = val;
}

void fanAuto(bool val){
  fan_AUTO_status = val;
}

void fanAutoTh1(int32_t val){
  fan_AUTO_th1_status = (int)val;
}

void fanAutoTh2(int32_t val){
  fan_AUTO_th2_status = (int)val;
}

void fanAutoTh1P(bool val){
  fan_AUTO_th1_P_status = val;
  if(val){
    PWM_th1 += 20;
    if(PWM_th1>100) PWM_th1 = 60;
    FAN_AUTO_th1_P.Set(PWM_th1);
  }
}

void fanAutoTh2P(bool val){
  fan_AUTO_th2_P_status = val;
  if(val){
    PWM_th2 += 20;
    if(PWM_th2>100) PWM_th2 = 60;
    FAN_AUTO_th2_P.Set(PWM_th2);
  }
}

void fanAutoRST(bool val){
  fan_AUTO_RST_status = val;
  if(val){
    PWM_th1 = 0;
    PWM_th2 = 0;
    FAN_AUTO_th1_P.Set(PWM_th1);
    FAN_AUTO_th2_P.Set(PWM_th2);
  }
}

void pmpOnOff(bool val){
  if(val){
    pmp_onoff_status = !pmp_onoff_status;
    pump_runtime = pmp_timer_status;
    pmp_uptime = millis();
  }
}

void pmpTimer(int32_t val){
  pmp_timer_status = val;
}

void pmpAuto(bool val){
  pmp_AUTO_status = val;
}

void pmpAutoStart(int32_t val){
  pmp_AUTO_start_status = val;
}

void pmpAutoStop(int32_t val){
  pmp_AUTO_stop_status = val;
}

void pmpAutoInterval(bool val){
  pmp_AUTO_interval_status = val;
  if(val){
    p_sec += 1;
    if(p_sec >= 60) p_sec = 0;
  }
  PMP_AUTO_interval.Set(p_sec);
}

void pmpAutoIntervalOnOff(bool val){
  pmp_AUTO_interval_onoff_status = val;
}

void ledOnOff(bool val){
   if(val){
    led_onoff_status = !led_onoff_status;
    led_runtime = led_timer_status;
    led_uptime = millis();
  }
}

void ledTimer(int32_t val){
   led_timer_status = val;
}

void ledAuto(bool val){
   led_AUTO_status = val;
}

void ledAutoH(bool val){
   led_AUTO_H_status = val;
   if(val){
    _hour += 1;
    if(_hour >= 24) _hour = 0;
   }
}

void ledAutoM(bool val){
   led_AUTO_M_status = val;
   if(val){
    _minute += 1;
    if(_minute >= 60) _minute = 0;
   }
}

void ledAutoS(bool val){
   led_AUTO_S_status = val;
   if(val){
    _sec += 1;
    if(_sec >= 60) _sec = 0;
   }
}

void ledAutoSET(bool val){
   led_AUTO_SET_status = val;
   if(val){
    Hour = _hour;
    Minute = _minute;
    Sec = _sec;
   }
}

void ledAutoRST(bool val){
   led_AUTO_RST_status = val;
   if(val){
    _hour = 0;
    _minute = 0;
    _sec = 0;
   }
}
