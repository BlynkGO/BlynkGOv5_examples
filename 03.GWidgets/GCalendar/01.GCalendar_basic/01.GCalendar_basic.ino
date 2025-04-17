#include <BlynkGOv5.h>

GCalendar calendar;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  const char * days[7] = {"อา", "จ", "อ", "พ", "พฤ", "ศ", "ส"};
  calendar.day_names(days);
  calendar.today_date(2024, 9, 22);
  calendar.showed_date(2024, 9);

  calendar.onValueChanged(GWIDGET_CB{
    calendar_date_t date;
    if(calendar.pressed_date(&date)){
      Serial.printf("[GCalendar] %02d/%02d/%04d\n", date.day, date.month, date.year);
    }
  });

  /*Highlight a few days*/
  static calendar_date_t highlighted_days[3];       /*Only its pointer will be saved so should be static*/
  highlighted_days[0].year  = 2024;
  highlighted_days[0].month = 9;
  highlighted_days[0].day   = 6;

  highlighted_days[1].year  = 2024;
  highlighted_days[1].month = 9;
  highlighted_days[1].day   = 11;

  highlighted_days[2].year  = 2023;
  highlighted_days[2].month = 12;
  highlighted_days[2].day   = 22;

  calendar.highlighted_dates( highlighted_days, 3);

  /* ให้ มี calendar header ด้วย
     โดย เป็นแบบใดแบบหนึ่งระหว่าง   header_dropdown_create() หรือ header_arrow_create() */

  // calendar.header_dropdown_create();   // ให้มี header แบบ dropdown       สำหรับเลือกแสดง ปี และ เดือน ที่ต้องการ
  calendar.header_arrow_create();         // ให้มี header แบบ ลูกศรเลื่อนซ้ายขวา สำหรับเลือกแสดง ปี และ เดือน ที่ต้องการ
}

void loop() {
  BlynkGO.update();
}
