#include <BlynkGOv5.h>

GBar bar1;
GBar bar2;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar1.size(200, 20);
  bar1.align(ALIGN_CENTER, 0, -15);
  bar1 = 70;                          // ใส่ค่าให้ bar1

  bar2.size(bar1);                    // ขนาดกว้างxสูง เท่ากับ bar1
  bar2.color(TFT_PALETTE(TFT_PALETTE_RED));
  bar2.padding(6);                    // ระยะช่องไฟ
  bar2.border(2, TFT_PALETTE(TFT_PALETTE_RED));
  bar2.align(bar1, ALIGN_BOTTOM, 0, 30);
  bar2.range(0, 200);                 // กำหนดช่วง
  bar2.value(160, true);              // ใส่ค่าให้ bar2 พร้อมเล่น animation เวลาแสดงค่า

}

void loop()
{
  BlynkGO.update();
}


