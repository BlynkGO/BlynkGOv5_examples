#include <BlynkGOv5.h>

GBar bar;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar.size(20, 200);
  bar.color(TFT_PALETTE(TFT_PALETTE_RED), TFT_PALETTE(TFT_PALETTE_BLUE));
  bar.grad_dir(GRAD_DIR_VER);
  bar.range(-20, 40);
  bar  = 30;

  static GAnimation anim;
  anim.init(bar, [](GWIDGET, int32_t value) {
    bar.value(value, true);
  }, -20, 40, 3000, true, true);
  anim.create();
}

void loop()
{
  BlynkGO.update();
}


