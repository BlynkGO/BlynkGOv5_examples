#include <BlynkGOv5.h>

IMAGE_DECLARE(img_wave1);
IMAGE_DECLARE(img_wave2);
IMAGE_DECLARE(img_wave3);

GImage wave1(img_wave1);
GImage wave2(img_wave2);
GImage wave3(img_wave3);

GAnimation anim_wave1;
GAnimation anim_wave2;
GAnimation anim_wave3;

GLabel label;

void setup() {
  BlynkGO.begin();
  
  // 1. พื้นหลัง (Background) - มืดนิดๆ เพื่อให้สีฟ้าคลื่นลอยออกมา
  BlynkGO.fillScreen(TFT_COLOR_HEX(0x0A1931)); 

  // Wave 1 (หลังสุด - กว้าง 750px) - สีน้ำเงินเข้มที่สุด
  wave1.width(PCT(100));
  wave1.image_align(IMAGE_ALIGN_TILE);
  wave1.recolor(TFT_COLOR_HEX(0x082a85), 200);    // ใช้สีน้ำเงินทางดำมาผสม ให้ดูเป็นเงาน้ำลึก
  wave1.opa(150); 
  wave1.align(ALIGN_BOTTOM, 0, 0);

  // Wave 2 (กลาง - กว้าง 500px) - สีฟ้าสว่างขึ้นและเริ่มโปร่งแสง
  wave2.width(PCT(100));
  wave2.image_align(IMAGE_ALIGN_TILE);
  wave2.recolor(TFT_COLOR_HEX(0x0077FF), 255);
  wave2.opa(180); 
  wave2.align(ALIGN_BOTTOM, 0, 0);

  // Wave 3 (หน้าสุด - กว้าง 333px) - สีฟ้า Cyan สว่างและโปร่งแสงที่สุด
  wave3.width(PCT(100));
  wave3.image_align(IMAGE_ALIGN_TILE);
  wave3.recolor(TFT_COLOR_HEX(0x00D4FF), 255);
  wave3.opa(120); 
  wave3.align(ALIGN_BOTTOM, 0, 0);

  anim_wave1.init([](GWIDGET, int32_t v){ wave1.offset_x(v); }, 0, - img_wave1.header.w, 20000, true);  anim_wave1.start();
  anim_wave2.init([](GWIDGET, int32_t v){ wave2.offset_x(v); }, 0, - img_wave2.header.w,  8000, true);  anim_wave2.start();
  anim_wave3.init([](GWIDGET, int32_t v){ wave3.offset_x(v); }, 0, - img_wave3.header.w,  3000, true);  anim_wave3.start();

  label = "ระบบกรองน้ำดื่ม";
  label.font(prasanmit_40, TFT_WHITE);
  label.align_center(0,-30);
}

void loop(){
  BlynkGO.update();
}
