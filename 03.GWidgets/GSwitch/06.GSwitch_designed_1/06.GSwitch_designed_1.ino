#include <BlynkGOv5.h>

GSwitch sw;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  /* ดีไซน์ ตัว sw สามารถ customizer แบบต่างๆ ได้ตามต้องการ */
  sw.color(TFT_WHITE);
  sw.border(1, TFT_RED  , (gstate_t) (DRAW_PART_MAIN      | GSTATE_OFF)); /* ขอบกว้าง และ สีขอบ ของ ส่วนพื้นหลัง     เมื่อสถานะ OFF */
  sw.border(1, TFT_RED  , (gstate_t) (DRAW_PART_INDICATOR | GSTATE_OFF)); /* ขอบกว้าง และ สีขอบ ของ ส่วน indicator เมื่อสถานะ OFF */
  sw.border(4, TFT_RED  , (gstate_t) (DRAW_PART_KNOB      | GSTATE_OFF)); /* ขอบกว้าง และ สีขอบ ของ ส่วน ปุ่ม knob  เมื่อสถานะ OFF */
  sw.border(1, TFT_GREEN, (gstate_t) (DRAW_PART_MAIN      | GSTATE_ON) );
  sw.border(1, TFT_GREEN, (gstate_t) (DRAW_PART_INDICATOR | GSTATE_ON) );
  sw.border(4, TFT_GREEN, (gstate_t) (DRAW_PART_KNOB      | GSTATE_ON) );
  sw.padding(-6, GPART_KNOB);

  /* เมื่อ sw มีการเปลี่ยนแปลง จากผู้ใช้กดเปลี่ยน */
  sw.onValueChanged(GWIDGET_CB{
    if(sw.isON()){
      // ...
    }else{
      // ...
    }
  });

}

void loop()
{
  BlynkGO.update();
}
