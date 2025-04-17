#include <BlynkGOv5.h>

IMAGE_DECLARE(img_fan_part01);
IMAGE_DECLARE(img_fan_part02);
IMAGE_DECLARE(img_fan_part03);

GImage fan(img_fan_part01);
  GImage fan_blade(img_fan_part02, fan);
  GImage fan_guard(img_fan_part03, fan);

GContainer fan_switch(LAYOUT_ROW_M, 6, 6, 6, 6, 3);
  GButton btn[4];

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  fan.align(ALIGN_CENTER);
    fan_blade.center(93, 90);
    fan_guard.center(93, 90);

  fan_switch.color(TFT_COLOR_HEX(0xf6b666));
  fan_switch.radius(30);
  for(int i = 0; i < 4; i++) {
    btn[i] = (i == 0) ? String("OFF") : String(i);
    btn[i].size(60, 40);
    btn[i].mode(BUTTON_SWITCH);
    btn[i].font(prasanmit_25);
    btn[i].radius(RADIUS_CIRCLE);
    btn[i].parent(fan_switch);
    btn[i].onClicked(GWIDGET_CB {
      static GAnimation anim_fan;
      int ii = (GButton *)widget - &btn[0];
      for(int i = 0; i < 4; i++) {
        if(i != ii || i == 0) {
          btn[i].OFF();
        }
      }
      switch(ii) {
        case 0:
          anim_fan.del();
          break;
        case 1:
          anim_fan.init(fan_blade, ANIM_ANGLE, 360, 0, 1000, true);
          anim_fan.start();
          break;
        case 2:
          anim_fan.init(fan_blade, ANIM_ANGLE, 360, 0, 600, true);
          anim_fan.start();
          break;
        case 3:
          anim_fan.init(fan_blade, ANIM_ANGLE, 360, 0, 300, true);
          anim_fan.start();
          break;
      }
    });
    fan_switch.align(fan, ALIGN_BOTTOM, 0, 20);
  }
}

void loop()
{
  BlynkGO.update();
}

