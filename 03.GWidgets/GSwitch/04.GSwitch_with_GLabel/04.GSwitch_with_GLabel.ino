/* 
  ตัวอย่าง เพิ่ม ข้อความ ON/OFF ให้ GSwitch ด้วยการใช้วิตเจ็ต GLabel เป็น ลูกของ GSwitch
  เป็นการดีไซน์ด้วยกราฟิกระดับบนๆ ผ่านวิตเจ็ต ซึ่งจะใช้ง่ายกว่า 
  แต่จะต้องใช้เม็มโมรี่ในการประกาศวิตเจ็ต
 */

#include <BlynkGOv5.h>

GSwitch sw;
  GLabel sw_label(sw);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  sw.size(65, 30);
  sw.color(TFT_GREEN, GSTATE_ON);
  sw.color(TFT_RED, GSTATE_OFF);

  sw.onDrawMainEnd(GWIDGET_CB{                 // เมื่อวิตเจ็ตมีการวาดส่วนหลักเสร็จสิ้น
    static SoftTimer timer;
    timer.delay(0,[](){
      if(sw.isON()){
        sw_label = "ON";
        sw_label.font(prasanmit_20, TFT_WHITE);
        sw_label.align(ALIGN_LEFT,10);
      }else{
        sw_label = "OFF";
        sw_label.font(prasanmit_20, TFT_WHITE);
        sw_label.align(ALIGN_RIGHT,-8);
      }
    });
  });
}

void loop()
{
  BlynkGO.update();
}
