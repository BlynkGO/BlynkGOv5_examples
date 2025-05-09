#include <BlynkGOv5.h>

IMAGE_DECLARE(img_skew_strip)

GBar bar;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar.image(img_skew_strip);
  bar.image_tiled(true);
  bar.image_opa(120);
  bar.size(260, 20);
  bar.mode(BAR_MODE_RANGE);
  bar.range(0, 100);
  bar.value_range(20, 90);

}

void loop()
{
  BlynkGO.update();
}

