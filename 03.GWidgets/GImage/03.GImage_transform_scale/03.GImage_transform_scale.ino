#include <BlynkGOv5.h>

IMAGE_DECLARE(img_coin_slot);

GImage coin_slot(img_coin_slot);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  coin_slot.align_center();

  static GAnimation anim;
  anim.init(coin_slot, [](GWIDGET, int32_t v){
    coin_slot.transform_scale(v);           // ทำการ scale ภาพ 
    coin_slot.opa(map(v,120,255,255,120));  // ทำโปร่งใส ภาพ
  },120,255,800, true);                     // ค่า เริ่มจาก 120 --> 255 ใน 800ms และให้เล่นซ้ำ
  anim.path_type(ANIM_PATH_EASE_IN);        // ค่าค่อยๆเพิ่มจากช้าไปเร็ว
  anim.start();                             // เริ่มให้ anim ทำงาน
}

void loop() {
  BlynkGO.update();
}

