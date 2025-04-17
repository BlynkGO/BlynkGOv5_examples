#include <BlynkGOv5.h>

IMAGE_DECLARE(img_roller_mask);

GContainer cont_date(LAYOUT_ROW_T,0,0,0,0,10);
  GRoller roller_day  (cont_date);
  GRoller roller_month(cont_date);
  GRoller roller_year (cont_date);

  String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                        "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                        "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

GLabel lbDate;
  String dd = "1";
  String mm = "มกราคม";
  String yy = "2510";

void roller_onValueChange(GWidget *widget){
  dd  = roller_day  .toString();
  mm  = roller_month.toString();
  yy  = roller_year .toString();
  lbDate = dd + " " + mm + " " + yy;
}

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  lbDate  = dd + " " + mm + " " + yy;   // สร้าง GLabel โดยการนำ String มาต่อกันได้เลย
  lbDate.font(prasanmit_35,TFT_WHITE);  // เปลี่ยน font
  lbDate.align( ALIGN_TOP, 0,10);       // จัดตำแหน่ง GLabel  
  
  cont_date.bg_opa(0);

    roller_day.options(1,31);                       // กำหนด options ให้ roller_day ด้วยตัวเลข ตั้งแต่ 1 ถึง 31
    roller_day.width(50);
    roller_day.onValueChanged(roller_onValueChange);
    roller_day.mask(img_roller_mask);
    roller_day.font(prasanmit_40, prasanmit_20);
    roller_day.radius(0);
    
    roller_month.options(th_month, 12);             // กำหนด options ให้ roller_month ด้วย String Array รายชื่อเดือน
    roller_month.onValueChanged(roller_onValueChange);
    roller_month.mask(img_roller_mask);
    roller_month.font(prasanmit_40, prasanmit_20);
    roller_month.radius(0);

    roller_year.options(2510,2563);                 // กำหนด options ให้ roller_year ด้วยตัวเลข ตั้งแต่ 2510 ถึง 2563
    roller_year.onValueChanged(roller_onValueChange);
    roller_year.mask(img_roller_mask);
    roller_year.font(prasanmit_40, prasanmit_20);
    roller_year.radius(0);

  cont_date.align(lbDate, ALIGN_BOTTOM, 0, 10);

}

void loop(){
  BlynkGO.update();
}

