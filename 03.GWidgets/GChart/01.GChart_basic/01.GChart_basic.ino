#include <BlynkGOv5.h>

GChart chart;
  chart_series_t * series1;
  chart_series_t * series2;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  chart.size(200, 150);
  chart.type(CHART_TYPE_LINE);                        // แสดงกราฟแบบเส้น
  // chart.type(CHART_TYPE_LINE | CHART_TYPE_POINT);  // แสดงกราฟแบบเส้น พร้อมจุด
  // chart.type(CHART_TYPE_BAR );                     // แสดงกราฟแบบแท่ง
  // chart.type(CHART_TYPE_SCATTER);                  // แสดงกราฟแบบ scatter
  chart.point_count(50);

    series1 = chart.createSerie(TFT_BLUE);
    series2 = chart.createSerie(TFT_RED);

  static SoftTimer timer;
  timer.setInterval(100, []() {
    chart.addPoint(series1, random(10, 50));
    chart.addPoint(series2, random(50, 90));
  });

}

void loop()
{
  BlynkGO.update();
}
