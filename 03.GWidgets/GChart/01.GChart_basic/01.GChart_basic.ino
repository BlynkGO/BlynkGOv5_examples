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
  // chart.type(CHART_TYPE_LINE);                       // แสดงกราฟแบบเส้น
  // chart.type(CHART_TYPE_LINE | CHART_TYPE_POINT);    // แสดงกราฟแบบเส้น และ จุดด้วย
  // chart.type(CHART_TYPE_BAR);                        // แสดงกราฟแบบแท่ง

  // สำหรับ version ใหม่ หากใช้แบบ curve ให้ เลือก แบบ vector
  // chart.type(CHART_TYPE_CURVE);                      // แสดงกราฟแบบ curve 
  // chart.type(CHART_TYPE_CURVE | CHART_TYPE_POINT);   // แสดงกราฟแบบ curve และ จุดด้วย 
  // chart.type(CHART_TYPE_STACKED);                    // แสดงกราฟแบบ stacked

  chart.point_count(10);                                // จำนวนจุดทั้งหมดของ series กราฟ ให้กำหนดก่อน มีการ createSerie(...)

  // chart.pad_column(8);                               // ระยะช่องไฟระหว่างแท่ง สำหรับกราฟ แบบ BAR / STACKED 
  // chart.point_size(10,10);                           // ขนาดจุด สำหรับ กราฟแบบ LINE / CURVE / SCATTER

  // chart_axis_t : CHART_AXIS_PRIMARY_Y, CHART_AXIS_SECONDARY_Y CHART_AXIS_PRIMARY_X CHART_AXIS_SECONDARY_X
  // chart.range(CHART_AXIS_PRIMARY_Y, 0, 100);         // กำหนด range ของ chart สำหรับแกน y ที่เป็น y หลัก (primary)
  // chart.div_line_count(จำนวนเส้นแบ่งแนวนอน, จำนวนเส้นแบ่งแนวตั้ง);

  series1 = chart.createSerie(TFT_BLUE);                // สร้าง series กราฟ สีน้ำเงิน
  series2 = chart.createSerie(TFT_RED);

  int32_t * series2_y_points = chart.y_array(series2);  // ค่า y points ของ series ที่ต้องการ

  // ใส่ค่าเริ่มต้นให้ series กราฟ ตามจำนวน point ของแต่ละ series กราฟ ที่จองไว้
  uint32_t i;
  for(i = 0; i < chart.point_count(); i++) {
      chart.addPoint(series1, random(10, 50));
      series2_y_points[i] = random(50, 90);      /* กำหนดค่าแบบ Direct สู่ points ให้ series2 */
  }
  chart.refresh();  /* ต้องใส่ สำหรับ แบบกำหนดค่าให้ series แบบ Direct  */

  // ใส่ค่าแบบ dynamic เพิ่มค่า ทีละ 1 วินาที ให้ series
  static SoftTimer timer;
  timer.setInterval(1000, []() {
    chart.addPoint(series1, random(10, 50));
  });
}

void loop()
{
  BlynkGO.update();
}
