#include <BlynkGOv5.h>

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  label = "ข้อความ";
  label.pivot(label.centerpoint());   // จุดหมุนที่จุดกึ่งกลางข้อความ
  label.rotation(60);                 // หมุนไป 60 องศา

}

void loop(){
  BlynkGO.update();
}

