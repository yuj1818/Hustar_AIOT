#include <VitconBrokerComm.h>
#include <SoftPWM.h>
#include "DHT.h"

using namespace vitcon;
SOFTPWM_DEFINE_CHANNEL(A3);

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define PUMP 16
#define LAMP 17

DHT dht(DHTPIN, DHTTYPE);

bool timeset = false;
bool Interval_Mup_status;
bool Interval_Hup_status;
bool Fan_Status;
bool Pump_Status;
bool Lamp_Status;
bool Device_Status;

int Hour = 0;
int Minute = 1;
uint32_t TimeSum = 0;
uint32_t TimeCompare;

uint32_t TimePushDelay = 0;
uint32_t TimerStartTime = 0;

int Soilhumi = 0;
float Temp;
float Humi;

int PWM = 0;
bool Pump_OnOff = LOW;

void timeset_out(bool val){
  timeset = val;
}

void Interval_Hup(bool val){
  Interval_Hup_status = val;
}

void Interval_Mup(bool val){
  Interval_Mup_status = val;
}

void IntervalReset(bool val){
  if(!timeset&&val){
    Hour = 0;
    Minute = 0;
  }
}

void Fan_out_status(bool val){
  Fan_Status = val;
}

void Pump_out_status(bool val){
  Pump_Status = val;
}

void Lamp_out_status(bool val){
  Lamp_Status = val;
}

void Device_auto(bool val){
  Device_Status = val;
}

IOTItemBin DeviceStatus;
IOTItemBin Device(Device_auto);

IOTItemBin StopStatus;
IOTItemBin Stop(timeset_out);

IOTItemBin FanStatus;
IOTItemBin Fan(Fan_out_status);

IOTItemBin PumpStatus;
IOTItemBin Pump(Pump_out_status);

IOTItemBin LampStatus;
IOTItemBin Lamp(Lamp_out_status);

IOTItemBin IntervalHUP(Interval_Hup);
IOTItemBin IntervalMUP(Interval_Mup);
IOTItemBin IntervalRST(IntervalReset);

IOTItemInt label_Hinterval;
IOTItemInt label_Minterval;

IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt soilhumi;

#define ITEM_COUNT 18

IOTItem *items[ITEM_COUNT] = {&DeviceStatus, &Device,
                              &StopStatus, &Stop,
                              &FanStatus, &Fan,
                              &PumpStatus, &Pump,
                              &LampStatus, &Lamp,
                              &IntervalHUP, &IntervalMUP, &IntervalRST,
                              &label_Hinterval, &label_Minterval,
                              &dht22_temp, &dht22_humi, &soilhumi
                              };
                            
const char device_id[]="05009e264ebe0020892ef36583d73129";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);
  dht.begin();
  SoftPWM.begin(490);
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LAMP, OUTPUT);
}

void loop() {
  IntervalSet(timeset);
  TempMeasure();
  SoilHumiMeasure();
  LampControl();

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  soilhumi.Set(Soilhumi);
  StopStatus.Set(timeset);
  FanStatus.Set(PWM);
  PumpStatus.Set(digitalRead(PUMP));
  LampStatus.Set(digitalRead(LAMP));
  label_Hinterval.Set(Hour);
  label_Minterval.Set(Minute);
  comm.Run();
}

void FanAuto(){
  if (Temp>=29){
    PWM = 100;
  } else if (Temp>20) {
    PWM = 65;
  }
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

  if(Device_Status == true) FanAuto();
  else{
    if(Fan_Status == true){
      PWM = 70;
    }else{
      PWM = 0;
    }
  }
  
  SoftPWM.set(PWM);
  
}

void PumpAuto(){
  if (Soilhumi<=30){
    Pump_OnOff = HIGH;
  } else if (Soilhumi>=60){
    Pump_OnOff = LOW;
  }
}

void SoilHumiMeasure(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  soilhumi.Set(Soilhumi);
  
  if(Device_Status == true) PumpAuto();
  else{
    if(Pump_Status == true){
      Pump_OnOff = HIGH;
    }else{
      Pump_OnOff = LOW;
    }
  }
  digitalWrite(PUMP,Pump_OnOff);
}

void IntervalSet(bool timeset){
  if(!timeset){
    TimeSum = (uint32_t)(Hour*60 + Minute)*60*1000;
    TimerStartTime = millis();

    if(millis() > TimePushDelay + 500){
      Hour += Interval_Hup_status;
      if(Hour>=24) Hour = 0;
      Minute += Interval_Mup_status;
      if(Minute>=60) Minute = 0;

      TimePushDelay = millis();
    }
  }
  else if(timeset){
    TimeCompare = (millis()-TimerStartTime)/TimeSum;
  }
}

void LampTimer(){
  if(timeset){
    if(TimeCompare % 2 == 0){
      digitalWrite(LAMP, LOW);
    }
    else if(TimeCompare % 2 == 1){
      digitalWrite(LAMP, HIGH);
    }
  }
  else if(!timeset){
    digitalWrite(LAMP,LOW);
  }
}

void LampControl(){
  if(Device_Status == true) LampTimer();
  else{
    if(Lamp_Status == true){
      digitalWrite(LAMP,HIGH);
    }else{
      digitalWrite(LAMP,LOW);
    }
  }
}
