#include <BlynkGOv5.h>

GContainer cont_frame;
  GLabel lb_title(cont_frame);
  GLabel lb_date(cont_frame);
  GChartRangeBar chart_rangebar_bp(cont_frame);
    chart_rangebar_series_t * series_bp;

  GChart chart_line_hr(cont_frame);
    chart_series_t * series_hr;

  GScale scale_chart_y(cont_frame);
  GScale scale_chart_x(cont_frame);
  GLabel lb_monthname(cont_frame);
  GLabel lb_unit_mmHg(cont_frame);

const char* today_date = "มิ.ย.10";

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_frame.size(580, SIZE_CONTENT);
  cont_frame.bg_opa(0);
  cont_frame.GRID_CELL( GRID_COL{ 60, GRID_FR(1), 30}, GRID_ROW{GRID_CONTENT, 40, 280, 50});
  cont_frame.padding(0,20,0,0,0);
  cont_frame.font_color(TFT_WHITE);
    chart_rangebar_bp.grid_cell(1,2, GRID_ALIGN_STRETCH, GRID_ALIGN_END);
    chart_rangebar_bp.bg_opa(0);
    chart_rangebar_bp.range(40, 160);
    chart_rangebar_bp.point_count(24);                              // จำนวนแท่ง rangebar ในหน้า chart_rangebar
    chart_rangebar_bp.div_line_count(7, 1);                         // ตีตารางเส้น แนวนอน 7 เส้น แนวตั้ง 1 เส้น
    chart_rangebar_bp.pad_column(15);                               // ระยะห่างระหว่าง แท่ง bar
    chart_rangebar_bp.line_dash_width(1);
    chart_rangebar_bp.line_dash_gap(2);
    chart_rangebar_bp.bg_opa(OPA_100, GPART_SELECTED);              // GPART_SELECTED สำหรับ PART ส่วน touch popup area
    chart_rangebar_bp.bg_color(TFT_WHITE, GPART_SELECTED);

    // ทำการ hook การวาดกราฟิกขณะกำลังวาดระดับล่าง
    chart_rangebar_bp.hookDrawTask(true);                           // เปิดให้สามารถดักการวาดกราฟิกระดับล่างได้ด้วย
    chart_rangebar_bp.onDrawTask(GWIDGET_CB{                        // เมื่อมีการวาดกราฟิกระดับล่าง
      if( chart_rangebar_bp.draw_part() == GPART_MAIN &&            // ขณะ กราฟิกระดับล่าง กำลังวาด ในส่วน main ของ chart
          chart_rangebar_bp.draw_type() == DRAW_TASK_TYPE_LINE )    // ขณะ กราฟิกระดับล่าง กำลังวาด ตีเส้น line ของ chart
      {
        auto line_dsc = chart_rangebar_bp.draw_line_dsc();          // ดึงค่ารายละเอียดของ line ที่กำลังจะใช้วาดระดับล่าง
        if(line_dsc->p1.x == chart_rangebar_bp.coords()->x1 &&      // หาก กำลังวาดเส้นแนวนอน (เนื่องจากไปเท่าระยะต้นและปลายของแนวนอน)
          line_dsc->p2.x == chart_rangebar_bp.coords()->x2 )
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
    chart_rangebar_bp.event_cb([](GWIDGET, event_t event){
      GChartRangeBar *p_rangebar = (GChartRangeBar*) widget;
      static int32_t pressed_id = CHART_POINT_NONE;
      if(event == EVENT_DRAW_POST_END){
        uint32_t id = p_rangebar->pressed_point();                      // id ที่มีการ กดสัมผัส pressed เข้ามา
        if(id == CHART_POINT_NONE || id == pressed_id) return;
        pressed_id = id;
        if(series_bp != NULL && series_hr != NULL) {
          point_t p;
          p_rangebar->point_pos_by_id(series_bp, pressed_id, NULL, &p); // ตำแหน่งของ point id สำหรับ series ที่ต้องการ
          uint32_t col_w = p_rangebar->column_width();                  // ความกว้างของ แท่ง rangebar

          int32_t bp_DIA, bp_SYS, hr;
          chart_rangebar_bp.get_value_by_id(series_bp, pressed_id, &bp_DIA, &bp_SYS);   // รับค่า min, max ของ แท่งกราฟ rangebar จาก series ที่ id ที่ต้องการ
          chart_line_hr.get_value_by_id(series_hr, pressed_id, &hr);                    // รับค่า ของ กราฟเส้น จากจาก series ที่ id ที่ต้องการ

          if(bp_SYS != CHART_POINT_NONE && bp_DIA != CHART_POINT_NONE && hr != CHART_POINT_NONE) {
            Serial.printf("[press_data] (%d) %d - %d : %d\n", pressed_id, bp_DIA, bp_SYS, hr);

            draw_rect_dsc_t rect_dsc;
            p_rangebar->init_draw_rect_dsc(GPART_SELECTED, rect_dsc);
            draw_rect_dsc_init(&rect_dsc); //?
            rect_dsc.bg_color = TFT_COLOR(p_rangebar->bg_color(GPART_SELECTED));
            rect_dsc.bg_opa = p_rangebar->bg_opa(GPART_SELECTED);//LV_OPA_50;
            rect_dsc.radius = p_rangebar->radius(GPART_SELECTED);

            area_t rect_area;
            rect_area.x1 = p_rangebar->obj->coords.x1 + p.x - 100/2 - p_rangebar->padding_left(GPART_SELECTED); //- 20;
            rect_area.x2 = p_rangebar->obj->coords.x1 + p.x + 100/2 + p_rangebar->padding_right(GPART_SELECTED); // + 20;
            rect_area.y1 = p_rangebar->obj->coords.y1 + p.y - 70    + p_rangebar->padding_top(GPART_SELECTED); //- 30;
            rect_area.y2 = p_rangebar->obj->coords.y1 + p.y - 10    + p_rangebar->padding_bottom(GPART_SELECTED); //- 10;

            // ถ้า touch popup area สูงเกิน chart ให้ กดต่ำลงมา
            if( rect_area.y1 < p_rangebar->obj->coords.y1 + p_rangebar->border() + abs(p_rangebar->padding_top(GPART_SELECTED))) {
              rect_area.y1 = p_rangebar->obj->coords.y1 + p_rangebar->border()+ abs(p_rangebar->padding_top(GPART_SELECTED));
              rect_area.y2 = rect_area.y1 + 60;
            }
            // ถ้า touch popup area ด้านซ้ายน้อยกว่า chart ให้เข้าไป chart
            if( rect_area.x1 < p_rangebar->obj->coords.x1 + p_rangebar->border() + abs(p_rangebar->padding_left(GPART_SELECTED))) {
              int32_t rect_area_width = rect_area.x2 - rect_area.x1;
              rect_area.x1 = p_rangebar->obj->coords.x1+ p_rangebar->border() + abs(p_rangebar->padding_left(GPART_SELECTED));
              rect_area.x2 = rect_area.x1 + rect_area_width;
            }
            // ถ้า touch popup area ด้านขวามากกว่า chart ให้เข้าไป chart
            if( rect_area.x2 > p_rangebar->obj->coords.x2 - p_rangebar->border() - abs(p_rangebar->padding_right(GPART_SELECTED))) {
              int32_t rect_area_width = rect_area.x2 - rect_area.x1;
              rect_area.x2 = p_rangebar->obj->coords.x2 - p_rangebar->border() - abs(p_rangebar->padding_right(GPART_SELECTED));
              rect_area.x1 = rect_area.x2 - rect_area_width;
            }

            p_rangebar->softdrawRect(rect_dsc, rect_area);  // วาดระดับล่าง softdraw สี่เหลี่ยม 
            //------------------------------------------
            draw_triangle_dsc_t draw_triangle_dsc;
            draw_triangle_dsc_init(&draw_triangle_dsc);
            draw_triangle_dsc.bg_color = TFT_COLOR(chart_rangebar_bp.bg_color(GPART_SELECTED));
            draw_triangle_dsc.bg_opa = OPA_100;
            draw_triangle_dsc.p[0].x = chart_rangebar_bp.obj->coords.x1 + p.x + col_w/2;
            draw_triangle_dsc.p[0].y = rect_area.y2+10;
            draw_triangle_dsc.p[1].x = draw_triangle_dsc.p[0].x - 5;
            draw_triangle_dsc.p[1].y = rect_area.y2;
            draw_triangle_dsc.p[2].x = draw_triangle_dsc.p[0].x + 5;
            draw_triangle_dsc.p[2].y = rect_area.y2;

            if( draw_triangle_dsc.p[1].x < rect_area.x1) {
              draw_triangle_dsc.p[1].x = rect_area.x1 +5;
              draw_triangle_dsc.p[2].x = draw_triangle_dsc.p[1].x + 10;
            }

            if( draw_triangle_dsc.p[2].x > rect_area.x2) {
              draw_triangle_dsc.p[2].x = rect_area.x2 - 5;
              draw_triangle_dsc.p[1].x = draw_triangle_dsc.p[2].x - 10;
            }

            p_rangebar->softdrawTriangle(draw_triangle_dsc);  // วาดระดับล่าง softdraw สามเหลี่ยม 

            //------------------------------------------
            draw_line_dsc_t draw_line_dsc;
            draw_line_dsc_init(&draw_line_dsc);
            draw_line_dsc.color = TFT_COLOR(TFT_PALETTE(TFT_PALETTE_GRAY));
            draw_line_dsc.width = 1;
            draw_line_dsc.p1.x = rect_area.x1+5;
            draw_line_dsc.p2.x = rect_area.x2-5;
            draw_line_dsc.p1.y = rect_area.y1+20;
            draw_line_dsc.p2.y = draw_line_dsc.p1.y;
            
            p_rangebar->softdrawLine(draw_line_dsc);          // วาดระดับล่าง softdraw เส้น

            //------------------------------------------
            char buf[128];
            area_t label_area;

            draw_label_dsc_t draw_label_dsc;
            draw_label_dsc_init(&draw_label_dsc);

            snprintf(buf, sizeof(buf), SYMBOL_DUMMY"%s %02d:00-%02d:59", today_date, pressed_id, pressed_id);
            draw_label_dsc.text = buf;
            draw_label_dsc.color = TFT_COLOR(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,1));
            draw_label_dsc.font = &prasanmit_15;

            text_get_area(&label_area, &draw_label_dsc);              // พื้นที่ที่ครอบข้อความ
            area_align(&rect_area, &label_area, ALIGN_TOP);           // จัดให้พื้นที่ข้อความ ไว้ชิดบน ของพื้นที่สี่เหลี่ยม
            p_rangebar->softdrawLabel(draw_label_dsc, label_area);    // วาดระดับล่าง softdraw ข้อความ

            //------------------------------------------
            snprintf(buf, sizeof(buf), SYMBOL_DUMMY"SYS/DIA");
            draw_label_dsc.text = buf;
            draw_label_dsc.color = TFT_COLOR(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,1));
            draw_label_dsc.font = &prasanmit_15;

            text_get_area(&label_area, &draw_label_dsc);
            area_align(&rect_area, &label_area, ALIGN_LEFT, 10, 3);
            p_rangebar->softdrawLabel(draw_label_dsc, label_area);

            //------------------------------------------
            snprintf(buf, sizeof(buf), SYMBOL_DUMMY"%d/%d", bp_SYS, bp_DIA);
            draw_label_dsc.text = buf;
            draw_label_dsc.color = TFT_COLOR(TFT_BLACK);
            draw_label_dsc.font = &prasanmit_20;

            text_get_area(&label_area, &draw_label_dsc);
            area_align(&rect_area, &label_area, ALIGN_RIGHT, -10, 3 - (prasanmit_20.base_line-prasanmit_15.base_line)/2 );
            p_rangebar->softdrawLabel(draw_label_dsc, label_area);

            //------------------------------------------
            snprintf(buf, sizeof(buf), SYMBOL_DUMMY"Pulse");
            draw_label_dsc.text = buf;
            draw_label_dsc.color = TFT_COLOR(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,1));
            draw_label_dsc.font = &prasanmit_15;

            text_get_area(&label_area, &draw_label_dsc);
            area_align(&rect_area, &label_area, ALIGN_LEFT, 10, 18);
            p_rangebar->softdrawLabel(draw_label_dsc, label_area);
            //------------------------------------------

            snprintf(buf, sizeof(buf), SYMBOL_DUMMY"%d", hr);
            draw_label_dsc.text = buf;
            draw_label_dsc.color = TFT_COLOR(TFT_BLACK);
            draw_label_dsc.font = &prasanmit_20;

            text_get_area(&label_area, &draw_label_dsc);
            area_align(&rect_area, &label_area, ALIGN_RIGHT, -10, 18 - (prasanmit_20.base_line-prasanmit_15.base_line)/2 );
            p_rangebar->softdrawLabel(draw_label_dsc, label_area);
          }
        }
      }
    });

    series_bp = chart_rangebar_bp.createSerie(TFT_PALETTE(TFT_PALETTE_BLUE));
    chart_rangebar_bp.radius(5, GPART_ITEMS);      // มุมโค้งของแท่ง rangebar

    chart_line_hr.grid_cell(1,2, GRID_ALIGN_STRETCH, GRID_ALIGN_END);
    chart_line_hr.bg_opa(0);
    chart_line_hr.line_opa(0);
    chart_line_hr.range(40, 160);
    chart_line_hr.point_count(chart_rangebar_bp.point_count());       // จำนวนแท่ง rangebar ในหน้า chart_rangebar
    chart_line_hr.clickable(false);

    series_hr = chart_line_hr.createSerie(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BLUE,3));//TFT_COLOR_HSV(220,100,40));

  scale_chart_y.height(chart_rangebar_bp);
  scale_chart_y.range(40, 160);
  scale_chart_y.mode(SCALE_MODE_VER_LEFT, 7);
  scale_chart_y.tick_length(0);
  scale_chart_y.axis_thickness(0);
  scale_chart_y.label_show(true);
  scale_chart_y.font_color(TFT_WHITE);
  scale_chart_y.pad_left(10, GPART_INDICATOR);
  scale_chart_y.grid_cell(0,2, GRID_ALIGN_END, GRID_ALIGN_END);

  scale_chart_x.size(chart_rangebar_bp.width(),20);
  scale_chart_x.mode(SCALE_MODE_HORIZONTAL_BOTTOM, 24);
  scale_chart_x.font(prasanmit_18, TFT_WHITE);
  scale_chart_x.tick_length(0);
  scale_chart_x.axis_thickness(0);
  scale_chart_x.text_src( {"0",".",".","3",".",".","6",".",".","9",".",".","12",".",".","15",".",".","18",".",".","21",".","." });
  scale_chart_x.grid_cell(1,3, GRID_ALIGN_CENTER, GRID_ALIGN_START,0,20);

  lb_unit_mmHg = "mmHg";
  lb_unit_mmHg.font(prasanmit_15);
  lb_unit_mmHg.grid_cell(0,2, GRID_ALIGN_END, GRID_ALIGN_START, -5,25);

  lb_monthname = today_date;
  lb_monthname.font(prasanmit_15);
  lb_monthname.grid_cell(1,3, GRID_ALIGN_START, GRID_ALIGN_START,0,3);

  lb_title = "ความดันโลหิต & อัตราเต้นของหัวใจ";
  lb_title.grid_cell(0,0,3,1, GRID_ALIGN_CENTER, GRID_ALIGN_END);
  lb_title.text_align(TEXT_ALIGN_CENTER);
  
  lb_date = "มิถุนายน 10";
  lb_date.font(prasanmit_25);
  lb_date.align(lb_title, ALIGN_BOTTOM);
  lb_date.grid_cell(0,1,3,1, GRID_ALIGN_CENTER, GRID_ALIGN_END);
  lb_date.text_align(TEXT_ALIGN_CENTER);

  for(int i=0; i< chart_rangebar_bp.point_count(); i++){
    int32_t val_min = random(60,85);
    int32_t val_max = random(110,130);
    chart_rangebar_bp.addBar(series_bp, val_min, val_max );
    chart_line_hr.addPoint(series_hr, random(50,100));
  }
}

void loop(){
  BlynkGO.update();
}

