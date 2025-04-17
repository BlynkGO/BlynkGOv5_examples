#include <BlynkGOv5.h>

GContainer cont_date_time;
  GContainer cont_date(cont_date_time);
    GRoller roller_day(cont_date);
    GRoller roller_month(cont_date);
    GRoller roller_year(cont_date);
  GContainer cont_time(cont_date_time);
    GRoller roller_hour(cont_time);
    GRoller roller_minute(cont_time);
    GRoller roller_second(cont_time);

GLabel lb_timestamp;

void roller_onValueChanged(GWIDGET){
  struct tm timeinfo = {0};  // กำหนดค่าตั้งต้นเป็น 0

  // ดึงเวลาจาก roller
  timeinfo.tm_year = roller_year.toInt() - 1900;    // ปีตั้งต้นที่ 1900, 2023 - 1900 = 123
  timeinfo.tm_mon  = roller_month.toInt() - 1;      // เดือน: มกราคมเป็นเดือนที่ 0
  timeinfo.tm_mday = roller_day.toInt();            // วันในเดือน
  timeinfo.tm_hour = roller_hour.toInt();           // ชั่วโมง
  timeinfo.tm_min  = roller_minute.toInt();         // นาที
  timeinfo.tm_sec  = roller_second.toInt();         // วินาที

  // แปลงเป็น timestamp
  time_t ts = mktime(&timeinfo);

  // จัดรูปแบบวันที่และเวลา (ไม่รวมมิลลิวินาที)
  char buffer[30];
  strftime(buffer, 30, "%Y-%m-%dT%H:%M:%S", &timeinfo);

  String timestamp_ISO8601 = String(buffer)+ String(".000Z");
  lb_timestamp = timestamp_ISO8601;
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_date_time.bg_opa(0);
  cont_date_time.layout(LAYOUT_ROW_M, 0,0,0,0, 20);
    cont_date.bg_opa(0);
    cont_date.layout(LAYOUT_ROW_M, 10, 10, 10, 10, 10);
    cont_date.border(1, TFT_SILVER);
      roller_day    .options(1, 31, true);
      roller_month  .options(1, 12);
      roller_year   .options(1970,2030);
      roller_year   .selected(2024-1970);
    cont_time.bg_opa(0);
    cont_time.layout(LAYOUT_ROW_M, 10, 10, 10, 10, 10);
    cont_time.border(1, TFT_SILVER);
      roller_hour   .options(0,23, true);
      roller_minute .options(0,59, true);
      roller_second .options(0,59, true);

      roller_day    .onValueChanged(roller_onValueChanged);
      roller_month  .onValueChanged(roller_onValueChanged);
      roller_year   .onValueChanged(roller_onValueChanged);
      roller_hour   .onValueChanged(roller_onValueChanged);
      roller_minute .onValueChanged(roller_onValueChanged);
      roller_second .onValueChanged(roller_onValueChanged);

  lb_timestamp.align(cont_date_time, ALIGN_BOTTOM, 0, 30);

}

void loop(){
  BlynkGO.update();
}
