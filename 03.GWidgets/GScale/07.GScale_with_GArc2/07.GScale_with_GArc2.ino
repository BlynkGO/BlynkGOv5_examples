#include <BlynkGOv5.h>

FONT_DECLARE(BebasNeueBold_num_45);

GRect rect_gauge;
  GArc   arc(rect_gauge);
  GScale scale(rect_gauge);
  GLabel label(rect_gauge);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect_gauge.size(330, 180);
  rect_gauge.radius(10);
  rect_gauge.color(TFT_WHITE);
  rect_gauge.align(ALIGN_TOP_LEFT, 20, 20);

    arc.size(300,300);
    arc.thickness(50);
    arc.range(20,180);
    arc.angle_offset(180);
    arc.angle_range(180);
    arc.bg_enable(true);
    arc.position(15,15);
    arc.color(TFT_PALETTE(TFT_PALETTE_RED));

    arc = 157;

    scale.size(298, 298);                       // ขนาดเล็กกว่า arc เล็กน้อย
    scale.mode(SCALE_MODE_ROUND_INNER, 81, 20); // scale แบบ โค้ง ภายใน มีเส้นขีด ทั้งหมด 41 ขีด เส้นขีดหลัก เว้นทุกๆ 10 ขีด
    scale.range(arc);                           // มี range เดียวกับ arc
    scale.angle_offset(arc);                    // มีมุมเริ่มต้น เดียวกับ arc
    scale.angle_range(arc);                     // มีช่วงมุม เดียวกับ arc
    scale.label_show(true);
    scale.tick_length(10,5);
    scale.axis_thickness(0);
    scale.align(arc, ALIGN_CENTER);             // จัดตำแหน่งด้วย align ไว้กลาง arc

    label.font(BebasNeueBold_num_45);
    label.align(arc, ALIGN_CENTER, 0, -label.font_height()/2);
    label = arc.toInt();
}



void loop(){
  BlynkGO.update();
}
