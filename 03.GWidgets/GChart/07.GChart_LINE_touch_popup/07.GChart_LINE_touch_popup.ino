#include <BlynkGOv5.h>

GChart chart;
  chart_series_t * series1;

GScale scale_x_chart;
GScale scale_y_chart;


void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  chart.size(400, 200);
  chart.type(CHART_TYPE_LINE);                        // แสดงกราฟแบบกราฟเส้น
  chart.touch_popup(true, [](chart_series_t * series, uint32_t id, int32_t value)->String{
    return StringX::printf("฿%d", value);
  });
  chart.point_count(12);                            // จำนวนจุดที่ plot ในหน้า char
  chart.pad_left(10);                               // ช่องไฟด้านซ้าย
  chart.pad_right(10);                              // ช่องไฟด้านขวา
  chart.pad_column(20);                             // ช่องไฟระหว่างกราฟแท่ง
  chart.div_line_count(6,0);                        // เส้นแบ่งช่วง เส้นนอน 6 เส้น และ เส้นตั้ง 5 เส้น
  chart.hookDrawTask(true);
  chart.onDrawTask(GWIDGET_CB {                    // เมื่อขณะมีการวาดกราฟ
    if(chart.draw_part() == GPART_ITEMS &&          // ขณะที่มีการวาด ITEMS ของ chart (ITEMS ในที่นี้คือตัวแท่งกราฟแต่ละแท่งๆ)
       chart.draw_type() == DRAW_TASK_TYPE_FILL) {  // โดยเป็นขณะที่กำลังวาดในส่วนเติมสีแท่งกราฟ
      auto fill_dsc = chart.draw_fill_dsc();
      int32_t * y_array = chart.y_array(chart.series_next()); // ค่า pointer ของ y_array ของ series แรกสุด (ในทีนี้มี serie เดียว)
      int32_t value = y_array[chart.draw_id2()];              // ค่า y_array ณ id ที่เป็น id ที่กำลังวาดแท่ง ; value นี้จะมีค่า 0-10000 (ตาม range แกน y)
      fill_dsc->color = TFT_COLOR(TFT_COLOR_HSV( map(value, 0, 10000, 0, 240), 100, 100)); // ปรับสีแท่งกราฟ ตาม value ที่ต้อฃการ
    }
  });

  series1 = chart.createSerie(TFT_RED);

  // คำนวณความกว้างแท่ง bar
  int32_t chart_bar_width = (chart.width() - (chart.point_count()-1) * chart.pad_column() )/  chart.point_count();

  scale_x_chart.size(chart.width() - chart_bar_width - chart.pad_left() - chart.pad_right(), 50);
  scale_x_chart.align(chart, ALIGN_BOTTOM);
  scale_x_chart.axis_thickness(0);
  scale_x_chart.tick_length(10);
  scale_x_chart.mode(SCALE_MODE_BOTTOM, 12);
  scale_x_chart.range(1,12);
  scale_x_chart.label_show(true);
  static const char * month_labels[] = {"ม.ค.", "ก.พ.", "มี.ค.", "เม.ย.", "พ.ค.", "มิ.ย.", "ก.ค.", "ส.ค.", "ก.ย.", "ต.ค.", "พ.ย.", "ธ.ค.",  NULL};
  scale_x_chart.text_src(month_labels);

  scale_y_chart.size(60, chart.height());
  scale_y_chart.align(chart, ALIGN_LEFT);
  scale_y_chart.axis_thickness(0);
  scale_y_chart.tick_length(10);
  scale_y_chart.mode(SCALE_MODE_LEFT, 5+1, 1);
  scale_y_chart.range(0,10000);
  scale_y_chart.label_show(true);

  chart.range(CHART_AXIS_PRIMARY_Y, 0, 10000);    // แกน y หลัก มีช่วง 0-10000
  for(int i = 0; i < 12 ; i++) {        
    int price = random(400, 9990);                // จำลองสุ่มเลขระหว่าง 400-9990
    chart.addPoint(series1, price);
  }

}

void loop()
{
  BlynkGO.update();
}
