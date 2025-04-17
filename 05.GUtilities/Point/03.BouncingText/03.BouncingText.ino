#include <BlynkGOv5.h>

IMG_DECLARE(img_background_800x480);

GImage bg;
GLabel label;

GWidget *wg;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  bg    = img_background_800x480;
  label = "Hello สวัสดีครับ";
  wg    = &label;
  static SoftTimer timer;
  timer.setInterval(10,[](){
    static float x = BlynkGO.width()/2, y = BlynkGO.height()/2;   // ตำแหน่งเริ่มต้น
    static float dx = 2.5, dy = 3.0;                              // ความเร็วในแกน X และ Y
    wg->center(Point(x += dx, y +=dy));                           // กำหนดตำแหน่ง
    if (x - wg->width()/2  <= 0 || x + wg->width()/2  >= BlynkGO.width() )  dx = -dx;    // เปลี่ยนทิศทางแกน X
    if (y - wg->height()/2 <= 0 || y + wg->height()/2 >= BlynkGO.height())  dy = -dy;    // เปลี่ยนทิศทางแกน Y
  });
}

void loop(){
  BlynkGO.update();
}
