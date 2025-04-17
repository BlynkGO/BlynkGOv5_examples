#include <BlynkGOv5.h>

GCircle cir;
GArc arc[4];
color_t arc_color[4] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW};

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  cir.size(120, 120);
  cir.border(2, TFT_SILVER);
  cir.opa(100);
  for(int i = 0; i < 4; i++) {
    arc[i].size(cir.width() + 35, cir.height() + 35);
    arc[i].angle_offset(90 * i + 15);   // มุมเริ่มต้น
    arc[i].bg_enable(true);             // ให้แสดง arc พื้นหลังด้วย
    arc[i].bg_angle(0, 60);             // arc พื้นหลัง 0-60 องศา นับจาก มุมเริ่มต้น
    arc[i].angle(0, random(20, 60));    // arc หลัก   0-มุมสุ่ม    นับจาก มุมเริ่มต้น
    arc[i].rounded(true);               // ปลายมน
    arc[i].color(arc_color[i]);
  }
}

void loop()
{
  BlynkGO.update();
}

