#include <BlynkGOv5.h>

GRect panel_pull_down;
  GLabel lb_panel_pull_down(panel_pull_down);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  panel_pull_down.color(TFT_BLUE);
  panel_pull_down.position(0, -panel_pull_down.height());
    lb_panel_pull_down = "Panel Pull-down";
    lb_panel_pull_down.color(TFT_WHITE);

  GScreen.onGesture(GWIDGET_CB{
    static GAnimation anim;
    static bool is_animating;

    if(is_animating) return;

    panel_pull_down.toForeground();           // ย้ายมาบนสุด

    if(GScreen.gesture_dir()== DIR_BOTTOM) {  // gesture ลากลง
      if( panel_pull_down.posY() == -panel_pull_down.height()) {
        is_animating = true;
        anim.init(panel_pull_down, ANIM_Y, -panel_pull_down.height(), 0, 500);
        anim.onCompleted([](GAnimation *a){ is_animating = false; });
        anim.start();
      }
    } else
    if(GScreen.gesture_dir()== DIR_TOP) {     // gesture ลากขึ้น
      if( panel_pull_down.posY() == 0) {
        is_animating = true;
        anim.init(panel_pull_down, ANIM_Y, 0, -panel_pull_down.height(), 500);
        anim.onCompleted([](GAnimation *a){ is_animating = false; });
        anim.start();
      }
    }
  });
}

void loop() {
  BlynkGO.update();
}
