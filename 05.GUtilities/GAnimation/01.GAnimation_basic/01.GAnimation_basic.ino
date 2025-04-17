#include <BlynkGOv5.h>

IMAGE_DECLARE(img_football);                      // ประกาศทรัพยากร รูป c file ก่อนนำไปใช้ตามวิตเจ็ตต่างๆ

GImage football(img_football);                    // วิตเจ็ตรูปภาพ football โดยโหลดรูป แบบ c file
GAnimation animation[2];                          // ประกาศตัวแปร GAnimation 2 ชุด

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_GREEN);
  
  football.create();
  
  // กำหนดให้ animation ใช้สำหรับ วิตเจ็ตรูปภาพ football 
  // ด้วยการปรับค่ามุม ด้วยฟังกชั่น ANIM_ANGLE (ฟังกชั่นภายใน BlynkGO ที่เตรียมไว้ให้ )
  // โดย ให้ปรับ ค่ามุมหมุนจาก 0 ไป 360 (ตามเข็มจาก 12 นาฬิกา) ภายใน 4 วินาที 
  animation[0].init(football, ANIM_ANGLE, 0, 360, 4000);
  animation[0].repeat_count(ANIM_REPEAT_INFINITE);      // ให้มีการเล่นซ้ำไปเรื่อยๆ หลัง animation ครบรอบ
  animation[0].start();                                 // ทำการลงทะเบียนให้ animation[0] เริ่มทำงาน

  // กำหนดให้ animation ใช้สำหรับ วิตเจ็ตรูปภาพ football 
  // ด้วยการเปลี่ยนตำแหน่ง x ของวิตเจ็ต ด้วยฟังกชั่น ANIM_X (ฟังกชั่นภายใน BlynkGO ที่เตรียมไว้ให้ )
  // เริ่มจาก x = -football.width() ไปจนถึง x = BlynkGO.width() ภายใน 10 วินาที
  animation[1].init(football, ANIM_X, -football.width(), BlynkGO.width(), 10000 );
  animation[1].repeat_count(ANIM_REPEAT_INFINITE);      // ให้มีการเล่นซ้ำไปเรื่อยๆ  หลัง animation ครบรอบ
  animation[1].start();                                 // ทำการลงทะเบียนให้ animation[1] เริ่มทำงาน

}

void loop() {
  BlynkGO.update();
}




