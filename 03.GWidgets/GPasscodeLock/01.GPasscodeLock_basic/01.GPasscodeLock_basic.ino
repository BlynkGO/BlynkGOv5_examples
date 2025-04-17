#include <BlynkGOv5.h>

#define PASSCODE       135790       // รหัส passcode

GWiFiManager wifi_manager;
GRect  rect_wifi_manager;
GPasscodeLock passcodelock;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT, -10);

  // สร้าง สี่เหลี่ยมโปร่งใส ขนาด ให้เมื่อวาง บัง wifi_manager ได้
  rect_wifi_manager.size(80,80);
  rect_wifi_manager.bg_opa(0);
  rect_wifi_manager.align(wifi_manager, ALIGN_CENTER);
  rect_wifi_manager.onClicked(GWIDGET_CB{
    passcodelock.passcode(PASSCODE);

    // User ใส่รหัสถูกต้อง ให้ไปทำอะไรต่อ
    passcodelock.auth_cb([](){
      Serial.println("[GPasscodeLock] Authentication verified!");
      wifi_manager.event_send(EVENT_CLICKED);
    });

    // User ใส่รหัสไม่ถูกต้อง ครบ 3 ครั้ง หรือ User ได้กดปุ่ม Cancel ยกเลิก ให้ไปทำอะไรต่อ
    passcodelock.cancel_cb([](){
      Serial.println("[GPasscodeLock] Authentication failed or cancelled");
    });
  });

}

void loop(){
  BlynkGO.update();
}
