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

  // กำหนดค่าแบบ direct input ใส่โดยตรง;  
  // หากตำแหน่งไหน ไม่ต้องการให้มีข้อมูล ให้ใส่ CHART_POINT_NONE
  // เช่น chart.y_array(series1)[5] = CHART_POINT_NONE;
  //  ==> ค่า y ตำแหน่งที่ 5 ของ series1 ไม่มีข้อมูล กราฟ series1 จะข้ามไม่มีการวาดจุดนี้ไป
  for(int i = 0; i < chart.point_count() ; i++) {
    chart.y_array(series1)[i] = random(10, 50);   
    chart.y_array(series2)[i] = random(50, 90);
    // หรือ ใช้แบบนี้ก็ได้
    // series1->y_points[i] = random(10, 50);
    // series2->y_points[i] = random(50, 90);
  }

  // *** สำคัญ *** ให้มีคำสั่งนี้ ด้วยหลัง มีการ ใส่ค่าแบบ direct input ให้ series แล้ว
  chart.refresh();
}

void loop()
{
  BlynkGO.update();
}
