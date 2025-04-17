#include <BlynkGOv5.h>

#define PASSCODE       135790       // รหัส passcode

GButton btn_setting;
GPasscodeLock passcodelock;

GRect screen_setting;
  GButton btn_screen_setting_exit(screen_setting);
  GLabel  lb_screen_setting(screen_setting);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  btn_setting = "หน้าตั้งค่า";
  btn_setting.onClicked(GWIDGET_CB{
    passcodelock.passcode(PASSCODE);   // รหัสลับที่ user ต้องลากเส้นให้ตรงตามรหัสนี้  
    passcodelock.auth_cb([](){
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
