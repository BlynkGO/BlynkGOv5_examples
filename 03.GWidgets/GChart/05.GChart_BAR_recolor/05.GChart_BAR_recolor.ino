#include <BlynkGOv5.h>

GChart chart;
  chart_series_t * series1;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  chart.size(300, 200);
  chart.type(CHART_TYPE_BAR);                        // แสดงกราฟแบบกราฟแท่ง
  chart.touch_popup(true, [](chart_series_t * series, uint32_t id, int32_t value)->String{
    // return StringX::printf("฿%.2f", value*50/20.0);
    return StringX::printf("฿%d", value);
  });
  chart.point_count(24);
  chart.pad_column(2);
  chart.hookDrawTask(true);
  chart.onDrawTask(GWIDGET_CB {    // เมื่อขณะมีการวาดกราฟ
    if(chart.draw_part() == GPART_ITEMS &&          // ขณะที่มีการวาด ITEMS ของ chart (ITEMS ในที่นี้คือตัวแท่งกราฟแต่ละแท่งๆ)
       chart.draw_type() == DRAW_TASK_TYPE_FILL) {  // โดยเป็นขณะที่กำลังวาดในส่วนเติมสีแท่งกราฟ
      auto fill_dsc = chart.draw_fill_dsc();
      int32_t * y_array = chart.y_array(chart.series_next()); // ค่า pointer ของ y_array ของ series แรกสุด (ในทีนี้มี serie เดียว)
      int32_t value = y_array[chart.draw_id2()];              // ค่า y_array ณ id ที่เป็น id ที่กำลังวาดแท่ง ; value นี้จะมีค่า 0-100
      fill_dsc->color = TFT_COLOR(TFT_COLOR_MIX(TFT_GREEN, TFT_RED, map(value, 0, 100, 0, 255))); // ปรับสีแท่งกราฟ ตาม id ที่ต้อฃการ
    }
  });

  series1 = chart.createSerie(TFT_RED);

  for(int i = 0; i < 24 ; i++) {
    chart.addPoint(series1, random(10, 90));
  }

}

void loop()
{
  BlynkGO.update();
}
