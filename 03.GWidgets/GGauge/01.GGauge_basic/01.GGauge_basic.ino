/******************************
 * วิตเจ็ต GGauge จะคล้าย GArc 
 *   แต่มีตัวเลขแสดงตรงกลาง ให้
 ******************************/

#include <BlynkGOv5.h>

GGauge gauge;
GGauge gauge_full;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  //-----------------------------------
  // คำสั่ง ขนาด และตำแหน่ง
  //-------------------------
  gauge.size(100,100);
  gauge.align(ALIGN_CENTER,-15- gauge.width()/2);

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  // gauge.bg_color(TFT_SILVER);      // สี ของ arc พื้นหลัง
  // gauge.bg_opa(180);               // ความโปร่งใส ของ arc พื้นหลัง

  // gauge.color(TFT_GREEN);          // สี ของ arc
  // gauge.thickness(20);             // ปรับความหนาของ arc
  // gauge.thickness(20, TFT_GREEN);  // กำหนดความหนา และสี ไปพร้อมๆกัน
  // gauge.opa(200);                  // ความโปร่งใส ของ arc
  // gauge.rounded(true);             // ให้ปลาย arc โค้งมน
  // gague.font( prasanmit_40, TFT_RED); // เปลี่ยน ฟอนต์และสีฟอนต์

  //-----------------------------------
  // คำสั่งดักเหตุการณ์ : เมื่อมีการเปลี่ยนแปลงค่า
  //-------------------------
  gauge.onValueChanged(GWIDGET_CB{
    Serial.printf("[GGauge] value : %.2f\n", gauge.toFloat());
  });
  gauge = 80.76;

  //==================================================================

  gauge_full.mode(GAUGE_MODE_FULL);
  gauge_full.size(gauge);                   // ขนาดเท่ากับ gauge ก่อนหน้า
  gauge_full.color(TFT_PALETTE(TFT_PALETTE_ORANGE));
  gauge_full.align(gauge, ALIGN_RIGHT,30);

  gauge_full.decimal(1);                    // กำหนดทศนิยมกี่ตำแหน่งที่จะแสดงที่หน้าเกจจ์ : ค่าเริ่มต้น 2 ตำแหน่ง
  gauge_full = 89.3234;

}

void loop(){
  BlynkGO.update();
}