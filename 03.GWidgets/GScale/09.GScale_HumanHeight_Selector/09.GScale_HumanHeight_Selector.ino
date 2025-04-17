/* ออกแบบสำหรับ 800x480  เป็น ตัวเลือกความสูง cm ให้ user เลือก โดยการปาดแบบเลื่อนๆ */

#include <BlynkGOv5.h>

#define SCALE_RANGE_MIN           75        // ความสูงคน ต่ำสุด cm  ใน scale
#define SCALE_RANGE_MAX           200       // ความสูงคน สูงสุด cm  ใน scale

#define SELECTOR_WIDTH            400       // px ความกว้างของตัว selector
#define TICK_PX                   4         // ระยะห่างระหว่าง tick (px) 
#define SCALE_RANGE               (SCALE_RANGE_MAX-SCALE_RANGE_MIN)   // ระยะ range (ระยะความสูงคน cm)
#define SCALE_WIDTH               (SCALE_RANGE * 10* TICK_PX)         // ความกว้างของ scale รวม ที่ไม่ถูก scroll

GRect rect_height_selector;
  GRect rect_scroll(rect_height_selector);
    GRect rect_scale_frame(rect_scroll);
      GScale scale_height_selector(rect_scale_frame);
  GLine  line_height_marker(rect_height_selector);

GLabel lb_height_selector;

/* เปลี่ยนความสูงของคน cm เป็น ระยะ px ของ scale ที่จะให้เลื่อนตำแหน่ง scroll ไป */
int32_t value_2_scroll_px(float v);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect_height_selector.size(SELECTOR_WIDTH, 80);
    rect_scroll.scrollable(true);
    rect_scroll.scrollbar(SCROLLBAR_ON, true, false);
    rect_scroll.anim_speed(500, GPART_SCROLLBAR);
    rect_scroll.bg_opa(0);
    rect_scroll.opa(0, GPART_SCROLLBAR);
      rect_scale_frame.size( SELECTOR_WIDTH + SCALE_WIDTH, 80);
      rect_scale_frame.bg_opa(0);
      rect_scale_frame.position(0,0);
        scale_height_selector.size(SCALE_WIDTH, 80);
        scale_height_selector.range(SCALE_RANGE_MIN, SCALE_RANGE_MAX);
        scale_height_selector.mode(SCALE_MODE_BOTTOM, SCALE_RANGE*10 +1, 10);
        scale_height_selector.tick_length(20, 10);
        scale_height_selector.axis_thickness(0);
        scale_height_selector.label_show(true);
    rect_scroll.event_cb(GWIDGET_EVENT_CB{
      if(event != EVENT_SCROLL) return;
      float value = SCALE_RANGE_MIN + rect_scroll.scroll_x()/(TICK_PX*10.0);
      lb_height_selector = String("ความสูง ") +String(value,1) + String(" cm");
    });

    line_height_marker.setPoints(Point(rect_height_selector.width()/2,0), Point(rect_height_selector.width()/2, 16));
    line_height_marker.thickness(3, TFT_RED);    

  lb_height_selector.align(rect_height_selector, ALIGN_TOP,0,-20);


  rect_scroll.scroll_to( value_2_scroll_px(150),0);
}

void loop(){
  BlynkGO.update();
}

int32_t value_2_scroll_px(float v) { 
  int32_t tick_num = (int32_t) floor( (v-SCALE_RANGE_MIN) *10);    // เปลี่ยความสูง เป็นจำนวน tick
  return tick_num*TICK_PX;
}
