#include <BlynkGOv5.h>

GContainer cont_chart(LAYOUT_COL);
  GChart chart(cont_chart);
    chart_series_t * series1;
    chart_series_t * series2;
  GScale scale_b(cont_chart);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  cont_chart.size(PCT(80), PCT(60));
    chart.width(PCT(100));
    chart.flex_grow(1);
    chart.type(CHART_TYPE_BAR);
    chart.range(CHART_AXIS_PRIMARY_Y, 0, 100);
    chart.range(CHART_AXIS_SECONDARY_Y, 0, 400);
    chart.point_count(12);

    scale_b.mode(SCALE_MODE_HOR_BOTTOM);
    scale_b.size(PCT(100), 25);
    scale_b.tick(12, 1);
    scale_b.thickness(0);
    scale_b.pad_hor(chart.first_point_center_offset());

    static const char * month[] = {"ม.ค.", "ก.พ.", "มี.ค.", "เม.ย.", "พ.ค.", "มิ.ย.", "ก.ค.", "ส.ค.", "ก.ย.", "ต.ค.", "พ.ย.", "ธ.ค.", NULL};
    scale_b.text_src(month);

    series1 = chart.createSerie(TFT_BLUE);
    series2 = chart.createSerie(TFT_RED);

    for(int i = 0; i < 10; i++) {
      chart.addPoint(series1, random(10, 50));
      series2->y_points[i] = random(50, 90);
    }

}

void loop()
{
  BlynkGO.update();
}
