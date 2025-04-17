#include <BlynkGOv5.h>

FONT_DECLARE(MontserratBold_num_50);

GArc arc[20];

GArc arc_knob;
GLabel label(arc_knob);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  for(int i = 0; i < 20; i++) {
    arc[i].size(200, 200);
    arc[i].angle_offset(90);
    arc[i].angle(270 / 20.0 * i, 270 / 20.0 * (i + 1) - 1);
    arc[i].color(TFT_COLOR_HSV(map(i, 0, 19, 240, 0), 100, 100));   // กำหนดสี ด้วย array สี ที่เตรียมไว้
    arc[i].thickness(20);
    arc[i].align(ALIGN_CENTER);
  }

  arc_knob.size(arc[0]);                // กำหนดขนาดให้เท่ากับ arc[0]
  arc_knob.thickness(arc[0].thickness());
  arc_knob.opa(0);                      // โปร่งใส มองไม่เห็น arc
  arc_knob.angle_offset(arc[0].angle_offset()); // มุมเริ่มต้นเท่ากับของ arc[0]
  arc_knob.bg_angle(0, 270);            // มุม พื้นหลัง 0-270 องศา จากมุมเริ่มต้น  (ปกติ arc พื้นหลังจะมองไม่เห็น จะไว้ใช้เป็นตัวกำหนดช่วงองศา)
  arc_knob.align(arc[0], ALIGN_CENTER); // วางทับ arc[0] ตรงกลาง
  arc_knob.knob.enable(true);         // ให้ ปุ่ม knob เห็น ทำงานขึ้นมา
  arc_knob.knob.color(TFT_WHITE);     // สีของ ปุ่ม knob
  arc_knob.knob.border(3, TFT_BLACK); // กำหนดขอบ และ สีขอบ ปุ่ม knob

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

