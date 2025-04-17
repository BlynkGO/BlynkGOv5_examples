#include <BlynkGOv5.h>

GSwitch sw;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_COLOR_HEX(0xe0e0e0));

  /* ดีไซน์ ตัว sw สามารถ customizer แบบต่างๆ ได้ตามต้องการ */
  sw.radius(5);                                                                     /* รัศมีโค้ง วิตเจ็ต sw */
  sw.color(TFT_PALETTE(TFT_PALETTE_BLUE), GSTATE_ON);                               /* สีพื้นหลัง วิตเจ็ต sw เมื่อสถานะ ON  */
  sw.color(TFT_PALETTE(TFT_PALETTE_RED) , GSTATE_OFF);                              /* สีพื้นหลัง วิตเจ็ต sw เมื่อสถานะ OFF */

  sw.border(4, TFT_WHITE, (gstate_t)(DRAW_PART_KNOB | GSTATE_ON));                  /* ขอบกว้าง และ สีขอบ ของ ส่วน ปุ่ม knob  เมื่อสถานะ ON  */
  sw.border(4, TFT_WHITE, (gstate_t)(DRAW_PART_KNOB | GSTATE_OFF));                 /* ขอบกว้าง และ สีขอบ ของ ส่วน ปุ่ม knob  เมื่อสถานะ OFF */
  sw.color(TFT_PALETTE(TFT_PALETTE_BLUE), (gstate_t)(DRAW_PART_KNOB | GSTATE_ON));  /* สีพื้นหลัง ส่วน ปุ่ม knob เมื่อสถานะ ON  */
  sw.color(TFT_PALETTE(TFT_PALETTE_RED) , (gstate_t)(DRAW_PART_KNOB | GSTATE_OFF)); /* สีพื้นหลัง ส่วน ปุ่ม knob เมื่อสถานะ OFF */

  sw.shadow(16, 2, TFT_BLACK);
  sw.shadow_opa(180);

  /* เมื่อ sw มีการเปลี่ยนแปลง จากผู้ใช้กดเปลี่ยน */
  sw.onValueChanged(GWIDGET_CB{
    if(sw.isON()){
      // ...
    }else{
      // ...
    }
  });
  
}

void loop(){
  BlynkGO.update();
}

