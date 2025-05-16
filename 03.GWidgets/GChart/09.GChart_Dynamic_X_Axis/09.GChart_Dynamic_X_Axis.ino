#include <BlynkGOv5.h>

#define CHART_SERIE_COLOR     TFT_COLOR_HEX(0x3a5fa6)
#define CHART_Y_MAX           100
#define CHART_Y_MIN           0

#define CHART_BLOCK_X_COUNT   7
#define CHART_BLOCK_Y_COUNT   5
#define CHART_BLOCK_WIDTH     80    // px
#define CHART_BLOCK_HEIGHT    60    // px

GChart chart_frame;       // สำหรับตีเส้น chart
GChart chart;             // สำหรับเป็น chart จริง
  chart_series_t *series; // series สำหรับเก็บข้อมูล
GScale scale_x_chart;     // แกน x
GScale scale_y_chart;     // แกน y
SoftTimer timer_series_update;

void chart_add_temp(String x_str, int32_t y_temp) {
  static std::vector<String> axis_x_txt_list;
  if( axis_x_txt_list.size() == 0){
    axis_x_txt_list.resize(CHART_BLOCK_X_COUNT, "");  // เพิ่ม "" เปล่าๆ ไปจำนวน CHART_BLOCK_X_COUNT-1
  }
  if( axis_x_txt_list.size() == CHART_BLOCK_X_COUNT) {
    axis_x_txt_list.erase(axis_x_txt_list.begin());   // เอาตัวแรกออกจาก vector
  }  
  axis_x_txt_list.push_back(x_str);
  scale_x_chart.label_show(true);
  scale_x_chart.text_src(axis_x_txt_list);
  chart.addPoint(series, y_temp);
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  chart_frame.size( CHART_BLOCK_X_COUNT * CHART_BLOCK_WIDTH, CHART_BLOCK_Y_COUNT * CHART_BLOCK_HEIGHT);
  chart_frame.div_line_count(CHART_BLOCK_Y_COUNT+1,CHART_BLOCK_X_COUNT+1);
  chart_frame.bg_opa(0);
  chart_frame.line_color(TFT_COLOR_HEX(0x202020));
  chart_frame.line_width(1);
  chart_frame.border_color(TFT_COLOR_HEX(0x202020));
  chart_frame.scrollable(false);
  chart_frame.hookDrawTask(true);
  chart_frame.onDrawTask(GWIDGET_CB{
    if( chart_frame.draw_part() == GPART_MAIN &&            // ขณะ กำลังวาดระดับล่าง ในส่วน main ของ chart
        chart_frame.draw_type() == DRAW_TASK_TYPE_LINE )    // ขณะ กำลังวาดระดับล่าง ในส่วนตี line ของ chart
    {
      auto line_dsc = chart_frame.draw_line_dsc();          // รายละเอียด line ที่กำลังจะวาดระดับล่าง
      if( line_dsc->p1.x == chart_frame.coords()->x1 &&     // แสดงว่า กำลังวาดเส้นแนวนอน
          line_dsc->p2.x == chart_frame.coords()->x2 )
      {
        line_dsc->dash_width = line_dsc->dash_gap = 4;      // ทำเป็นแนวนอนให้เป็นเส้นประ
      }
    }
  });

    chart.size(chart_frame);
    chart.pad_hor(CHART_BLOCK_WIDTH/2);
    chart.range(CHART_Y_MIN, CHART_Y_MAX);
    chart.point_count(CHART_BLOCK_X_COUNT);
    chart.bg_opa(0);
    chart.line_width(0);
    chart.border(0);
    chart.draw_grad(true);
    chart.touch_popup(true, [](chart_series_t* series, uint32_t id, int32_t value)->String{
      return StringX::printf("%d" SYMBOL_DEGREE "C", value);   // ค่าที่สัมผัส จะอยู่ระหว่าง 0-100 มาจัด format เองได้
    });
    chart.chart_type(CHART_TYPE_LINE);

    // สีพื้นหลัง touch popup
    chart.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,2), GPART_SELECTED);
    chart.onPressing(GWIDGET_CB{
      timer_series_update.pause();
    });
    chart.onReleased(GWIDGET_CB{
      static SoftTimer t;
      t.delay(1000,[](){
        timer_series_update.resume();
      });
    });

    // จุดบน serie
    chart.size(8, 8, GPART_INDICATOR);
    // chart.style_width(8, GPART_INDICATOR);
    // chart.style_height(8, GPART_INDICATOR);
    chart.color(TFT_BLACK, GPART_INDICATOR);
    chart.border(2, CHART_SERIE_COLOR, GPART_INDICATOR);

    scale_y_chart.height(chart_frame);
    scale_y_chart.mode(SCALE_MODE_VER_LEFT, CHART_BLOCK_Y_COUNT+1);
    scale_y_chart.align(chart_frame, ALIGN_LEFT, -5);
    scale_y_chart.range(CHART_Y_MIN, CHART_Y_MAX);
    scale_y_chart.thickness(0);
    scale_y_chart.font_color(TFT_WHITE);
  
    scale_x_chart.width((CHART_BLOCK_X_COUNT-1)*CHART_BLOCK_WIDTH);
    scale_x_chart.mode(SCALE_MODE_HOR_BOTTOM, CHART_BLOCK_X_COUNT);
    scale_x_chart.align(chart, ALIGN_BOTTOM, 0, 5);
    scale_x_chart.thickness(0);
    scale_x_chart.font_color(TFT_WHITE);
    scale_x_chart.label_show(false);

    series = chart.createSerie(CHART_SERIE_COLOR);
    timer_series_update.setInterval(1000,[](){
      static int d = 0; d = (d+1)%32; if(d == 0) d = 1;
      String date = StringX::printf("%02d พ.ค.", d);
      chart_add_temp(date.c_str(), random(CHART_Y_MIN, CHART_Y_MAX));
    });
}

void loop(){
  BlynkGO.update();
}