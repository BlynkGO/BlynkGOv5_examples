#include <BlynkGOv5.h>

GTextArea ta_date;
GCalendar calendar;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);


  ta_date.align(ALIGN_TOP,-50,10);
  ta_date.default_keyboard(false);
  ta_date.width(120);
  ta_date.font(prasanmit_20);
  ta_date.cursor_show(false);
  ta_date.onClicked(GWIDGET_CB{
    calendar.today_date(year(), month(), day());
    calendar.show(!calendar.show());
    calendar.toForeground();
  });

  ta_date = StringX::printf("%02d/%02d/%04d", day(), month(), year());

  const char * days[7] = {"อา", "จ", "อ", "พ", "พฤ", "ศ", "ส"};
  calendar.show(false);
  // calendar.width(ta_date);
  calendar.align(ta_date, ALIGN_BOTTOM_LEFT,0,3);
  calendar.day_names(days);
  calendar.showed_date(year(), month());

  calendar.onClicked(GWIDGET_CB{
    calendar.show(false);
  });

  calendar.onValueChanged(GWIDGET_CB{
    calendar_date_t date;
    if(calendar.pressed_date(&date)){
      Serial.printf("[GCalendar] %02d/%02d/%04d\n", date.day, date.month, date.year);
      ta_date = StringX::printf("%02d/%02d/%04d", date.day, date.month, date.year);
    }
    calendar.show(false);
  });

  // ออกแบบ
  calendar.items_pressed.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_PURPLE,2));

  calendar.header_arrow_create();         // ให้มี header แบบ ลูกศรเลื่อนซ้ายขวา สำหรับเลือกแสดง ปี และ เดือน ที่ต้องการ
}

void loop() {
  BlynkGO.update();
}
