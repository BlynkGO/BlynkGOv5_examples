#include <BlynkGOv5.h>

#define CHART_POINTS_NUM 256

GContainer cont(
  GRID_CELL_COL {GRID_FR(1), 200, GRID_FR(1)},
  GRID_CELL_ROW {30, 15, 15, GRID_FR(1)}
);
  GRect anim_obj(cont);
  GLabel lb_p1(cont);
  GLabel lb_p2(cont);
  GSlider slider_p1(cont);
  GSlider slider_p2(cont);
  GButton btn_run(cont);
  GChart  chart(cont);
    chart_series_t * ser;

void G_refer_chart_cubic_bezier()
{
  for(uint16_t i = 0; i <= CHART_POINTS_NUM; i ++) {
    uint32_t t = i * (1024 / CHART_POINTS_NUM);
    int32_t step = lv_bezier3(t, 0, slider_p1.toInt(), slider_p2.toInt(), 1024);
    chart.value_by_id2(ser, i, t, step);
  }
  chart.refresh();
}

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin(); BlynkGO.fillScreen(TFT_BLACK);

  cont.size(320, 240);
  cont.padding(10);

  anim_obj.size(20, 20);
  anim_obj.color(TFT_PALETTE(TFT_PALETTE_RED));
  anim_obj.grid_cell(0,0);

  lb_p1 = "p1:0";
  lb_p1.font(prasanmit_15, TFT_BLACK);
  lb_p1.grid_cell(0,1);
  lb_p1.grid_cell_align(GRID_ALIGN_START, GRID_ALIGN_CENTER);

  lb_p2 = "p2:0";
  lb_p2.font(prasanmit_15, TFT_BLACK);
  lb_p2.grid_cell(0,2);
  lb_p2.grid_cell_align(GRID_ALIGN_START, GRID_ALIGN_CENTER);

  slider_p1.range(0, 1024);
  slider_p1.width(190);
  slider_p1.grid_cell(1,1);

  slider_p2.range(0, 1024);
  slider_p2.width(190);
  slider_p2.grid_cell(1,2);

  slider_p1.onValueChanged(GWIDGET_CB {
    lb_p1 = StringX::printf("p1:%d", slider_p1.toInt());
    G_refer_chart_cubic_bezier();
  });
  slider_p2.onValueChanged(GWIDGET_CB {
    lb_p2 = StringX::printf("p2:%d", slider_p2.toInt());
    G_refer_chart_cubic_bezier();
  });

  btn_run = SYMBOL_PLAY;
  btn_run.grid_cell(2, 1);
  btn_run.grid_cell_span(1, 2);
  btn_run.grid_cell_align(GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  btn_run.onClicked(GWIDGET_CB{
    static GAnimation anim;
    int32_t end = cont.content_width() - anim_obj.width();
    anim.init(anim_obj, [](GWIDGET, int32_t value){
      anim_obj.translate_x(value);
    }, 0, end, 2000);

    anim.path_cb([](int32_t cur_time, int32_t total_time, int32_t start_value, int32_t end_value)->int32_t
    {
      uint32_t t = map(cur_time, 0, total_time, 0, 1024);
      int32_t step = lv_bezier3(t, 0, slider_p1.toInt(), slider_p2.toInt(), 1024);
      int32_t new_value;
      new_value = step * (end_value - start_value);
      new_value = new_value >> 10; // คือ new_value/2^10
      new_value += start_value;
      return new_value;
    });
    anim.start();
  });


  chart.type(CHART_TYPE_SCATTER);
  chart.range(CHART_AXIS_PRIMARY_X, 0, 1024);
  chart.range(CHART_AXIS_PRIMARY_Y, 0, 1024);
  chart.point_count(CHART_POINTS_NUM);
  chart.grid_cell(0, 3);
  chart.grid_cell_span(3, 1);

  ser = chart.createSerie(TFT_PALETTE(TFT_PALETTE_RED));

  G_refer_chart_cubic_bezier();

}

void loop()
{
  BlynkGO.update();
}

