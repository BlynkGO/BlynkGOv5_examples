#include <BlynkGOv5.h>

IMAGE_DECLARE(img_cat_samuri_sprite_sheet);  // sprite sheet  รวม sprite ขนาด 128x128px จำนวน 16 รูป (4x4)

GImage cat_samuri(img_cat_samuri_sprite_sheet);
GAnimation cat_anim;

GImage cat_samuri2(img_cat_samuri_sprite_sheet);
  GRect  frame_cat_samuri2(cat_samuri2);
  GLabel lb_cat_samuri2(cat_samuri2);

void cat_samuri_sprite(uint8_t id) {  // 0..
  id = id % 16;  // จำกัดค่าให้อยู่ในช่วง 0-15
  // 1. กำหนดค่าคงที่
  const int sprite_size = 128;  const int columns = 4;

  // 2. คำนวณหาตำแหน่ง Column และ Row
  // ใช้ % (mod) เพื่อหาลำดับในแถว (0, 1, 2, 3)  ; ใช้ / (หาร) เพื่อหาว่าอยู่แถวไหน (0, 1, 2, 3)
  int col = id % columns;   int row = id / columns;

  // 3. คำนวณค่า Offset (ต้องเป็นค่าลบเพื่อเลื่อนภาพเข้ากรอบ)
  int offset_x = -(col * sprite_size);
  int offset_y = -(row * sprite_size);

  // 4. นำไปใช้งานกับ Object ของคุณ
  cat_samuri.size(sprite_size, sprite_size);    // ขนาดของ sprite แต่ละรูป
  cat_samuri.offset(offset_x, offset_y);        // ปรับเลื่อน offset เพื่อแสดง sprite ที่ถูกต้อง
  cat_samuri.image_align(IMAGE_ALIGN_TILE);     // กำหนดให้แสดงภาพแบบ tile เพื่อใช้ offset ในการเลือกส่วนของ sprite sheet
}


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cat_samuri.align(ALIGN_RIGHT, -100, 0);
  cat_anim.init([](GWIDGET, int32_t value){
    cat_samuri_sprite(value);
  }, 0, 15, 100*16, true);
  cat_anim.start();

  cat_samuri2.align(ALIGN_LEFT, 0, 0);
  cat_samuri2.scale(180,180);
    int32_t frame_w = cat_samuri2.width()*180/256;
    frame_cat_samuri2.size(frame_w,frame_w);
    frame_cat_samuri2.bg_opa(0);
    frame_cat_samuri2.border(2, TFT_CYAN);
    lb_cat_samuri2 = "Cat Samuri Sprite Sheet";
    lb_cat_samuri2.color(TFT_WHITE);
    lb_cat_samuri2.align(frame_cat_samuri2, ALIGN_BOTTOM);
}

void loop(){
  BlynkGO.update();
}
