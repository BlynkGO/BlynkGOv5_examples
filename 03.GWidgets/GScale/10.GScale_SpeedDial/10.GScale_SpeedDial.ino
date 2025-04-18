#include <BlynkGOv5.h>

FONT_DECLARE(BebasNeueBold_num_45);

GCircle cir_frame;
GScale scale;
  uint32_t _range[] = {0,80,90};
  color_t sect_color[] = { TFT_WHITE, TFT_RED };

GScale scale_inner;
GLabel lb_speed;
GLabel lb_unit;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cir_frame.size(355,355);
  cir_frame.bg_opa(0);
  cir_frame.shadow(15, 5, TFT_WHITE);
  cir_frame.shadow_opa(120);

  scale.size(350,350);
  scale.range(0,90);
  scale.angle_offset(90);
  scale.angle_range(36*9);
  scale.mode(SCALE_MODE_ROUND);
  scale.tick_opa(0);
  for(int i=0; i<2; i++) {
    GScaleSection *section = scale.addSection();
    section->thickness(20, sect_color[i]);
    section->range(_range[i], _range[i+1]);
  }
  scale.thickness(0);

  scale_inner.size(300,300);
  scale_inner.angle_offset(90);
  scale_inner.angle_range(36*8);
  scale_inner.mode(SCALE_MODE_ROUND, 10*8+1, 10);
  scale_inner.label_show(true);                       // แสดงข้อความที่เส้นขีดหลักด้วย
  static const char * custom_labels[] =  {"0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8", NULL};
  scale_inner.text_src(custom_labels);                // ข้อความ ที่ เส้นขีดหลัก

  scale_inner.tick_length(10,5);  // ความยาว ขีดหลัก, ขีดรอง
  scale_inner.thickness(0,3,1);   // ความหนา เส้นแกน, ขีดหลัก, ขีดรอง
  scale_inner.color(TFT_WHITE);
  scale_inner.font(prasanmit_40, TFT_WHITE);

  lb_speed = 255;
  lb_speed.font(BebasNeueBold_num_45);

  lb_unit = "km/h";
  lb_unit.font(prasanmit_20, TFT_SILVER);
  lb_unit.align(lb_speed, ALIGN_BOTTOM,0,-5);
}

void loop(){
  BlynkGO.update();
}
