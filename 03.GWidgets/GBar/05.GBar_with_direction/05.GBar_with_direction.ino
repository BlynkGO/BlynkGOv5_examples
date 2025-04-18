#include <BlynkGOv5.h>

GBar bar_left_to_right;
GBar bar_right_to_left;

GLabel label_l2r;
GLabel label_r2l;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar_left_to_right.size(200, 20);
  bar_left_to_right = 70;
  bar_left_to_right.align_center(0, -30);

  label_l2r = "Left to Right direction";
  label_l2r.align(bar_left_to_right, ALIGN_TOP, 0, -5);

  bar_right_to_left.size(bar_left_to_right);
  bar_right_to_left.direction(BASE_DIR_RTL);            // ให้ทิศทาง GBar เป็นแบบ จาก ขวาไปซ้าย
  bar_right_to_left.align_center(0, 30);
  bar_right_to_left = 70;

  label_r2l = "Right to Left direction";
  label_r2l.align(bar_right_to_left, ALIGN_TOP, 0, -5);
}

void loop()
{
  BlynkGO.update();
}

