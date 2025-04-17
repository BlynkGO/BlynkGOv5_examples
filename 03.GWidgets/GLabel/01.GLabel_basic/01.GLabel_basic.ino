#include <BlynkGOv5.h>

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();

  BlynkGO.begin();

  label = "ขอต้อนรับสู่ BlynkGOv5";
  label.color(TFT_CYAN);

}

void loop(){
  BlynkGO.update();
}

