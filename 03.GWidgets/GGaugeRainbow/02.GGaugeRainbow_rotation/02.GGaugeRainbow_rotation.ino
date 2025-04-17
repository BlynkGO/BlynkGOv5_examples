
#include <BlynkGOv5.h>

GContainer cont(LAYOUT_ROW_M,5,5,5,5,30);
  GGaugeRainbow gauge(cont);
  GGaugeRainbow gauge_rotation(cont);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.bg_opa(0);

  gauge = 89.43;
  gauge.angle_offset(-90);        /* มุมเริ่มต้น นับจากแกน x */
  gauge.angle_total(270);         /* มุมของ gauge ทั้งหมด */

  gauge_rotation = 94.23;
  gauge_rotation.rotation(90);    /* หมุน gauge 90 องศา */

  /* เปลี่ยนขนาด สำหรับจอเล็ก */
  if(BlynkGO.width() == 240 || BlynkGO.height()==240) {
    gauge.size(120, 120);
    gauge_rotation.size(120, 120);
  }

}

void loop()
{
  BlynkGO.update();
}
