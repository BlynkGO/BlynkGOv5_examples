#include <BlynkGOv5.h>

GScale scale;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  scale.size(PCT(80),100);  // กว้าง 80% ของหน้าจอ สูง 100px
  scale.range(10,40);       // scale ช่วงระหว่าง 10-40
  scale.mode(SCALE_MODE_HORIZONTAL_BOTTOM,31,5);  // แบบแนวนอนด้านล่าง มีเส้นขีด ทั้งหมด 31 ขีด เส้นขีดหลัก เว้นทุกๆ 5 ขีด
  scale.tick_length(10,5);  // ความยาว เส้นขีดหลัก 10px ความยาวเส้นขีดทั่วไป 5px
  scale.label_show(true);   // แสดงข้อความที่เส้นขีดหลักด้วย

  scale.color(TFT_WHITE);
  scale.font_color(TFT_WHITE);
}

void loop(){
  BlynkGO.update();
}

