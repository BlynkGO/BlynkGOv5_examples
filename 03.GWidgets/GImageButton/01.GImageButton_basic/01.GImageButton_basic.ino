#include <BlynkGOv5.h>

IMAGE_DECLARE(img_btn_released);
IMAGE_DECLARE(img_btn_pressed);

IMAGE_DECLARE(img_sw_on);
IMAGE_DECLARE(img_sw_off);

/* ปุ่มแบบ BUTTON_PUSH (กดแล้วปล่อยจะกลับสู่ภาพเดิม) */
GImageButton btn(img_btn_released, img_btn_pressed);  // แบบกำหนด 2 รูป คือ รูปตอนปล่อย กับ รูปตอนกด
// GImageButton btn(img_btn_released);                // แบบกำหนด 1 รูป คือ รูปตอนปล่อย เมื่อกด รูปจะเป็นสีเข้มขึ้นอัตโนมัติ

/* ปุ่มแบบ BUTTON_SWITCH ( กด ON กด OFF สลับกัน ) */
GImageButton sw(BUTTON_SWITCH, img_sw_off, img_sw_on); // แบบกำหนด 2 รูป คือ รูปตอน OFF และ รูปตอน ON
// GImageButton sw(BUTTON_SWITCH, img_sw_on);          // แบบกำหนด 1 รูป เฉพาะ รูปตอน ON  ส่วนตอนเมื่อสถานะ OFF รูปจะเป็นสีเข้มขึ้นอัตโนมัติ 

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  /* ปุ่มแบบ BUTTON_PUSH ให้ดักที่ onClicked (เมื่อมีการกด clicked) */
  btn.onClicked(GWIDGET_CB{
    Serial.println("[btn] clicked");
  });

  sw.align(btn, ALIGN_BOTTOM, 0, 30);
  /* ปุ่มแบบ BUTTON_SWITCH ให้ดักที่ onValueChanged (เมื่อมีการเปลี่ยนแปลงสถานะ ON <-> OFF)*/
  sw.onValueChanged(GWIDGET_CB{
    if(sw.isON()){
      Serial.println("[sw] ON");
    }else{
      Serial.println("[sw] OFF");
    }
  });
}

void loop(){
  BlynkGO.update();
}
