#include <BlynkGOv5.h>

IMAGE_DECLARE(img_roller_mask);

GContainer cont_clock(LAYOUT_ROW_T,0,0,0,0,10);
  GRoller roller_hour  (cont_clock);
  GRoller roller_minute(cont_clock);
  GRoller roller_second(cont_clock);

GLabel lbClock;

uint8_t hour   = 0;
uint8_t minute = 0;
uint8_t second = 0;


void roller_onValueChange(GWidget *widget){
  hour    = roller_hour   .toInt();
  minute  = roller_minute .toInt();
  second  = roller_second .toInt();

  lbClock = StringX::printf("เวลา %02d:%02d:%02d", hour, minute, second);
}

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  lbClock  = StringX::printf("เวลา %02d:%02d:%02d", hour, minute, second);
  lbClock.font(prasanmit_35,TFT_WHITE);
  lbClock.align( ALIGN_TOP,0,10);        // จัดตำแหน่ง GLabel

  cont_clock.bg_opa(0);
  
    roller_hour.options(0,23, true);      // สร้าง options จาก 0 ถึง 23 โดย true คือ ให้มี 0 นำหน้าด้วย
    roller_hour.width(70);                // กำหนดความกว้าง
    roller_hour.border(3, TFT_RED, GPART_SELECTED);
    roller_hour.font(prasanmit_40, prasanmit_20);
    roller_hour.mask(img_roller_mask);
    roller_hour.onValueChanged(roller_onValueChange);
    
    roller_minute.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
    roller_minute.width(70);
    roller_minute.border(3, TFT_RED, GPART_SELECTED);
    roller_minute.font(prasanmit_40, prasanmit_20);
    roller_minute.mask(img_roller_mask);
    roller_minute.onValueChanged(roller_onValueChange);

    roller_second.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
    roller_second.width(70);
    roller_second.border(3, TFT_RED, GPART_SELECTED);
    roller_second.font(prasanmit_40, prasanmit_20);
    roller_second.mask(img_roller_mask);
    roller_second.onValueChanged(roller_onValueChange);

  cont_clock.align(lbClock, ALIGN_BOTTOM, 0, 10);
}


void loop(){
  BlynkGO.update();
}
