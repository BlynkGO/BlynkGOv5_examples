#include <BlynkGOv5.h>

GChartRangeBar chart_rangebar;
  chart_rangebar_series_t * series;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  chart_rangebar.size(300,200);
  chart_rangebar.range(40, 160);
  chart_rangebar.point_count(12);       // จำนวนแท่ง rangebar ในหน้า chart_rangebar
  chart_rangebar.div_line_count(5, 8);  // ตีตารางเส้น แนวนอน 5 เส้น แนวตั้ง 8 เส้น
  chart_rangebar.pad_column(15);        // ระยะห่างระหว่าง แท่ง bar

  series = chart_rangebar.createSerie(TFT_PALETTE(TFT_PALETTE_BLUE));

  static SoftTimer timer;
  timer.setInterval(500,[](){
    int32_t val_min = random(60,85);
    int32_t val_max = random(110,130);
    chart_rangebar.addBar(series, val_min, val_max );
  });
}

void loop(){
  BlynkGO.update();
}

