#include <BlynkGOv5.h>

GLed led1, led2, led3;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  led1.align(ALIGN_CENTER,-80);
  led1.OFF();

  led2.brightness(150);   // 0 dark  255 light
  led2.color(TFT_PALETTE(TFT_PALETTE_RED));

  led3.align(ALIGN_CENTER,80);
  led3.ON();
}

void loop(){
  BlynkGO.update();
}
