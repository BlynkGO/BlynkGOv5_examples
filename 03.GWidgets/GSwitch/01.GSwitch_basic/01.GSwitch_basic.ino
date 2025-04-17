#include <BlynkGOv5.h>

GContainer cont;
  GSwitch sw1;
  GSwitch sw2;
  GSwitch sw3;
  GSwitch sw4;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.bg_opa(0);
  cont.layout(LAYOUT_COL_M, 5, 5, 5, 5, 10);  /* วิตเจ็ตลูกๆที่มาวางบน GContainer นี้จะเรียงตัวแนวตั้ง (แนว column) ไล่ลงมา แนวตรงกลางตรงกันอัตโนมัติ มีช่องไฟตามกำหนด */

    sw1.parent(cont);                 /* วางบน cont จะมีการเรียงตัวอัตโนมัติ ตาม layout ของ cont */
    sw1.OFF();                        /* กำหนดสถานะ sw ON() หรือ OFF() เริ่มต้น */

    sw2.parent(cont);
    sw2.ON();

    sw3.parent(cont);
    sw3.active(false);                /* กำหนดให้ active ไหม ถ้า active เป็น false ผู้ใช้จะกดเปลี่ยนแปลง สถานะ sw ไม่ได้ */

    sw4.parent(cont);
    sw4.ON();
    sw4.active(false);

    /* เมื่อ sw1 มีการเปลี่ยนแปลง จากผู้ใช้กดเปลี่ยน */
    sw1.onValueChanged(GWIDGET_CB{
      if(sw1.isON()){
        //...
      }else{
        //...
      }
    });

    sw2.onValueChanged(GWIDGET_CB{
      if(sw2.isON()){
        //...
      }else{
        //...
      }
    });

    sw3.onValueChanged(GWIDGET_CB{
      if(sw3.isON()){
        //...
      }else{
        //...
      }
    });

    sw4.onValueChanged(GWIDGET_CB{
      if(sw4.isON()){
        //...
      }else{
        //...
      }
    });


}

void loop(){
  BlynkGO.update();
}

