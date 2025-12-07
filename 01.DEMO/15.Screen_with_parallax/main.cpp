#include <BlynkGOv5.h>

IMAGE_DECLARE(img_nature_1);
IMAGE_DECLARE(img_nature_2);
IMAGE_DECLARE(img_nature_3);

IMAGE_DECLARE(img_icon1);
IMAGE_DECLARE(img_icon2);
IMAGE_DECLARE(img_icon3);

GPage page_main;                    // layer สำหรับทำ scrollbar
  GContainer cont_main(page_main);  // layer สำหรับรวมแต่ละหน้าเรียงกันตามแถว
    GContainer screen[3];           // screen แต่ละหน้า
      GImage bg[3] = { GImage(img_nature_1), GImage(img_nature_2), GImage(img_nature_3) };  // ภาพพื้นหลังในแต่ละ screen
      GImage icon[3] = { GImage(img_icon1), GImage(img_icon2), GImage(img_icon3) };         // วิตเจ็ตอื่นๆ ในแต่ละ screen

int w = BlynkGO.width();
int h = BlynkGO.height();

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // ตั้งค่า page_main scrollable horizontal
  page_main.scrollbar(SCROLLBAR_OFF, true, false); // ซ่อน scrollbar
  page_main.size(w, h);
  page_main.bg_opa(0);
  page_main.radius(0);
  page_main.padding(0);

  // container หลัก จัด layout row
  cont_main.bg_opa(0);
  cont_main.layout(LAYOUT_ROW_M);
  cont_main.padding(0);
  cont_main.radius(0);
  cont_main.position(0,0);

  for (int i = 0; i < 3; i++) {
    // screen เป็นลูกของ cont_main
    screen[i].size(w, h);
    screen[i].parent(cont_main);
    screen[i].bg_opa(0);
    screen[i].radius(0);
    screen[i].padding(0);
    screen[i].layout(LAYOUT_NONE);

    // background image ของแต่ละ page
    bg[i].parent(screen[i]);
    bg[i].scale_x(255*1.5);         // ให้รูปด้านกว้างใหญ่กว่าปกติ 1.5 เท่า
    bg[i].align_center(-w*0.25, 0); // offset ครึ่งหนึ่ง

    icon[i].parent(screen[i]);
  }

  // ------------------------
  // Scroll event ของ page_main
  // ตรวจจับการ scroll และ adjust background image ตามการเลื่อน
  // หากเลื่อนไปซ้าย background จะขยับตัวตามแบบ parallax effect
  // เป็นเทคนิค parallax scrolling ที่ทำให้ background เคลื่อนที่ช้ากว่า foreground
  // ------------------------
  page_main.event_cb([](GWIDGET, event_t event){
    static int32_t scroll_start = 0;  // บันทึกตำแหน่งเริ่มต้น scroll
    int screen_count = 3;               // จำนวน screen ทั้งหมด (3 หน้า)
    int screen_w = w;                   // ความกว้างของแต่ละ screen (800px)

    // เมื่อผู้ใช้เริ่มเลื่อน
    if(event == EVENT_SCROLL_BEGIN){
      scroll_start = page_main.scroll_x();  // บันทึกตำแหน่งเริ่ม scroll เพื่อคำนวณระยะ
    }
    // ระหว่างการเลื่อน (continuously)
    else if(event == EVENT_SCROLL){
      int32_t scroll_x = page_main.scroll_x();  // ตำแหน่ง scroll ปัจจุบัน

      // วนลูปแต่ละ screen และปรับตำแหน่ง background image
      for(int i = 0; i < screen_count; i++){
        // คำนวณจุดกึ่งกลางของ screen ที่ i
        int32_t cont_center = i*screen_w + screen_w/2;
        // คำนวณจุดกึ่งกลางที่กำลังแสดงบนหน้าจอ
        int32_t widget_center = scroll_x + screen_w/2;

        // คำนวณ ratio ว่า screen ห่างจากจุดกึ่งกลางหน้าจอแค่ไหน (-1 ถึง 1)
        float ratio = float(cont_center - widget_center)/(screen_w/2.0f);
        if(ratio < -1.0f) ratio = -1.0f;  // จำกัดค่าต่ำสุด
        if(ratio > 1.0f) ratio = 1.0f;    // จำกัดค่าสูงสุด

        // คำนวณขนาด background ที่ใหญ่กว่า (1.5 เท่า)
        int32_t bg_w = w*1.5;
        // คำนวณส่วนเกิน (extra width) ที่ใช้เคลื่อนที่ parallax
        int32_t extra = bg_w - screen_w;
        // คำนวณ offset โดยใช้ ratio เพื่อได้ parallax effect
        int32_t offset = -(int32_t)(ratio * (extra/2));

        // ปรับตำแหน่ง background image ด้วย parallax offset
        bg[i].align_center(-w*0.25 + offset, 0);    // ทำให้รูปพื้นหลังเคลื่อนที่ช้ากว่าการ scroll
      }
    }
    // เมื่อผู้ใช้ปล่อยการเลื่อน (snap to page)
    else if(event == EVENT_SCROLL_END){
      int32_t scroll_x = page_main.scroll_x();
      // คำนวณ screen index ที่ใกล้ที่สุด
      int page_index = (scroll_x + screen_w/2)/screen_w;

      // จำกัดค่า screen index ให้อยู่ในช่วง
      if(page_index < 0) page_index = 0;
      if(page_index >= screen_count) page_index = screen_count-1;

      // Snap ไปที่ screen ที่เลือกแบบ smooth animation (200ms)
      page_main.scroll_to_x(page_index * screen_w, ANIM_ON);
      page_main.anim_duration(200, GPART_SCROLLBAR);
    }
  });
}

void loop() {
  BlynkGO.update();
}
