#include <VitconBrokerComm.h>
#include <SoftPWM.h>
using namespace vitcon;
SOFTPWM_DEFINE_CHANNEL(A3);

bool timeset = false;
bool Interval_Mup_status;
bool Interval_Hup_status;
bool Fan_Status;
int PWM_status = 60;

int Hour = 0;
int Minute = 1;
uint32_t TimeSum = 0;
uint32_t TimeCompare;

uint32_t TimePushDelay = 0;
uint32_t TimerStartTime = 0;

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

void PWM_Control(int32_t val){
  PWM_status = (int)val;
}

void Fan_out_status(bool val){
  Fan_Status = val;
}

IOTItemBin FanStatus;
IOTItemBin Fan(Fan_out_status);

IOTItemBin StopStatus;
IOTItemBin Stop(timeset_out);

IOTItemInt PWMStatus;
IOTItemInt PWM(PWM_Control);
IOTItemInt PBar;

IOTItemBin IntervalHUP(Interval_Hup);
IOTItemBin IntervalMUP(Interval_Mup);
IOTItemBin IntervalRST(IntervalReset);

IOTItemInt label_Hinterval;
IOTItemInt label_Minterval;

#define ITEM_COUNT 12

IOTItem*items[ITEM_COUNT] = {&FanStatus, &Fan,
                             &PWMStatus, &PWM, &PBar,
                             &StopStatus, &Stop,
                             &label_Hinterval, &label_Minterval,
                             &IntervalHUP, &IntervalMUP, &IntervalRST
                            };

const char device_id[]="05009e264ebe0020892ef36583d73129";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  Serial.begin(250000);
  comm.SetInterval(200);

  SoftPWM.begin(490);
}

void loop() {
  IntervalSet(timeset);

  if(Fan_Status == true){
    SoftPWM.set(PWM_status);
    if(timeset){
      if(TimeCompare % 2 == 1){
        SoftPWM.set(0);
      }
    }
  }else SoftPWM.set(0);
  
  StopStatus.Set(timeset);
  label_Hinterval.Set(Hour);
  label_Minterval.Set(Minute);
  PBar.Set(PWM_status);
  comm.Run();
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
