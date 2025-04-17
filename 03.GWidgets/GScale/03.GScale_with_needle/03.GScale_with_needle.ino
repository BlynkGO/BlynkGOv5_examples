#include <BlynkGOv5.h>

IMAGE_DECLARE(img_hand);

GScale scale_line;
  GLine line_needle(scale_line);

GScale scale_image;
  GImage image_needle(img_hand, scale_image);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  scale_line.size(150,150);
  scale_line.bg_color(TFT_SILVER);
  scale_line.radius(RADIUS_CIRCLE);
  scale_line.corner_mask(true);
  scale_line.align(ALIGN_LEFT, PCT(2));

  scale_line.mode(SCALE_MODE_ROUND_INNER,31,5);  // scale แบบ โค้ง ภายใน มีเส้นขีด ทั้งหมด 31 ขีด เส้นขีดหลัก เว้นทุกๆ 5 ขีด
  scale_line.label_show(true);
  scale_line.range(10,40);
  
  scale_line.tick_length(10,5);
  scale_line.angle_range(270);
  scale_line.angle_offset(135);

    line_needle.thickness(6, TFT_BLUE);
    line_needle.rounded(true);

    static GAnimation anim_scale_line;
    anim_scale_line.init(scale_line,[](GWIDGET, int32_t v){
      scale_line.line_needle_value(line_needle, 60, v);
    }, 10,40, 1000, true, true );
    anim_scale_line.create();

  scale_image.size(scale_line);
  scale_image.bg_color(scale_line.bg_color() );
  scale_image.radius(RADIUS_CIRCLE);
  scale_image.corner_mask(true);
  scale_image.align(ALIGN_RIGHT, PCT(-2));

  scale_image.mode(SCALE_MODE_ROUND_INNER,31,5);  // scale แบบ โค้ง ภายใน มีเส้นขีด ทั้งหมด 31 ขีด เส้นขีดหลัก เว้นทุกๆ 5 ขีด
  scale_image.label_show(true);
  scale_image.range(10,40);
  
  scale_image.tick_length(10,5);
  scale_image.angle_range(270);
  scale_image.angle_offset(135);
    image_needle.align(ALIGN_CENTER,47,-2);
    image_needle.pivot(3,4);
    image_needle.color(TFT_RED);

    static GAnimation anim_scale_image;
    anim_scale_image.init(scale_image,[](GWIDGET, int32_t v){
      scale_image.image_needle_value(image_needle, v);
    }, 10,40, 1000, true, true );
    anim_scale_image.create();

}

void loop(){
  BlynkGO.update();
}

