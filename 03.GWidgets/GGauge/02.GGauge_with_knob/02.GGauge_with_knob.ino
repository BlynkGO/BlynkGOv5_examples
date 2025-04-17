#include <BlynkGOv5.h>

GGauge gauge;
GGauge gauge2;

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
  gauge.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_GREEN,2));     // สี ของ arc
  gauge.font(prasanmit_35);
  gauge.knob.enable(true);          // ให้มีปุ่ม ให้การปรับค่าหรือไม่ ถ้ามี knob หากมีการดัก onValueChanged(...) เมื่อมีการเปลี่ยนแปลงค่า ก็จะสามารถนำไปทำอะไรๆได้
  // gauge.knob.clickable(false);   // หากไม่ต้องการให้ผู้ใช้ ปรับค่า ทาง knob ได้ ให้ปิดสัมผัส
  // gauge.knob.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_CYAN,2));  // สีของปุ่ม knob
  // gauge.knob.opa(0);             // ให้ knob มองไม่เห็น แต่ปรับค่าด้วยสัมผัสได้อยู่เมื่อ knob_enable(true);
  // gauge.knob.padding(5);         // ปรับขยายขนาด ปุ่ม knob เพิ่มจาก ระยะ thick ของ arc
  gauge.knob.border(3, TFT_WHITE);  // ขอบ และสีขอบของ ปุ่ม knob

  //-----------------------------------
  // คำสั่ง เฉพาะตัว สำหรับ วิตเจ็ต GArc
  //-------------------------
  // gauge.range(0,100);
  gauge.decimal(0);                  // กำหนดทศนิยมกี่ตำแหน่งที่จะแสดงที่หน้าเกจจ์ : ค่าเริ่มต้น 2 ตำแหน่ง
  gauge = 70;

  //-----------------------------------
  // คำสั่งดักเหตุการณ์ : เมื่อมีการเปลี่ยนแปลงค่า
  //-------------------------
  gauge.onValueChanged(GWIDGET_CB{
    Serial.println( gauge.toInt());
  });

  //==================================================================

  gauge2.size(gauge);                 // ขนาดเท่ากับ gauge ก่อนหน้า
  gauge2.align(gauge, ALIGN_RIGHT,30);
  gauge2.decimal(1);
  gauge2.knob.enable(true); 
  gauge2.knob.border(3, TFT_WHITE);
  gauge2.angle_offset(180);
  gauge2.bg_angle(0,270);

}

void loop(){
  BlynkGO.update();
}

