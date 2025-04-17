#include <BlynkGOv5.h>

GRect rect;
  GCircle circle(rect);

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect.size(200,150);
  rect.color(TFT_WHITE);
  rect.onPressing(GWIDGET_CB{
    circle.center( rect.touch_point() );
  });

    circle.radius(20);
    circle.color(TFT_RED);
    circle.shadow(20, 5, TFT_RED);  // shadow width, shadow spread, shadow color

  label = "สัมผัส เพื่อเปลี่ยนจุดตามสัมผัส";
  label.font(prasanmit_25);
  label.align(rect,ALIGN_BOTTOM);
  label.underline(true);
}

void loop(){
  BlynkGO.update();
}
