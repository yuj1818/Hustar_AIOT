#include <U8g2lib.h>
#include "DHT.h"
#include <SoftPWM.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22

SOFTPWM_DEFINE_CHANNEL(A3);
DHT dht(DHTPIN, DHTTYPE);

uint32_t DataCaptureDelay = 3000;
uint32_t DataCapture_ST = 0;
int PWM = 0;
float Temp;

void setup() {
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);
  DataCapture_ST = millis();
}

void loop() {
  if ((millis() - DataCapture_ST) > DataCaptureDelay) {
    Temp = dht.readTemperature();
  
    if (isnan(Temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    
    DataCapture_ST=millis();
  }
  if (Temp>=30){
    PWM = 100;
  } else if (Temp>25) {
    PWM = 65;
  } else {
    PWM = 0;
  }
  SoftPWM.set(PWM);
  OLEDdraw();
}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "SMART FARM");

  u8g2.drawStr(15, 36, "Temp. ");
  u8g2.setCursor(85, 36);
  u8g2.print(Temp);
  u8g2.drawStr(114, 36, "\xb0");
  u8g2.drawStr(119, 36, "C");

  u8g2.drawStr(15, 47, "PWM");
  u8g2.setCursor(85, 47); u8g2.print(PWM);

  u8g2.sendBuffer();
}
