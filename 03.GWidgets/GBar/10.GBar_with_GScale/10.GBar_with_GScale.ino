#include <BlynkGOv5.h>

GRect rect_bar;
  GBar bar(rect_bar);
  GScale scale(rect_bar);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect_bar.size(150,240);
  rect_bar.radius(10);

    bar.orientation(BAR_ORIENTATION_VERTICAL);  // แนวตั้ง
    bar.size(30, 200);
    bar.color(TFT_YELLOW);
    bar.bg_color(TFT_PALETTE(TFT_PALETTE_YELLOW)); 
    bar.padding(3);                             // ช่องไฟตัว bar กับ indicator
    bar.border(2, TFT_BLACK);
    bar.range(0,100);

    bar = 67;

    scale.size(100, bar.height());
    scale.range(bar);                             // range เดียวกับ bar
    scale.mode(SCALE_MODE_VERTICAL_RIGHT, 11, 2); // แบบแนวตั้งด้านขวา มีเส้นขีด ทั้งหมด 11 ขีด เส้นขีดหลัก เว้นทุกๆ 2 ขีด
    scale.tick_length(10,5);                      // ความยาว เส้นขีดหลัก 10px ความยาวเส้นขีดทั่วไป 5px
    scale.label_show(true);                       // แสดงข้อความที่เส้นขีดหลักด้วย
    scale.thickness(1, TFT_BLACK);

    scale.color(TFT_BLACK);
    scale.font_color(TFT_BLACK);
    scale.align(bar, ALIGN_RIGHT,20,0);
}

void loop(){
  BlynkGO.update();
}