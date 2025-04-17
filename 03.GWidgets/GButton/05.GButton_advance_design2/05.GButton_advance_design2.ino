#include <BlynkGOv5.h>

GButton btn;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  static GStyle  style_default;
    static GTransition trans_default;
    static style_prop_t props[] = {GSTYLE_TRANSFORM_WIDTH, GSTYLE_TRANSFORM_HEIGHT, GSTYLE_TEXT_LETTER_SPACE, 0};
  trans_default.init(props,  ANIM_PATH_OVERSHOOT, 250, 100);
  style_default.transition(trans_default);

  static GStyle  style_pressed;
    static GTransition trans_pressed;
    trans_pressed.init(props, ANIM_PATH_EASE_IN_OUT, 250, 100);

  style_pressed.transform_width(10);
  style_pressed.transform_height(-10);
  style_pressed.text_letter_space(10);
  style_pressed.transition(trans_pressed);

  btn = "ปุ่มเด้งดึ๋ง";
  btn.addStyle(style_default, GSTATE_DEFAULT);
  btn.addStyle(style_pressed, GSTATE_PRESSED);

}

void loop()
{
  BlynkGO.update();
}

