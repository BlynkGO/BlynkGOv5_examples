#include <BlynkGOv5.h>

GGaugeRainbow gauge;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge.size(100,100);
  // gauge.gauge_type(GAUGE_TYPE_FULL);       // ประเภทเกจจ์ : GAUGE_TYPE_ARC, GAUGE_TYPE_FULL
  gauge.color(TFT_PALETTE(TFT_PALETTE_LIME),TFT_PALETTE_DARKEN(TFT_PALETTE_RED,3));
  // gauge.color_bg(TFT_SILVER);              // สี ของตัว เกจจ์ เมื่อ ไม่มีค่าระดับ
  // gauge.font(prasanmit_35, TFT_WHITE);     // เปลี่ยน font และสีฟอนต์ ของค่าตรงกลางเกจจ์
  // gauge.thickness(20);                     // ความหนาของตัวเกจจ์
  gauge.pad(10);                              // ช่องไฟ บน-ล่าง-ซ้าย-ขวา-ภายใน มีระยะ 10px
  gauge.transform_scale(280, GSTATE_PRESSED); // เมื่อมีการกด ทางกราฟิกให้ขยาย scale เป็นสัดส่วน 280/256 เท่า
  gauge.align(ALIGN_TOP_RIGHT,-20,20);
  
  static SoftTimer timer;
  timer.setInterval(10,[](){
    static int n = 0;  static int step =1;
    n = n+ step*random(200,300);
    if(n > 10000)   { n=10000;  step = -1; }
    else if( n < 0) { n= 0;     step =  1; }
    gauge = n/100.0;
  });
}

void loop(){
  BlynkGO.update();
}
