/*********************************************************
 * 
 * วิตเจ็ต GKeyUnlocker 
 * เป็น วิตเจ็ตสำหรับ ล็อคหน้าจอ ด้วยการลากนิ้วผ่านจุด 4 จุดขึ้นไปจาก 9 จุด
 *
 *       *  *  *       1  2  3
 *       *  *  *  -->  4  5  6
 *       *  *  *       7  8  9
 *
 * ปกติให้ใช้เรียกขึ้นมาทำงานผ่านวิตเจ็ตอื่นๆ เพื่อ lock หน้าจอป้องกัน
 * ภายหลังเมื่อปลดล็อคสำเร็จจะถูกซ่อนลงไป
 * 
 * หมายเหตุ 
 *   - รหัส secretkey ที่กำหนด ตัวเลขแต่ละหลักห้ามซ้ำกัน
 * 
 *********************************************************/

#include <BlynkGOv5.h>

#define KEY       7415369       // รหัสเริ่มต้น ลากนิ้วเป็นรูปตัว M ที่จุด 9 จุด

GButton btn_setting;
GKeyUnlocker keyunlocker;

GRect screen_setting;
  GButton btn_screen_setting_exit(screen_setting);
  GLabel  lb_screen_setting(screen_setting);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  btn_setting = "หน้าตั้งค่า";
  btn_setting.onClicked(GWIDGET_CB{
    keyunlocker.secretkey(KEY);   // รหัสลับที่ user ต้องลากเส้นให้ตรงตามรหัสนี้  
    keyunlocker.auth_cb([](){
      screen_setting.show(true);
    });
  });

  screen_setting.show(false);
  screen_setting.color(TFT_BLACK);
    btn_screen_setting_exit = SYMBOL_ARROW_LEFT;
    btn_screen_setting_exit.round_design();
    btn_screen_setting_exit.align(ALIGN_TOP_LEFT, 10, 10);
    btn_screen_setting_exit.onClicked(GWIDGET_CB{
      screen_setting.show(false);
    });
    lb_screen_setting = "หน้าตั้งค่า";
}

void loop(){
  BlynkGO.update();
}
