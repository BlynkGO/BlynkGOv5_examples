#include <BlynkGOv5.h>

IMAGE_DECLARE(img_background_box);

GRect rect;
  GLabel label(rect);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect.size(225,150);
  rect.border(3, TFT_RED);
  rect.image(img_background_box);   /* กำหนดภาพพื้นหลังไปได้เลย โดยไม่ต้องเปลืองเม็มในการสร้างวิตเจ็ต GImage */
    label = "ข้อความ";
    label.align(ALIGN_TOP);
}

void loop(){
  BlynkGO.update();
}

