#include <BlynkGOv5.h>

FONT_DECLARE(prasanmit_15);
FONT_DECLARE(prasanmit_18);

IMAGE_DECLARE(img_gauge_100bar);
IMAGE_DECLARE(img_gauge_dial);
IMAGE_DECLARE(img_gauge_needle);

GImage gauge_dial(img_gauge_dial);
  GScale scale(gauge_dial);         // scale หน้าปัด หลัก
  GScale scale_section(gauge_dial); // scale ส่วนที่เป็น แถบสีฟ้า ขาว แดง
    uint32_t _range[4] = { 0, 33, 66, 100};
    GScaleSection section[3];
    color_t sect_color[3] = { TFT_COLOR_HEX(0x1a8ad7), TFT_WHITE, TFT_COLOR_HEX(0xe0191a)};
  GScale scale_helper(gauge_dial);  // scale ส่วนเส้นเสริม ภายใน ของแถบขาว

  GLabel lb_pressure("PRESSURE", gauge_dial);
  GLabel lb_low("LOW", gauge_dial);
  GLabel lb_high("HIGH", gauge_dial);
  GLabel lb_unit_bar("UNIT: BAR", gauge_dial);
    
  GImage gauge_needle(img_gauge_needle, gauge_dial);

void gauge_set_value(int32_t v){
  scale.image_needle_value(gauge_needle, constrain(v, scale.range_min_value(), scale.range_max_value()));
}

#define START_VALUE     60

GSlider slider_value;
GLabel  lb_value;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  scale.size(180,180);
  scale.range(0,100);
  scale.angle_offset(135);
  scale.angle_range(270);
  scale.mode(SCALE_MODE_ROUND_OUTER, 10 * 5 + 1, 5);
  scale.font(prasanmit_25, TFT_BLACK);
  scale.tick_length(8, 5);
  scale.pad_radial(-4);

  scale_section.size(178,178);
  scale_section.range(0,100);
  scale_section.angle_offset(135);
  scale_section.angle_range(270);
  scale_section.mode(SCALE_MODE_ROUND_OUTER, 2, 1);
  scale_section.tick_opa(0);  scale_section.label_show(false);  // ทำ section อย่างเดียว
    for(int i=0; i<3; i++) {
      section[i] = scale_section.add_section();
      section[i].thickness(28, sect_color[i]);
      section[i].range(_range[i],_range[i+1]);
    }
  
  scale_helper.size(180,180);
  scale_helper.range(33,66);
  scale_helper.angle_offset(135+270.0/100*33);
  scale_helper.angle_range(270.0/100*(66-33));
  scale_helper.mode(SCALE_MODE_ROUND_INNER, 18, 1);
  scale_helper.axis_opa(0);
  scale_helper.thickness(1);
  scale_helper.tick_opa(200);
  scale_helper.tick_length(28);  scale_helper.label_show(false);
    
  lb_pressure.font(prasanmit_20, TFT_BLACK);  lb_pressure.align(ALIGN_CENTER,0,-25);
  lb_low.font(prasanmit_15, TFT_BLACK);       lb_low.align(ALIGN_TOP_LEFT,100,160);
  lb_high.font(prasanmit_15, TFT_BLACK);      lb_high.align(ALIGN_TOP_RIGHT,-100,160);
  lb_unit_bar.font(prasanmit_18, TFT_BLACK);  lb_unit_bar.align(ALIGN_BOTTOM,0,-70);

  gauge_needle.align(ALIGN_CENTER, 31, 0);    // วางให้ รูปเข็มหน้าปัด มาอยู่กลาง GScale ที่เป็นแม่
  gauge_needle.pivot(35,14);                  // กำหนดจุดหมุน ให้ระยะ x และ y เข้าไป ถึงจุดหมุน ของ รูป เข็มหน้าปัด ให้เป็นแนวนอน ชี้ไปทางแกน X ทิศ +


  slider_value.range(scale.range_min_value(), scale.range_max_value());
  slider_value.align(ALIGN_BOTTOM,0,-30);
  slider_value.onValueChanged(GWIDGET_CB{
    gauge_set_value(slider_value.toInt());
    lb_value = slider_value.toInt();
  });
  lb_value.align(slider_value, ALIGN_TOP,0,-5);  
  lb_value.color(TFT_WHITE);

  gauge_set_value(START_VALUE);
  slider_value = START_VALUE;
  lb_value     = START_VALUE;

}

void loop(){
  BlynkGO.update();
}
