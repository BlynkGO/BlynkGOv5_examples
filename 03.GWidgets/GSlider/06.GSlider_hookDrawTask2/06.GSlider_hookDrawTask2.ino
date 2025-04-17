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
  slider.border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_TEAL,2), GPART_KNOB);
  slider.hookDrawTask(true);
  slider.onDrawTask(GWIDGET_CB{
    enum KnobType { KNOB_NONE = 0, KNOB_LEFT = 1, KNOB_RIGHT = 2 };
    static uint8_t last_pressed_drawn_knob = KNOB_NONE;
    if( slider.draw_part() == GPART_KNOB && 
        slider.draw_type() == DRAW_TYPE_BORDER)         // ขณะกำลังวาด knob ในส่วนของ border
    {
      GArea draw_area       = slider.draw_area();       // พื้นที่ knob ที่กำลังอยู่ระหว่างวาด
      GArea left_knob_area  = slider.left_knob_area();  // พื้นที่ปุ่ม knob ซ้ายของ slider
      GArea right_knob_area = slider.right_knob_area(); // พื้นที่ปุ่ม knob ขวาของ slider
      Point p               = slider.touch_point(true); // จุดสัมผัสแบบ absolute จากจุด (0,0) จอ

      if( slider.isPressed()) {   // ถ้า slider ถูกกด  (ตอนนี้คือ knob ถูกกด เพราะกำหนดวาด knob อยู่)
        bool flag_must_change_style =false;
        // หาก knob ที่วาดอยู่ ตรงกับพื้นที่ knob ซ้าย   และ จุดสัมผัส พิกัด x อยู่ระหว่าง ช่วง พิกัด x ของ พื้นที่ knob ซ้าย
        if( draw_area == left_knob_area && (left_knob_area.p[0].x <= p.x && left_knob_area.p[1].x >= p.x)) {
          if( last_pressed_drawn_knob == 0 || last_pressed_drawn_knob == KNOB_LEFT) {
            last_pressed_drawn_knob = KNOB_LEFT;
            flag_must_change_style = true;              // กำหนดให้ต้องมีการเปลี่ยน style ของ border ของ knob
          }
        }else
        // หาก knob ที่วาดอยู่ ตรงกับพื้นที่ knob ขวา   และ จุดสัมผัส พิกัด x อยู่ระหว่าง ช่วง พิกัด x ของ พื้นที่ knob ขวา
        if( draw_area == right_knob_area && (right_knob_area.p[0].x <= p.x && right_knob_area.p[1].x >= p.x)){
          if( last_pressed_drawn_knob == KNOB_NONE || last_pressed_drawn_knob == KNOB_RIGHT) {
            last_pressed_drawn_knob = KNOB_RIGHT;
            flag_must_change_style = true;              // กำหนดให้ต้องมีการเปลี่ยน style ของ border ของ knob
          }
        }

        if(flag_must_change_style) {
          auto border_dsc = slider.draw_border_dsc();   // รายละเอียดของ border ของ knob ที่ถูกกด ขณะกำลังวาดอยู่
          border_dsc->color = TFT_COLOR(TFT_BLUE);      // เปลี่ยน style ของ border ของ knob ที่ถูกกด ขณะกำลังวาดอยู่ เป็น สีอื่นจากปกติ
          border_dsc->width = 5;                        // เปลี่ยน style ของ border ของ knob ที่ถูกกด ขณะกำลังวาดอยู่ เป็น ขนาดอื่นจากปกติ
        }
      }else{
        last_pressed_drawn_knob = KNOB_NONE;            // เมื่อ knob ไม่ได้ถูกกด ให้ไม่ต้องจำ knob สุดท้ายที่เคยถูกกด
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
