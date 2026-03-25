#include <BlynkGOv5.h>

FONT_DECLARE(Pieces_of_Eight_45);
FONT_DECLARE(Pieces_of_Eight_60);

IMAGE_DECLARE(img_pirate_bg);
IMAGE_DECLARE(img_pirate_paper);
IMAGE_DECLARE(img_compass_dial);
IMAGE_DECLARE(img_compass_needle);

#define START_VALUE               320
#define TFT_COLOR_PIRATE_BROWN    TFT_COLOR_HEX(0xe5d6b5)
#define TFT_COLOR_PIRATE_BROWN2   TFT_COLOR_HEX(0xa37146)

GImage bg(img_pirate_bg);
  GLabel lb_title("$ Pirate of ASEAN $", bg);

  GImage compass_dial(img_compass_dial, bg);
    GScale scale(compass_dial);
    GImage compass_needle(img_compass_needle, compass_dial);

  GImage pirate_paper(img_pirate_paper, bg);
    GLabel lb_value(pirate_paper);

  GSlider slider_value(bg);

void compass_set_value(int32_t v){
  scale.image_needle_value(compass_needle, constrain(v, scale.range_min_value(), scale.range_max_value()));
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  lb_title.font(Pieces_of_Eight_45, TFT_COLOR_PIRATE_BROWN);
  lb_title.align(ALIGN_TOP, 0, 25);

  compass_dial.center(285,257);
    scale.size(300,300);
    scale.mode(SCALE_MODE_ROUND);
    scale.range(0,360);
    scale.angle_offset(-90);
    scale.angle_range(360);
    scale.label_show(false);
    scale.tick_opa(0);
    scale.thickness(0);

  compass_needle.align(compass_dial, ALIGN_CENTER, -5, 17);  // ปรับเข็มจนกว่าให้มาอยู่ตรงกลาง หน้าปัด
  compass_needle.pivot(161,22);                  // กำหนดจุดหมุน ให้ระยะ x และ y เข้าไป ถึงจุดหมุน ของ รูป เข็มหน้าปัด ให้เป็นแนวนอน ชี้ไปทางแกน X ทิศ +

  pirate_paper.position(480,270);
    lb_value.font(Pieces_of_Eight_60);
    lb_value.align(ALIGN_CENTER, 10, 5);

  slider_value.range(scale.range_min_value(), scale.range_max_value());
  slider_value.width(235);
  slider_value.align(pirate_paper, ALIGN_BOTTOM, 0, -25);
  slider_value.color(TFT_COLOR_PIRATE_BROWN2);
  slider_value.knob_color(TFT_COLOR_PIRATE_BROWN2);
  slider_value.bg_color(TFT_COLOR_PIRATE_BROWN2);
  slider_value.onValueChanged(GWIDGET_CB{
    compass_set_value(slider_value.toInt());
    lb_value = slider_value.toInt() + String(SYMBOL_DEGREE);
  });

  compass_set_value(START_VALUE);
  slider_value = START_VALUE;
  lb_value     = START_VALUE + String(SYMBOL_DEGREE);

}

void loop(){
  BlynkGO.update();
}
