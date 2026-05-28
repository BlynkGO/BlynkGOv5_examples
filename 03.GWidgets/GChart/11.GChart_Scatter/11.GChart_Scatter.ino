#include <BlynkGOv5.h>

GChart chart;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  chart.size(240,200);
  chart.line_width(0, GPART_ITEMS);
  chart.type(CHART_TYPE_SCATTER);

  chart.range(CHART_AXIS_PRIMARY_X, 0, 200);
  chart.range(CHART_AXIS_PRIMARY_Y, 0, 1000);

  chart.point_count(50);

  // ขนาดจุด
  chart.point_size(8,8);

  auto ser = chart.add_series(TFT_PALETTE(TFT_PALETTE_RED));
  uint32_t i;
  for(i = 0; i < 50; i++) {
    chart.next_value2(ser, (int32_t)random(0, 200), (int32_t)random(0, 1000));
  }

  // ทำการ hook เพิ่มลูกเล่น ให้ เม็ดสีเดิม จางลง
  chart.hookDrawTask(true);
  chart.onDrawTask(GWIDGET_CB{
    // ขณะกำลังวาด จุด
    if(chart.draw_part() == GPART_INDICATOR) {
      auto ser = chart.series_next(NULL);
      draw_fill_dsc_t* fill_draw_dsc = chart.draw_fill_dsc();
      if(fill_draw_dsc == NULL) return;

      uint32_t cnt = chart.point_count();

      /*Make older value more transparent*/
      fill_draw_dsc->opa = ((255 * chart.draw_id2()) / (cnt - 1));

      /*Make smaller values blue, higher values red*/
      int32_t * x_array = chart.x_array(ser);
      int32_t * y_array = chart.y_array(ser);

      /*dsc->id is the tells drawing order, but we need the ID of the point being drawn.*/
      uint32_t start_point = chart.x_start_point(ser);
      uint32_t p_act = (start_point + chart.draw_id2()) % cnt; /*Consider start point to get the index of the array*/
      opa_t x_opa = ((x_array[p_act] * OPA_50) / 200);
      opa_t y_opa = ((y_array[p_act] * OPA_50) / 1000);
      fill_draw_dsc->color = TFT_COLOR(TFT_COLOR_MIX(TFT_PALETTE(TFT_PALETTE_RED), TFT_PALETTE(TFT_PALETTE_BLUE), x_opa + y_opa));
    }
  });

  static SoftTimer timer_add_data;
  timer_add_data.setInterval(100,[](){
    chart.next_value2( chart.series_next(NULL), (int32_t)random(0, 200), (int32_t)random(0, 1000));
  });
}

void loop(){
  BlynkGO.update();
}
