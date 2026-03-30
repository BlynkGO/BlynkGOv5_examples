#include <BlynkGOv5.h>

IMAGE_DECLARE(img_arc_grad_conical360); // เตรียมภาพ gradient แบบ conical 360 องศา ไว้ใช้กับ arc ให้ใช้ GIMP สร้างภาพแบบ conical gradient ขนาด 360x1 pixel แล้ว export เป็น .c ด้วย plugin GIMP C source code exporter แล้วเอาไฟล์ .c มาใส่ในโปรเจคนี้ แล้วประกาศด้วย IMAGE_DECLARE() ตามตัวอย่างนี้

GArc arc;
GLabel lb_value;
GSlider slider;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  arc.arc_image_src(&img_arc_grad_conical360, GPART_INDICATOR);
  arc.bg_enable(true);
  arc.bg_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,3));
  arc.radius(100);
  arc.thickness(15);
  arc.rounded(false);
  arc.angle_offset(-90);
  arc.angle_range(360);
  arc.range(0, 1001);   // ใส่พอดีไม่ได้ ให้ ขยาย 10 เท่า แล้วเพิ่มอีก 1 หน่วย เพื่อให้ได้ 100% พอดี

  lb_value.font(BebasNeueBold_num_45, TFT_WHITE);

  slider.width(arc.width());
  slider.range(0, 100);
  slider.align(arc, ALIGN_BOTTOM_MID, 0, 30);
  slider.onValueChanged(GWIDGET_CB{
    arc = slider.toInt()*10;    // ขยาย 10 เท่า
    lb_value = String(slider.toInt()) + "%";
  });
  slider = 70;
  slider.event_send(EVENT_VALUE_CHANGED);  // ส่ง event ให้แสดงผลตอนเริ่มต้นด้วย
}

void loop(){
  BlynkGO.update();
}
