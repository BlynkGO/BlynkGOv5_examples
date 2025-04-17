#include <BlynkGOv5.h>

GContainer cont_ddlist(LAYOUT_PRETTY,5,5,5,5,20,10);
  GDropdownList ddlist_day    (cont_ddlist);
  GDropdownList ddlist_month  (cont_ddlist);
  GDropdownList ddlist_year   (cont_ddlist);

  GDropdownList ddlist_hour   (cont_ddlist);
  GDropdownList ddlist_minute (cont_ddlist);

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

void ddlist_date_onValueChange(GWIDGET);
void ddlist_clock_onValueChange(GWIDGET);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_ddlist.width(PCT(100));
  cont_ddlist.bg_opa(0);
  cont_ddlist.align(ALIGN_TOP,0,15);

    ddlist_day    .options(1,31);           // กำหนด options ให้ ddlist_day ด้วยตัวเลข ตั้งแต่ 1 ถึง 31
    ddlist_month  .options(th_month, 12);   // กำหนด options ให้ ddlist_month ด้วย String Array
    ddlist_year   .options(2510,2563);      // กำหนด options ให้ ddlist_year ด้วยตัวเลข ตั้งแต่ 2510 ถึง 2563

    ddlist_day    .symbol(NULL);
    ddlist_month  .symbol(NULL);
    ddlist_year   .symbol(NULL);
    
    ddlist_day    .onValueChanged(ddlist_date_onValueChange);    // เมื่อ user มีการกดเลือกแล้วค่าเปลี่ยนแปลงเข้ามา
    ddlist_month  .onValueChanged(ddlist_date_onValueChange);
    ddlist_year   .onValueChanged(ddlist_date_onValueChange);

    ddlist_day    .size(60 , 50);
    ddlist_month  .size(140, 50);
    ddlist_year   .size(90 , 50);

    ddlist_day    .font(prasanmit_25);
    ddlist_month  .font(prasanmit_25);
    ddlist_year   .font(prasanmit_25);

    ddlist_hour   .options(0,23,true);
    ddlist_minute .options(0,59, true);

    ddlist_hour   .symbol(NULL);
    ddlist_minute .symbol(NULL);

    ddlist_hour   .size(60,50);
    ddlist_minute .size(60,50);

    ddlist_hour   .font(prasanmit_25);
    ddlist_minute .font(prasanmit_25);

    ddlist_hour   .onValueChanged(ddlist_clock_onValueChange);
    ddlist_minute .onValueChanged(ddlist_clock_onValueChange);

}

void loop() {
  BlynkGO.update();
}

void ddlist_date_onValueChange(GWIDGET){
  String date_selected = ddlist_day.selected() +  " " +  ddlist_month.selected() + " " + ddlist_year.selected();
  Serial.println( date_selected );

  int day   = ddlist_day  .toInt();           // ดึงค่า ตัวเลือก ปัจจุบันของ ddist ออกมาและแปลงเป็น int ให้
  int year  = ddlist_year .toInt();           // ดึงค่า ตัวเลือก ปัจจุบันของ ddist ออกมาและแปลงเป็น int ให้
  int month = ddlist_month.selected_id() + 1; // ดึงค่า ลำดับ ของ ตัวเลือกปัจจุบัน  (ค่าจะเป็น 0 หรือ 1, 2, 3... ) เมื่อ +1 คือเลขเดือนปัจจุบันที่เลือกอยู่
  Serial.printf("%02d/%02d/%04d\n", day, month, year);
}

void ddlist_clock_onValueChange(GWIDGET){
  String clock_selected = ddlist_hour.selected() +  ":" +  ddlist_minute.selected();
  Serial.println( clock_selected );
}

