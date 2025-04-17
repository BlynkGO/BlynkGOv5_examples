#include <BlynkGOv5.h>

FONT_DECLARE(MontserratBold_num_50);

GArc arc[5];
uint8_t arc_level[] = {0, 40, 60, 70, 80, 100};                             // แบ่ง range : 0-100 เป็น 5 ช่วงระดับ
color_t arc_color[] = {TFT_BLUE, TFT_CYAN, TFT_GREEN, TFT_YELLOW, TFT_RED}; // สี arc สำหรับ 5 ช่วงระดับ
GArc arc_knob;
GLabel label(arc_knob);


void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  for(int i = 0; i < 5; i++) {
    arc[i].size(200, 200);
    arc[i].angle_offset(135);
    arc[i].angle(map(arc_level[i], 0, 100, 0, 270), map(arc_level[i + 1], 0, 100, 0, 270)); // เปลียน range: 0-100 เป็น 0-270 องศา
    arc[i].color(arc_color[i]);         // กำหนดสี ด้วย array สี ที่เตรียมไว้
    arc[i].align(ALIGN_CENTER, 0, 20);
  }

  arc_knob.size(200, 200);
  arc_knob.opa(0);                      // มองไม่เห็น arc
  arc_knob.angle_offset(135);           // มุมเริ่มต้น 135 องศา
  arc_knob.bg_angle(0, 270);            // มุม พื้นหลัง 0-270 องศา จากมุมเริ่มต้น  (แต่มองไม่เห็น)
  arc_knob.align(arc[0], ALIGN_CENTER); // วางทับ arc[0] ตรงกลาง
  arc_knob.knob.enable(true);         // ให้ ปุ่ม knob เห็น ทำงานขึ้นมา
  arc_knob.knob.color(TFT_WHITE);     // สีของ ปุ่ม knob
  arc_knob.knob.border(3, TFT_BLACK); // กำหนดขอบ และ สีขอบ ปุ่ม knob

  // arc_knob.knob.clickable(false);
  arc_knob = 50;
  arc_knob.onValueChanged(GWIDGET_CB {
    label = arc_knob.toInt();
  });
  label.font(MontserratBold_num_50, TFT_BLACK);

}

void loop()
{
  BlynkGO.update();
}

