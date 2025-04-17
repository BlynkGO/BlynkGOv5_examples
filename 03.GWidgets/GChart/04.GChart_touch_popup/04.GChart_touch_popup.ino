#include <BlynkGOv5.h>

GContainer cont_chart(LAYOUT_COL);
  GChart chart(cont_chart);
    chart_series_t * series1;
    chart_series_t * series2;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  chart.size(300, 200);
  chart.type(CHART_TYPE_LINE);
  chart.touch_popup(true, [](chart_series_t* series, uint32_t id, int32_t value)->String{
    // return StringX::printf("฿%.2f", value*50/20.0);
    return StringX::printf("฿%d", value);   // ค่าที่สัมผัส จะอยู่ระหว่าง 0-100 มาจัด format เองได้
  });
  chart.point_count(10);

    series1 = chart.createSerie(TFT_BLUE);
    series2 = chart.createSerie(TFT_RED);

  for(int i = 0; i < 10 ; i++) {
    chart.addPoint(series1, random(10, 50));    // ค่าใหม่เพิ่มต่อท้าย serie1 โดยต้องเปลี่ยนเป็นค่า 0-100
    chart.addPoint(series2, random(50, 90));
  }
}

void loop()
{
  BlynkGO.update();
}
