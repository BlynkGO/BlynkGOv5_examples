#include <BlynkGOv5.h>

GRect panel_pull_down;
  GLabel lb_panel_pull_down(panel_pull_down);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  /* panel สำหรับลากลง */
  panel_pull_down.color(TFT_BLUE);
  panel_pull_down.position(0, -panel_pull_down.height());
    lb_panel_pull_down = "Panel Pull-down";
    lb_panel_pull_down.color(TFT_WHITE);
    

  GScreen.onGesture(GWIDGET_CB{
    static GAnimation anim;
    static bool is_animating;

    // ถ้ากำลังเคลื่อนไหว ให้ออกจากฟังก์ชัน
    if(is_animating) return;

    // ย้าย panel ไปยังชั้นด้านบนสุด (foreground)
    panel_pull_down.toForeground();           // ย้ายมาบนสุด

    // ตรวจสอบการเลื่อนลง (gesture ลากลง)
    if(GScreen.gesture_dir()== DIR_BOTTOM) {  // gesture ลากลง
      if( panel_pull_down.posY() == -panel_pull_down.height()) {
        is_animating = true;
        // สร้าง animation จากตำแหน่งนอกหน้าจอ ไปถึง ตำแหน่ง 0 (ระยะเวลา 500ms)
        anim.init(panel_pull_down, ANIM_Y, -panel_pull_down.height(), 0, 500);
        // เมื่อ animation เสร็จ ให้เปลี่ยนสถานะกลับเป็น false
        anim.onCompleted([](GAnimation *a){ is_animating = false; });
        // เริ่มการเคลื่อนไหว
        anim.start();
      }
    } else
    // ตรวจสอบการเลื่อนขึ้น (gesture ลากขึ้น)
    if(GScreen.gesture_dir()== DIR_TOP) {     // gesture ลากขึ้น
      if( panel_pull_down.posY() == 0) {
        is_animating = true;
        // สร้าง animation จากตำแหน่ง 0 ไปถึง นอกหน้าจอบนสุด (ระยะเวลา 500ms)
        anim.init(panel_pull_down, ANIM_Y, 0, -panel_pull_down.height(), 500);
        // เมื่อ animation เสร็จ ให้เปลี่ยนสถานะกลับเป็น false
        anim.onCompleted([](GAnimation *a){ is_animating = false; });
        // เริ่มการเคลื่อนไหว
        anim.start();
      }
    }
  });
}

void loop() {
  BlynkGO.update();
}
