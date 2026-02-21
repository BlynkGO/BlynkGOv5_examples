#include <BlynkGOv5.h>

IMAGE_DECLARE(img_bg);

GImage bg(img_bg);
  GLabel label(bg);   // label เป็นลูกของ bg 

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  bg.align(ALIGN_TOP,0,20);
    label = "ทดสอบรูป";
    
}

void loop(){
  BlynkGO.update();
}
