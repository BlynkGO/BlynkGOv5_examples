#include <BlynkGOv5.h>

IMAGE_DECLARE(img_bar_mask);

GMask  bar_mask(img_bar_mask);
  GBar bar(bar_mask);

void setup()
{
  Serial.begin(115200);
  BlynkGO.begin();

  bar = 80;

}

void loop()
{
  BlynkGO.update();
}



