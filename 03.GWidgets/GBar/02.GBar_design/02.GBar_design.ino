#include <BlynkGOv5.h>

GBar bar;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar.size(200, 20);
  bar.border(2, TFT_PALETTE(TFT_PALETTE_BLUE), GPART_MAIN);
  bar.padding(6,  GPART_MAIN);
  bar.radius(6, GPART_MAIN);
  bar.anim_duration(1000,  GPART_MAIN);
  bar.color(TFT_PALETTE(TFT_PALETTE_BLUE), GPART_INDICATOR);
  bar.radius(6, GPART_INDICATOR);
  bar.range(0,100);
  bar.value(100, true);
}

void loop()
{
  BlynkGO.update();
}

