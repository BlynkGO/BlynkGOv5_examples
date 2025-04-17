#include <BlynkGOv5.h>

GSlider slider;
GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  slider.mode(SLIDER_MODE_RANGE);
  slider.range(0,100);
  slider.value_range(20,70);
  slider.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_TEAL,1));
  slider.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_TEAL,2), GPART_MAIN);
  slider.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_TEAL,1), GPART_KNOB);
  slider.border(3, TFT_PALETTE_DARKEN(TFT_PALETTE_TEAL,2), GPART_KNOB);
  slider.hookDrawTask(true);
  slider.onDrawTask(GWIDGET_CB{
    if( slider.draw_part() == GPART_KNOB ) {            // ขณะกำลังวาด knob
      GArea draw_area       = slider.draw_area();       // พื้นที่ knob ที่กำลังอยู่ระหว่างวาด
      GArea left_knob_area  = slider.left_knob_area();  // พื้นที่ปุ่ม knob ซ้ายของ slider
      if( draw_area == left_knob_area ) {               // หาก knob ที่วาดอยู่ ตรงกับพื้นที่ knob ซ้าย 
        if( slider.draw_type() == DRAW_TYPE_BORDER) {   // ขณะกำลังวาด knob ซ้าย ในส่วนของ border
          auto border_dsc = slider.draw_border_dsc();   // รายละเอียดของ border ของ knob ซ้าย ขณะกำลังวาดอยู่
          border_dsc->color = TFT_COLOR(TFT_BLACK);     // เปลี่ยน สี border ของ knob ซ้าย เป็น สีอื่นจากปกติ
        }
      }
    }
  });
  slider.onValueChanged(GWIDGET_CB{
    GArea area = slider.indic_area();
    label = StringX::printf("%d - %d", slider.left_value(), slider.value());
    label.center((area.p[0].x+ area.p[1].x)/2, (area.p[0].y-20));
  });
  
  static SoftTimer timer;
  timer.delay(100,[](){
    slider.event_send(EVENT_VALUE_CHANGED);
  });
}

void loop(){
  BlynkGO.update();
}
