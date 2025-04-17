
/*
 * ตัวอย่างการเพิ่ม title เข้าไป โดยใช้ วิตเจ็ต GLabel เพิ่ม
 * (เขียนง่าย ใช้เม็มในการสร้างวิตเจ็ต GLabel เพิ่ม)
 */

#include <BlynkGOv5.h>

GGaugeRainbow gauge;
  GLabel lb_title(gauge);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  gauge.size(200,200);
  gauge.border(1,TFT_SILVER);
  gauge.radius(20);
  gauge.padding(45,20,20,20);
  gauge = 89.34;
    lb_title = "อุณหภูมิ";
    lb_title.font(prasanmit_25, TFT_SILVER);
    lb_title.align(ALIGN_TOP,0,-40);

}

void loop()
{
  BlynkGO.update();
}
