
/***************************************************************************
 *   HowTo
 * -------------
 *
 * 1. ออกแบบกราฟิกด้วยโปรเแกรม squareline
 * และกำหนดโฟลเดอร์ ที่จะ Export UI Files ได้ที่
 * เมนู File > Project Settings > FILE EXPORT > UI Files Export Path
 *
 *    สำหรับ ArduinoIDE : ที๋โปรเแกรม squareline ให้กำหนด UI Files Export Path ไว้ที่ โฟลเดอร์ย่อย src/squareline ภายในโปรเจค ArduinoIDE นั้นๆ
 *    สำหรับ PlatformIO : ที๋โปรเแกรม squareline ให้กำหนด UI Files Export Path ไว้ที่ โฟลเดอร์ย่อย squareline     ภายในโปรเจค PlatformIO นั้นๆ
 *
 * 2. สร้างวิตเจ็ต BlynkGO โดยทำการ ลิงค์วิตเจ็ต BlynkGO เข้ากับ ui ที่ออกแบบจาก squareline
 *   (ui ต่างๆที่ออกแบบจาก squareline ที่จะเลือกทำการลิงค์วิตเจ็ต BlynkGO ดูได้ที่ squreline/ui.h)
 *
 * 3. หลัง ลิงค์วิตเจ็ต แล้ว
 * จะสามารถ เรียกใช้ คำสั่ง BlynkGO API ต่างๆ เพื่อปรับเปลี่ยนกราฟิกภายหลังได้อีกตามต้องการ
 * เช่น เปลี่ยนสี เปลี่ยนฟอนต์ เปลี่ยนขนาด และ maker สามารถกำหนดดักสัมผัสแบบต่างๆ ไปใช้งานได้ ฯลฯ
 *
 ***************************************************************************/

#include <BlynkGOv5.h>

#if defined(__has_include)
  #if __has_include("src/squareline/ui.h")
    #include "src/squareline/ui.h"        // ArduinoIDE
  #endif
  #if __has_include("squareline/ui.h")
    #include "squareline/ui.h"            // PlatformIO
  #endif
#endif

GButton btn;                        // วิตเจ็ตปุ่ม     ที่จะไป link กับ ui ที่ออกแบบจาก squareline
GButton btn_switch(BUTTON_SWITCH);  // วิตเจ็ตปุ่มสวิตซ์ ที่จะไป link กับ ui ที่ออกแบบจาก squareline

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  //----------------------------------------------
  // ออกแบบจาก squareline
  // แล้วทำการ ลิงค์ เชื่อม widget กับ ui จาก squareline เฉพาะที่ต้องการ
  // (ดู รายการ ui ต่างๆ ที่จะสามารถใช้ลิงค์ได้ ที่ไฟล์ squreline/ui.h)

  BlynkGO   .link(ui_Screen1);      // ลิงค์ BlynkGO ไปที่ Screen หลัก ที่ออกแบบจาก squareline *** ต้องมี ***
  btn       .link(ui_Button1);      // ลิงค์วิตเจ็ตปุ่ม กับ ui ที่ออกแบบจาก squareline
  btn_switch.link(ui_Button2);      // ลิงค์วิตเจ็ตปุ่ม กับ ui ที่ออกแบบจาก squareline

  //----------------------------------------------
  // ปรับเปลี่ยนกราฟิกเพิ่มเติมจากที่ออกแบบจาก squareline และกำหนดดักสัมผัสใช้งานต่างๆ ภายหลังได้

  btn = "ปุ่มกด";
  btn.onClicked(GWIDGET_CB {
    Serial.println("[btn] Clicked");
  });

  btn_switch = "ปุ่มสวิตซ์";
  btn_switch.ON();
  btn_switch.onValueChanged(GWIDGET_CB {
    Serial.printf("[btn_switch] %s\n", btn_switch.isON() ? "ON" : "OFF");
  });

}

void loop()
{
  BlynkGO.update();
}

