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

  chart.point_count(10);

    series1 = chart.createSerie(TFT_BLUE);
    series2 = chart.createSerie(TFT_RED);

  for(int i = 0; i < 10 ; i++) {
    series1->y_points[i] = random(10, 50); // กำหนดค่าแบบใส่โดยตรง
    series2->y_points[i] = random(50, 90);
  }
}

void loop()
{
  BlynkGO.update();
}
