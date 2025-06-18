#include <BlynkGOv5.h>

GRect rect;
  GLabel label(rect);
  GGaugeRainbow gauge(rect);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE(TFT_PALETTE_LIGHT_BLUE));
  // ออกแบบวิตเจ็ตตามต้องการ
  rect.size(180,180);
  rect.padding(20);
  rect.GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_FR(1)});
  rect.radius(50);
  rect.color(TFT_COLOR_HSV(0,0,10));
    label = "สวัสดีครับ";  label.font(prasanmit_20, TFT_WHITE);
    label.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    gauge.grid_cell(0,1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    gauge.size(120,120);
    gauge.color(TFT_BLUE, TFT_RED, TFT_COLOR_HSV(0,0,90));
    gauge.clickable(false);
    gauge = 89.34;

  rect.onClicked(GWIDGET_CB{
    static GImage mini_capture;
    mini_capture = rect.capture(); // จับเป็นภาพ แล้วใส่วิตเจ็ต GImage
    mini_capture.show(true); mini_capture.scale(128); mini_capture.position(0,0); 
    static SoftTimer timer; 
    timer.delay(3000,[](){mini_capture.show(false);});    // อีก 3 วินาทีให้ซ่อน
  });

  static SoftTimer timer;
  timer.setInterval(5000,[](){
    gauge = random(5000,9700)/100.0;
  });
}

void loop(){
  BlynkGO.update();
}
