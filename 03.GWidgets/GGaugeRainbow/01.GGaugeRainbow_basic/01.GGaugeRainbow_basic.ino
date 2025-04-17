
#include <BlynkGOv5.h>

GGaugeRainbow gauge;
GGaugeRainbow gauge_full;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  /* เปลี่ยนขนาด สำหรับจอเล็ก */
  if(BlynkGO.width() == 240 || BlynkGO.height()==240) {
    gauge.size(120, 120);
    gauge_full.size(120, 120);
  }

  gauge = 89.43;
  gauge.align(ALIGN_CENTER, -gauge.width()/2 - 20);

  gauge_full = 89.43;
  gauge_full.gauge_type(GAUGE_MODE_FULL);
  gauge_full.align(ALIGN_CENTER, gauge_full.width()/2 + 20);
}

void loop()
{
  BlynkGO.update();
}