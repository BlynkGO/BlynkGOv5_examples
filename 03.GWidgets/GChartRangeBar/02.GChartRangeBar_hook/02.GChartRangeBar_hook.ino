#include <BlynkGOv5.h>

GChartRangeBar chart_rangebar;
  chart_rangebar_series_t * series;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  chart_rangebar.size(480,240);
  chart_rangebar.bg_opa(255);
  chart_rangebar.range(40, 160);
  chart_rangebar.point_count(18);       // จำนวนแท่ง rangebar ในหน้า chart_rangebar
  chart_rangebar.div_line_count(7, 1);  // ตีตารางเส้น แนวนอน 7 เส้น แนวตั้ง 1 เส้น
  chart_rangebar.pad_column(15);        // ระยะห่างระหว่าง แท่ง bar
  chart_rangebar.line_dash_width(1);
  chart_rangebar.line_dash_gap(2);
  chart_rangebar.line_opa(40);

  // ทำการ hook การวาดกราฟิกขณะกำลังวาดระดับล่าง
  chart_rangebar.hookDrawTask(true);                            // เปิดให้สามารถดักการวาดกราฟิกระดับล่างได้ด้วย
  chart_rangebar.onDrawTask(GWIDGET_CB{                        // เมื่อมีการวาดกราฟิกระดับล่าง
    if( chart_rangebar.draw_part() == GPART_MAIN &&             // ขณะ กราฟิกระดับล่าง กำลังวาด ในส่วน main ของ chart
        chart_rangebar.draw_type() == DRAW_TASK_TYPE_LINE )     // ขณะ กราฟิกระดับล่าง กำลังวาด ตีเส้น line ของ chart
    {
      auto line_dsc = chart_rangebar.draw_line_dsc();           // ดึงค่ารายละเอียดของ line ที่กำลังจะใช้วาดระดับล่าง
      if(line_dsc->p1.x == chart_rangebar.coords()->x1 &&       // หาก กำลังวาดเส้นแนวนอน (เนื่องจากไปเท่าระยะต้นและปลายของแนวนอน)
         line_dsc->p2.x == chart_rangebar.coords()->x2 )
      {
        // หากกำลังวาดกราฟิกระดับล่าง กำลังตีเส้นแนวนอน โดยนับจากเส้นที่ อยู่ตรงกลางแนวนอน
        if( line_dsc->base.id1 == 2 || line_dsc->base.id1 == 4) {
          line_dsc->color       = TFT_COLOR(TFT_PALETTE(TFT_RED));
          line_dsc->opa         = 100;
          line_dsc->dash_width  = 0;
          line_dsc->dash_gap    = 0;
        }
      }
    }
  });

  series = chart_rangebar.createSerie(TFT_PALETTE(TFT_PALETTE_BLUE));
  chart_rangebar.radius(5, GPART_ITEMS);      // มุมโค้งของแท่ง rangebar

  static SoftTimer timer;
  timer.setInterval(500,[](){
    int32_t val_min = random(60,85);
    int32_t val_max = random(110,130);
    chart_rangebar.addBar(series, val_min, val_max );
  });
}

void loop(){
  BlynkGO.update();
}

