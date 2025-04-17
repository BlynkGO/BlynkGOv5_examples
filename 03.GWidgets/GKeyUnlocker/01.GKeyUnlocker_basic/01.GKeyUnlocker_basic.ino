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

GWiFiManager wifi_manager;
GRect  rect_wifi_manager;
GKeyUnlocker keyunlocker;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT, -10);

  // สร้าง สี่เหลี่ยมโปร่งใส ขนาด ให้เมื่อวาง บัง wifi_manager ได้
  rect_wifi_manager.size(80,80);
  rect_wifi_manager.bg_opa(0);
  rect_wifi_manager.align(wifi_manager, ALIGN_CENTER);
  rect_wifi_manager.onClicked(GWIDGET_CB{
    keyunlocker.secretkey(KEY);

    // User ใส่รหัสถูกต้อง ให้ไปทำอะไรต่อ
    keyunlocker.auth_cb([](){
      Serial.println("[GKeyUnlocker] Authentication verified!");
      wifi_manager.event_send(EVENT_CLICKED);
    });

    // User ใส่รหัสไม่ถูกต้อง ครบ 3 ครั้ง หรือ User ได้กดปุ่ม Cancel ยกเลิก ให้ไปทำอะไรต่อ
    keyunlocker.cancel_cb([](){
      Serial.println("[GKeyUnlocker] Authentication failed or cancelled");
    });
  });

}

void loop(){
  BlynkGO.update();
}

