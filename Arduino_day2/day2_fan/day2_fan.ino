#include <SoftPWM.h>

SOFTPWM_DEFINE_CHANNEL(A3);

void setup() {
  SoftPWM.begin(490);
}

void loop() {
  for (int i = 65; i < 100; i++)
  {
    SoftPWM.set(i);
    delay(100);
  }

  for (int i = 100; i > 65; i--)
  {
    SoftPWM.set(i);
    delay(100);
  }
  //SoftPWM.set(0);
}
