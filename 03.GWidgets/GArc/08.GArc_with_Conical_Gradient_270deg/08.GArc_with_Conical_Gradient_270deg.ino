#include <BlynkGOv5.h>

IMAGE_DECLARE(img_arc_src);  // รูปภาพของ arc ที่ไล่ gradient จาก แดง --> น้ำเงิน (หรือ อยากเปลี่ยนให้เตรียมภาพสร้างจาก GIMP)

GArc arc;
  GLabel lb_value(arc);

GSlider slider;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  arc.arc_image_src(&img_arc_src, GPART_INDICATOR);
  arc.bg_enable(true);
  arc.bg_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,3));
  arc.rounded(true);
  arc.radius(100);
  arc.thickness(18);
  arc.angle_offset(135);
  arc.angle_range(270);
  arc.range(0, 100);
  arc = 70;
    lb_value = "70%";
    lb_value.font(BebasNeueBold_num_45, TFT_WHITE);

  slider.width(arc.width());
  slider.range(arc.min_value(), arc.max_value());
  slider.align(arc, ALIGN_BOTTOM_MID, 0, 30);
  slider.onValueChanged(GWIDGET_CB{
    arc = slider.toInt();
    lb_value = String(slider.toInt()) + "%";
  });
}

void loop(){
  BlynkGO.update();
}
