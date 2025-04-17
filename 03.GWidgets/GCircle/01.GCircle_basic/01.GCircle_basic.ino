#include <BlynkGOv5.h>

GCircle circle;
  GCircle cir_dot[12];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE(TFT_PALETTE_LIME));

  circle.radius(100);
  circle.border(1,TFT_BLACK);

  for(int i=0; i<12;i++){
    cir_dot[i].parent(circle);
    cir_dot[i].radius(4);
    cir_dot[i].color(TFT_RED);
    // จุดที่สร้างจากระยะแขน circle.radius()-20 ไปเป็นมุม i*30 องศา ที่นับจากมุมเริ่มต้น 270 องศา รอบจุดศก.วงกลม โดยหมุนตามเข็ม
    Point p = Point(circle.radius()-20, i*30, 270, circle.centerpoint(), CLOCKWISE);
    cir_dot[i].center(p);
  }

}

void loop(){
  BlynkGO.update();
}

