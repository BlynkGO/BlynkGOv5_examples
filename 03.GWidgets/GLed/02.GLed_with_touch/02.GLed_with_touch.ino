#include <BlynkGOv5.h>

GLed led;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  led.size(60,60);
  led.onClicked(GWIDGET_CB{
    led.toggle();                 // สลับสถานะ led
    Serial.println(led.isON());   // เช็คว่า ON หรือไม่
  });
}

void loop(){
  BlynkGO.update();
}

