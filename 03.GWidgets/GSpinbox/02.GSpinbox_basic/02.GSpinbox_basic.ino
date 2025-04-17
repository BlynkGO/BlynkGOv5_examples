#include <BlynkGOv5.h>

GSpinbox spinbox;
GButton  btn_plus;
GButton  btn_minus;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  spinbox.range(-1000,25000);
  spinbox.digit_format(5,2);        // ตัวเลขทั้งหมด 5 ตัว โดยเลขก่อนทศนิยม 2 หลัก
  spinbox.step_prev();
  spinbox.width(100);
  spinbox.onValueChanged(GWIDGET_CB{
    Serial.println(spinbox.value());
  });

  auto btn_event_cb = 
    GWIDGET_EVENT_CB{
      if(event==EVENT_CLICKED || event== EVENT_LONG_PRESSED_REPEAT ){
        if(btn_plus == widget){
          spinbox.increment();
        }else
        if(btn_minus == widget){
          spinbox.decrement();
        }
      }
    };

  btn_plus.round_design();
  btn_plus.size(spinbox.height(), spinbox.height());
  btn_plus = SYMBOL_PLUS;
  btn_plus.align(spinbox, ALIGN_RIGHT,5);
  btn_plus.event_cb(btn_event_cb);

  btn_minus.round_design();
  btn_minus.size(btn_plus);
  btn_minus = SYMBOL_MINUS;
  btn_minus.align(spinbox, ALIGN_LEFT,-5);
  btn_minus.event_cb(btn_event_cb);
}

void loop(){
  BlynkGO.update();
}
