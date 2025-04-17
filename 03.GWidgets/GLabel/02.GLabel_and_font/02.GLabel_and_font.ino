/***************************************************************
 * 
 * ฟอนต์ 
 *   1. ฟอนต์พร้อมใช้ 
 *        prasanmit_20, prasanmit_25, prasanmit_30, prasanmit_35, prasanmit_40
 * 
 *   2. ฟอนต์ อื่นๆ หากต้องการเพิ่มเข้าไปในโปรเจค โหลดได้ที่ BlynkGO Font
 *      https://github.com/BlynkGO/BlynkGO_font
 *   
 *      หมายเหตุ ก่อนใช้ font ให้ประกาศ FONT_DECLARE(...); เสียก่อน
 * 
 ***************************************************************/

#include <BlynkGOv5.h>

FONT_DECLARE(kanit_40);

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();

  BlynkGO.begin();
  // BlynkGO.default_font(prasanmit_20);  // หากต้องการเปลี่ยน default font ของทั้งระบบ ให้วางตรงนี้หลัง BlynkGO.begin();

  label = "ขอต้อนรับสู่ BlynkGOv5";

  label.font(kanit_40);
  label.color(TFT_CYAN);

}

void loop(){
  BlynkGO.update();
}

