#include <BlynkGOv5.h>

#define PASSCODE       135790       // รหัส passcode เริ่มต้น

GPasscodeLock passcodelock;
GLabel  lb_passcode;
GButton btn_change_passcode;
uint32_t passcode = PASSCODE;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

#if defined(ESP32)
  // หากเคยมีจำ passcode เดิมไว้ที่ flashMem ให้ดึงออกมาคืน
  if(BlynkGO.flashMem_exists("Passcode")){
    passcode = BlynkGO.flashMem_Int("Passcode");
  }
#endif

  lb_passcode = String("รหัสปัจจุบัน : " ) + String(passcode);
  btn_change_passcode = "ตั้งรหัสใหม่";
  btn_change_passcode.align(lb_passcode, ALIGN_BOTTOM, 0, 40);
  btn_change_passcode.onClicked(GWIDGET_CB{
    passcodelock.passcode(passcode);       // ใส่รหัสเก่า
    passcodelock.mode(GPASSCODELOCK_MODE_SETTING);
    passcodelock.auth_cb([](){
      passcode = passcodelock.passcode();  // รหัสที่เปลี่ยนใหม่
      Serial.println(String("รหัสใหม่ : " ) + String(passcode));
      lb_passcode = String("รหัสใหม่ : " ) + String(passcode);
#if defined(ESP32)
      // หากเคยมีจำ passcode เดิมไว้ที่ flashMem ให้ล้าง แล้วบันทึก passcode ใหม่ลง flashMem
      if(BlynkGO.flashMem_exists("Passcode")) BlynkGO.flashMem_erase("Passcode");
      BlynkGO.flashMem("Passcode", passcode);
#endif
    });
  });

}

void loop(){
  BlynkGO.update();
}
