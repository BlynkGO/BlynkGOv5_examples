#include <BlynkGOv5.h>

GRect rect;
  GArc arc(rect);
    GLabel lb_value(arc);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE_LIGHTEN(TFT_PALETTE_GRAY, 3));
 
  rect.size(SIZE_CONTENT, SIZE_CONTENT);
  rect.padding(20,5,20,20);
  rect.color(TFT_COLOR_HEX(0x1c1c1c));
  rect.radius(10);
    arc.size(120, 120);
    arc.rounded(true);

    arc.arc_color(TFT_BLACK, GPART_MAIN); 
    arc.arc_opa(255, GPART_MAIN);
    arc.arc_width(4, GPART_MAIN);

    arc.arc_color(TFT_PALETTE_DARKEN(TFT_PALETTE_TEAL, 2), GPART_INDICATOR); 
    arc.arc_opa(255, GPART_INDICATOR);
    arc.arc_width(8, GPART_INDICATOR);
    arc.padding(-2, GPART_INDICATOR);

    arc.angle_offset(135);
    arc.angle_range(270);
    arc.range(0,100);              // range ของค่าที่จะใช้กับ arc (หากไม่ใส่ จะเป็นช่วง 0-100 อัตโนมัติ)
    arc = 80;                      // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด

    lb_value = 80;
    lb_value.font(prasanmit_40, TFT_WHITE);
}

void loop(){
  BlynkGO.update();
}
