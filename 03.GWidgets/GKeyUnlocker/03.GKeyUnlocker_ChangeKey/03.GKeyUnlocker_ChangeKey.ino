
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

GKeyUnlocker keyunlocker;
GLabel  lb_key;
GButton btn_change_key;
uint32_t key = KEY;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

#if defined(ESP32)
  // หากเคยมีจำ key เดิมไว้ที่ flashMem ให้ดึงออกมาคืน
  if(BlynkGO.flashMem_exists("SecretKey")){
    key = BlynkGO.flashMem_Int("SecretKey");
  }
#endif

  lb_key = String("รหัสปัจจุบัน : " ) + String(key);
  btn_change_key = "ตั้งรหัสใหม่";
  btn_change_key.align(lb_key, ALIGN_BOTTOM, 0, 40);
  btn_change_key.onClicked(GWIDGET_CB{
    keyunlocker.secretkey(key);       // ใส่รหัสเก่า
    keyunlocker.mode(GKEYUNLOCKER_MODE_SETTING);
    keyunlocker.auth_cb([](){
      key = keyunlocker.secretkey();  // รหัสที่เปลี่ยนใหม่
      Serial.println(String("รหัสใหม่ : " ) + String(key));
      lb_key = String("รหัสใหม่ : " ) + String(key);
#if defined(ESP32)
      // หากเคยมีจำ key เดิมไว้ที่ flashMem ให้ล้าง แล้วบันทึก key ใหม่ลง flashMem
      if(BlynkGO.flashMem_exists("SecretKey")) BlynkGO.flashMem_erase("SecretKey");
      BlynkGO.flashMem("SecretKey", key);
#endif
    });
  });

}

void loop(){
  BlynkGO.update();
}

