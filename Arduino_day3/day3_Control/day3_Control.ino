#include <VitconBrokerComm.h>
using namespace vitcon;
#include <SoftPWM.h>

#define LAMP 17
#define PUMP 16
SOFTPWM_DEFINE_CHANNEL(A3);

bool fan_out_status;
bool pump_out_status;
bool lamp_out_status;

void fan_out(bool val){
  fan_out_status = val;
}

void pump_out(bool val){
  pump_out_status = val;
}

void lamp_out(bool val){
  lamp_out_status = val;
}

IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);
IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);
IOTItemBin LampStatus;
IOTItemBin Lamp(lamp_out);

#define ITEM_COUNT 6

IOTItem *items[ITEM_COUNT] = {&FanStatus, &Fan,
                              &PumpStatus, &Pump,
                              &LampStatus, &Lamp
                              };

const char device_id[] = "05009e264ebe0020892ef36583d73129";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);

  SoftPWM.begin(490);
  pinMode(LAMP, OUTPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  if(fan_out_status == true) SoftPWM.set(100);
  else SoftPWM.set(0);
  digitalWrite(PUMP, pump_out_status);
  digitalWrite(LAMP, lamp_out_status);

  FanStatus.Set(fan_out_status);
  LampStatus.Set(digitalRead(LAMP));
  PumpStatus.Set(digitalRead(PUMP));

  comm.Run();
}
