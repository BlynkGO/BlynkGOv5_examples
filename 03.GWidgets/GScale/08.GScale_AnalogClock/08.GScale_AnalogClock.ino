#include <BlynkGOv5.h>

IMAGE_DECLARE(img_hand_hour);
IMAGE_DECLARE(img_hand_min);

GScale scale;
GLabel label;

GImage pin_h(img_hand_hour, scale);
GImage pin_m(img_hand_min, scale);
GLine  pin_s(scale);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  scale.size(210,210);
  scale.mode(SCALE_MODE_ROUND_INNER, 61,5);  // scale แบบ โค้ง ภายใน มีเส้นขีด ทั้งหมด 61 ขีด เส้นขีดหลัก เว้นทุกๆ 5 ขีด
  scale.range(0, 12);
  scale.angle_offset(270);
  scale.angle_range(360);
  scale.label_show(true);
  scale.tick_length(10,5);
  scale.axis_thickness(1);

  static const char * custom_labels[] = {"", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",  NULL};
  scale.text_src(custom_labels);                // ข้อความ ที่ เส้นขีดหลัก

  label.align(scale, ALIGN_TOP,0,-10);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    label = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());

    int h = hour();
    int m = minute();
    int s = second();
    pin_h.pivot(4,77);  pin_h.align(ALIGN_BOTTOM, 0, - scale.height()/2 + pin_h.height() - pin_h.pivot().y);  // กำหนดจุดหมุน และ กำหนดตำแหน่งขณะยังไม่หมุน
    pin_m.pivot(4,96);  pin_m.align(ALIGN_BOTTOM, 0, - scale.height()/2 + pin_m.height() - pin_m.pivot().y);  // กำหนดจุดหมุน และ กำหนดตำแหน่งขณะยังไม่หมุน
    pin_h.angle( (h * 1.0 + m / 60.0 + s / 3600.0) * 5 * 6 ); // หมุนเข็มยาว จากขณะยังไม่หมุนไปกี่องศา
    pin_m.angle( (m * 1.0 + s / 60.0) * 6 );                  // หมุนเข็มยาว จากขณะยังไม่หมุนไปกี่องศา
    pin_s.setPoints(80, s*6, 270, scale.centerpoint() );      // หมุน GLine เข็มสั้น ยาว 80px ไป s*6 องศา จากมุมเริ่มต้น 270 องศา โดยมีจุด scale.centerpoint() เป็นจุดหมุน
  }, true);

  pin_s.color(TFT_RED);
  pin_s.thickness(2);
}

void loop(){
  BlynkGO.update();
}
