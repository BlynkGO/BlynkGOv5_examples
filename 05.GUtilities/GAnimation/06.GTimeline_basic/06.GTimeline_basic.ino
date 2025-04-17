#include <BlynkGOv5.h>

#define RECT_WIDTH    90
#define RECT_HEIGHT   70

GContainer cont_rect(LAYOUT_ROW_M,10,10,10,10,20);
  GRect rect1(cont_rect);
  GRect rect2(cont_rect);
  GRect rect3(cont_rect);

GSlider slider_timeline;
GLabel  lb_timeline;

GButton btn(BUTTON_SWITCH);

GTimeline timeline;
  GAnimation anim1;
  GAnimation anim2;
  GAnimation anim3;

void setup() {
  Serial.begin(9600);
  BlynkGO.begin();

  cont_rect.bg_opa(0);
    rect1.size(RECT_WIDTH, RECT_HEIGHT);
    rect2.size(RECT_WIDTH, RECT_HEIGHT);
    rect3.size(RECT_WIDTH, RECT_HEIGHT);

  slider_timeline.align(cont_rect, ALIGN_BOTTOM,0,30);
  slider_timeline.range(0, PROGRESS_MAX);                 // PROGRESS_MAX = 65535
  slider_timeline.onValueChanged(GWIDGET_CB{
    timeline = slider_timeline.toInt();                   // กำหนดค่า progress ปัจจุบันให้ timeline (อยุ่ระหว่าง 0- PROGRESS_MAX = 65535)
    lb_timeline = timeline.progress_ms() + String(" ms"); // แสดงค่า progress ในหน่วย ms (ค่าระหว่าง 0- playtime)
  });
  lb_timeline = "0 ms";
  lb_timeline.align(slider_timeline, ALIGN_BOTTOM,0,20);
  
  btn = SYMBOL_STOP " หยุด";
  btn.align(slider_timeline,ALIGN_TOP, 0, -RECT_HEIGHT -30 - 50);
  btn.event_cb(GWIDGET_EVENT_CB{
    static bool is_longpressed;
    switch(event){
      case EVENT_PRESSED:
        is_longpressed = false;
        break;
      case EVENT_LONG_PRESSED:
        is_longpressed = true;
        timeline.stop();
        btn.OFF();
        btn = SYMBOL_STOP " หยุด";
        break;
      case EVENT_CLICKED:
        if(is_longpressed) { btn.OFF(); return; }
        if(btn.isON()) {
          timeline.start();
          btn = SYMBOL_PLAY " เล่น";
        }else{
          timeline.pause();
          btn = SYMBOL_PAUSE " หยุดชั่วคราว";
        }
        break;
    }
  });

  anim1.init(rect1, [](GWIDGET, int32_t v){
    rect1.size( map(v,0,1024,0,RECT_WIDTH),  map(v,0,1024,0,RECT_HEIGHT));
  }, 0,1024,300);                       // ค่า v : 0-1024 ใน 300 ms โดย ค่าเพิ่มแบบ LINEAR

  anim2.init(rect2, [](GWIDGET, int32_t v){
    rect2.size( map(v,0,1024,0,RECT_WIDTH),  map(v,0,1024,0,RECT_HEIGHT));
  }, 0,1024,300, ANIM_PATH_EASE_OUT);   // ค่า v : 0-1024 ใน 300 ms โดย ค่าเพิ่มแบบ EASE_OUT  (เร็วไปช้า)

  anim3.init(rect3, [](GWIDGET, int32_t v){
    rect3.size( map(v,0,1024,0,RECT_WIDTH),  map(v,0,1024,0,RECT_HEIGHT));
  }, 0,1024,300, ANIM_PATH_OVERSHOOT);  // ค่า v : 0-1024 ใน 300 ms โดย ค่าเพิ่มแบบ OVERSHOOT (ค่าพุ่งไปเกินระดับจุดหมายเล็กๆแล้วเข้าสู่ค่าจุดหมาย)

  timeline.add(0  , anim1);     // timeline ณ เวลา ms : 0    ให้เริ่มเล่น anim1
  timeline.add(200, anim2);     // timeline ณ เวลา ms : 200  ให้เริ่มเล่น anim2
  timeline.add(400, anim3);     // timeline ณ เวลา ms : 400  ให้เริ่มเล่น anim3
  timeline.progress_ms(200);

  Serial.println(timeline.playtime());  // แสดงระยะเวลา timeline ทั้งหมด (ms)

  // เมื่อ timeline กำลังทำงาน ค่า progress จะ เปลี่ยนไป ระหว่าง 0 -- PROGRESS_MAX = 65535
  timeline.onProgress([](GTIMELINE){
    slider_timeline = timeline.toInt();                     // progress ของ timeline มีค่า 0 -- PROGRESS_MAX = 65535
    lb_timeline = timeline.progress_ms() + String(" ms");   // แสดงค่า progress ในหน่วย ms (ค่าระหว่าง 0- playtime)
  });

  // เมื่อ timeline ทำงานไปจนจบในรอบ ทิศทาง นั้นๆ
  timeline.onCompleted([](GTIMELINE){
    if(btn.isON()){
      timeline.reverse(!timeline.reverse());  // กลับทิศทาง จากเดิม
      timeline.start();
    }
  });

  // timeline.progress_ms(200);       // กำหนดค่าเริ่มต้น ให้ timeline ไปเริ่มที่ เวลา ms : 200

}

void loop(){
  BlynkGO.update();
}

