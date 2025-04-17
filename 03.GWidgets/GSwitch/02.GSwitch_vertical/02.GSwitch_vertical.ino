#include <BlynkGOv5.h>

GSwitch sw;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  sw.size(30,60);                                 /* เปลี่ยนเป็นแนวตั้ง อัตโนมัติ หาก กว้าง น้อยกว่า สูง */
  // sw.orientation(SWITCH_ORIENTATION_VERTICAL); /* สามารถบังคับทิศให้เป็นแนวตั้งเองได้ (เช่น ที่ออกแบบไว้ หาก กว้าง มากกว่า สูง ) */
  sw.color(TFT_GREEN);                            /* เปลี่ยนสีพื้นหลังขณะ ON */
  sw.color(TFT_RED, GSTATE_OFF);                  /* เปลี่ยนสีพื้นหลังขณะ OFF */
  // sw.knob_color(TFT_VIOLET);                   /* เปลี่ยนสีปุ่มเลื่อน */
  // sw.radius(5);                                /* เปลี่ยนรัศมี */

  /* เมื่อ sw มีการเปลี่ยนแปลง จากผู้ใช้กดเปลี่ยน */
  sw.onValueChanged(GWIDGET_CB{
    if(sw.isON()){
      Serial.println("[GSwitch] ON");
    }else{
      Serial.println("[GSwitch] OFF");
    }
  });
}

void loop(){
  BlynkGO.update();
}
