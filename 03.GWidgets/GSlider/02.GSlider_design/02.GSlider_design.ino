#include <BlynkGOv5.h>

GSlider slider;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  slider.color(TFT_PALETTE(TFT_PALETTE_CYAN));                    // สี slider
  slider.bg_color(TFT_COLOR_HEX(0xbbb));                          // สีพื้นหลัง
  slider.knob_color(TFT_PALETTE(TFT_PALETTE_CYAN));               // สีปุ่มเลื่อน
  slider.knob_border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_CYAN,3));  // ขอบปุ่มเลื่อน
  slider.knob_padding(6);                                         // ขยายพื้นที่ปุ่ม
  slider.range(0,100);
  slider = 70;
  
}

void loop(){
  BlynkGO.update();
}
