#include <BlynkGOv5.h>

GRect mywidget;
  GLabel label(mywidget);
  GGaugeRainbow gauge(mywidget);

GLabel lb_capture_status;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE(TFT_PALETTE_LIGHT_BLUE));
  // ออกแบบวิตเจ็ตตามต้องการ
  mywidget.size(180,180);
  mywidget.padding(20);
  mywidget.GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_FR(1)});
  mywidget.radius(50);
  mywidget.color(TFT_COLOR_HSV(0,0,10));
    label = "สวัสดีครับ";  label.font(prasanmit_20, TFT_WHITE);
    label.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    gauge.grid_cell(0,1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    gauge.size(120,120);
    gauge.color(TFT_BLUE, TFT_RED, TFT_COLOR_HSV(0,0,90));
    gauge.clickable(false);
    gauge = 89.34;

  lb_capture_status.align(ALIGN_BOTTOM_RIGHT);
  lb_capture_status.font(prasanmit_20, TFT_BLACK);
  lb_capture_status.show(false);

  mywidget.onClicked(GWIDGET_CB{
    // นามสกุลที่แปลงได้คือ .bmp  .jpg  .png ( สำหรับ png ใช้ได้สำหรับจอ BeeNeXT รุ่นที่มี PSRAM เท่านั้น)
    bool ret = mywidget.capture("mywidget.bmp");

    // bool ret = mywidget.capture("SD:://mywidget.bmp");       // สำหรับบังคับ save ลง SD
    // bool ret = mywidget.capture("SPIFFS:://mywidget.bmp");   // สำหรับบังคับ save ลง SPIFFS
    // bool ret = mywidget.capture("LITTLEFS:://mywidget.bmp"); // สำหรับบังคับ save ลง LITTLEFS

    lb_capture_status.show(true);
    lb_capture_status = ( ret )? "capture saved!" : "capture failed!";
    static SoftTimer timer; 
    timer.delay(3000,[](){lb_capture_status.show(false);});    // อีก 3 วินาทีให้ซ่อน
  });

  static SoftTimer timer;
  timer.setInterval(5000,[](){
    gauge = random(5000,9700)/100.0;
  });
}

void loop(){
  BlynkGO.update();
}
