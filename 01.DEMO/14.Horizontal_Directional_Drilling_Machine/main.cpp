/* สำหรับจอ 800x480 */

#include <BlynkGOv5.h>

IMAGE_DECLARE(img_hdd_bg);
IMAGE_DECLARE(img_hdd_machine);

GImage hdd_bg(img_hdd_bg);
  GChart chart(hdd_bg);
    chart_series_t * series_rod;
    chart_series_t * series_rod_working;

  GScale scale_x_deg(hdd_bg);
  GScale scale_x_rod(hdd_bg);

  GLabel lb_scale_y_depth(hdd_bg);
  GScale scale_y_depth(hdd_bg);

  GLabel lb_degree(hdd_bg);
  GLabel lb_rod_no(hdd_bg);

  float depth_m[84] = {
     -0.0000,  -1.0692,  -2.0862,  -3.0510,  -3.9637,  -4.8243,  -5.6327,  -6.3889,  -7.0930,  -7.7449, 
     -8.3447,  -8.8923,  -9.3878,  -9.8311, -10.2222, -10.5612, -10.8481, -11.0828, -11.2653, -11.3957, 
    -11.4739, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, 
    -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, 
    -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, 
    -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000, -11.5000,
    -11.5000, -11.5000, -11.5000, -11.4739, -11.3957, -11.2653, -11.0828, -10.8481, -10.5612, -10.2222, 
     -9.8311,  -9.3878,  -8.8923,  -8.3447,  -7.7449,  -7.0930,  -6.3889,  -5.6327,  -4.8243,  -3.9637, 
     -3.0510,  -2.0862,  -1.0692,  -0.0000
  };

  GSlider slider_rod(hdd_bg);

  GImage hdd_machine(img_hdd_machine, hdd_bg);
  GRect rect_rig_info(hdd_bg);
    GLabel lb_rig_title(rect_rig_info);
    GLabel lb_rod_info_titlebar(rect_rig_info);
    GLabel lb_rod_id(rect_rig_info);
    GLabel lb_elevation_info_titlebar(rect_rig_info);
    GLabel lb_elevation_m(rect_rig_info);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  hdd_bg.position(0,0);
    chart.size(690, 160);
    chart.align(ALIGN_RIGHT, -10, 68);
    chart.type(CHART_TYPE_LINE);                        // แสดงกราฟแบบเส้น
    chart.point_count(84);                              // มีจุดข้อมูล 84 จุด
    chart.bg_opa(20);                                   // ความโปร่งใสพื้นหลัง 20
    chart.border_opa(20);                               // ความโปร่งใสขอบกราฟ 20
    chart.line_color(TFT_BLACK);
    chart.line_opa(30);
    chart.line_width(6, GPART_ITEMS);                   // เส้น series ความหนา 6
    chart.line_rounded(true, GPART_ITEMS);              // เส้น series ให้ปิดด้วยมุมกลม
    chart.div_line_count(7,84);                         // เส้นกริด 7 แนวนอน 84 แนวตั้ง
    chart.hookDrawTask(true);                           // เปิดใช้งาน การ hook draw task
    chart.onDrawTask(GWIDGET_CB{                        // ตั้ง callback สำหรับ การ hook draw task
      if(chart.draw_part() == GPART_ITEMS &&            // ขณะที่มีการวาด ITEMS ของ chart (ITEMS ในที่นี้คือตัวแท่งกราฟแต่ละแท่งๆ)
         chart.draw_type() == DRAW_TYPE_LINE ) {        // โดยเป็นขณะที่กำลังวาดในส่วนเติมสีของเส้นกราฟ
        if(chart.draw_id1() == 0) {                     // ขณะที่กำลังวาดเส้นของ series แรก 
          auto line_dsc = chart.draw_line_dsc();        // ดึงโครงสร้างการตั้งค่าเส้นที่กำลังจะวาดขึ้นมาแก้ไข
          line_dsc->width = 4;                          // เปลี่ยนความหนาของเส้นเป็น 4 จากเดิมที่ตั้งไว้ 6
        }
      }
    });

    // ตั้ง popup แสดงค่าตำแหน่งที่สัมผัส
    chart.touch_popup(true, [](chart_series_t* series, uint32_t id, int32_t value)->String{
      if( series == series_rod_working )  return CHART_TOUCH_POPUP_SKIP;  // ไม่แสดง popup สำหรับ series_rod_working

      return StringX::printf("%d, %.4f", id, value/10000.0);   // ค่าที่สัมผัส จะอยู่ระหว่าง -120000 ถึง 0 มาจัด format เองได้
    });

    chart.range(CHART_AXIS_PRIMARY_Y, -120000, 0);            // แกน y หลัก มีช่วง -120000 ถึง 0
      series_rod_working  = chart.createSerie(TFT_BLACK);                                 // series ที่มาก่อนจะวาดอยู่บน series ที่มาหลัง
      series_rod          = chart.createSerie(TFT_PALETTE_LIGHTEN(TFT_PALETTE_GRAY,1));

    scale_x_deg.size(chart.width(), 50);
    scale_x_deg.align(chart, ALIGN_BOTTOM,0, 10);
    scale_x_deg.axis_thickness(0);
    scale_x_deg.tick_length(0);
    scale_x_deg.mode(SCALE_MODE_BOTTOM, 40);
    scale_x_deg.range(1,40);
    scale_x_deg.font(prasanmit_15, TFT_WHITE);
    scale_x_deg.label_show(true);
    static const char * x_deg_labels[] = {
      "-40", "-40", "-42", "-42", "-48", "-50", "-50", "-42", "-26", "-16", 
      "0", "+8", "+2", "+2", "-0", "-0", "-0", "-0", "-1", "-0", 
      "-0", "+4", "+22", "+17", "+14", "+10", "+5", "+4", "+4", "+10", 
      "+7", "+9", "+8", "+9", "+15", "+25", "+44", "+48", "+42", "+44",  NULL};
    scale_x_deg.text_src(x_deg_labels);

    lb_degree = "DEGREE";
    lb_degree.font(prasanmit_15, TFT_WHITE);
    lb_degree.align(chart, ALIGN_OUT_LEFT_BOTTOM, -15, 30); 

    scale_x_rod.size(chart.width(), 50);
    scale_x_rod.align(chart, ALIGN_BOTTOM, 0,20+5);
    scale_x_rod.axis_thickness(0);
    scale_x_rod.tick_opa(0);
    scale_x_rod.mode(SCALE_MODE_BOTTOM, 84, 5);
    scale_x_rod.range(0,83);
    scale_x_rod.font(prasanmit_15, TFT_WHITE);
    scale_x_rod.label_show(true);

    lb_rod_no = "ROD.NO.";
    lb_rod_no.font(prasanmit_15, TFT_WHITE);
    lb_rod_no.align(lb_degree, ALIGN_BOTTOM_LEFT, 0, 0); 

    scale_y_depth.size(60, chart.height());
    scale_y_depth.align(chart, ALIGN_LEFT,-5);
    scale_y_depth.axis_thickness(0);
    scale_y_depth.tick_length(0);
    scale_y_depth.tick_opa(128);
    scale_y_depth.mode(SCALE_MODE_LEFT, 6+1, 1);
    scale_y_depth.font(prasanmit_15, TFT_WHITE);
    scale_y_depth.range(-12,0);
    scale_y_depth.label_show(true);

    lb_scale_y_depth = "ELEVATION";
    lb_scale_y_depth.font(prasanmit_15, TFT_WHITE);
    lb_scale_y_depth.align(chart, ALIGN_LEFT, -20, 0);

    for (int i = 0; i < 84; i++) {
      series_rod->y_points[i] = depth_m[i] *10000;
      series_rod_working->y_points[i] = CHART_POINT_NONE;
    }

    rect_rig_info.size(SIZE_CONTENT, SIZE_CONTENT);
    rect_rig_info.padding(0, 0, 0, 0, 0, 5);
    rect_rig_info.bg_opa(0);
    rect_rig_info.position(500, 30);
    rect_rig_info.GRID_CELL(GRID_COL{120, 140}, GRID_ROW {GRID_CONTENT, 5, GRID_CONTENT, 60});

      lb_rig_title.grid_cell(0, 0, 2, 1);                   // ไว้ที่คอลัมน์ที่ 0 แถวที่ 0 กินพื้นที่ 2 คอลัมน์ 1 แถว
      lb_rig_title.text_align(TEXT_ALIGN_CENTER);           // จัดกึ่งกลาง
      lb_rig_title = "เครื่องเจาะท่อแนวนอนใต้ดิน (HDD Machine)";
      lb_rig_title.font(prasanmit_18, TFT_PALETTE_DARKEN(TFT_PALETTE_INDIGO, 3));

      lb_rod_info_titlebar.grid_cell(0,2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  // ไว้ที่คอลัมน์ที่ 0 แถวที่ 2 และขยายเต็มความกว้างคอลัมน์และแถว
      lb_rod_info_titlebar.text_align(TEXT_ALIGN_CENTER);   // จัดกึ่งกลาง
      lb_rod_info_titlebar = "ROD ID";
      lb_rod_info_titlebar.font(prasanmit_18, TFT_WHITE);
      lb_rod_info_titlebar.bg_opa(255);
      lb_rod_info_titlebar.bg_color(TFT_PALETTE(TFT_PALETTE_BLUE));

      lb_rod_id.grid_cell(0,3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);             // ไว้ที่คอลัมน์ที่ 0 แถวที่ 3 และขยายเต็มความกว้างคอลัมน์และแถว
      lb_rod_id.text_align(TEXT_ALIGN_CENTER);
      lb_rod_id.pad_top(8);                                 // ช่องไฟด้านบน 8 pixel
      lb_rod_id.bg_opa(70);                                 // ความโปร่งใสพื้นหลัง 70
      lb_rod_id.font(BebasNeueBold_num_45);
      lb_rod_id = 0;

      lb_elevation_info_titlebar.grid_cell(1,2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      lb_elevation_info_titlebar.text_align(TEXT_ALIGN_CENTER);
      lb_elevation_info_titlebar = "ELEVATION (m)";
      lb_elevation_info_titlebar.font(prasanmit_18, TFT_WHITE);
      lb_elevation_info_titlebar.bg_opa(255);
      lb_elevation_info_titlebar.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BROWN, 1));

      lb_elevation_m.grid_cell(1,3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      lb_elevation_m.text_align(TEXT_ALIGN_CENTER);
      lb_elevation_m.pad_top(lb_rod_id.pad_top());        // ช่องไฟด้านบน เท่ากับ lb_rod_id
      lb_elevation_m.bg_opa(lb_rod_id.bg_opa());          // ความโปร่งใสพื้นหลัง เท่ากับ lb_rod_id
      lb_elevation_m.font(BebasNeueBold_num_45); 
      lb_elevation_m = 0;
        
    hdd_machine.align(rect_rig_info, ALIGN_LEFT, -15, 25);

    slider_rod.size(chart.width(), 4);
    slider_rod.align(chart, ALIGN_TOP, 0, -5);
    slider_rod.color(TFT_PALETTE(TFT_PALETTE_RED));
    slider_rod.range(0,83);
    slider_rod.mode(SLIDER_MODE_NORMAL);
    slider_rod.knob_opa(0);
    slider_rod.bg_opa(128);
    slider_rod.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,2));
    slider_rod.onValueChanged(GWIDGET_CB{
      int32_t rod_id = slider_rod.value();
      lb_rod_id = rod_id;
      lb_elevation_m = StringX::printf("%.2f", depth_m[rod_id]);
      for (int i = 0; i < 84 ; i++) {
        if(i<= rod_id){
          series_rod_working->y_points[i] = depth_m[i] *10000;
        } else {
          series_rod_working->y_points[i] = CHART_POINT_NONE;   // ไม่แสดงค่าของจุดข้อมูลที่เกินกว่า rod_id
        }
      }
      chart.refresh();      // รีเฟรช chart เพื่อแสดงผลใหม่ สำหรับการใส่ data แบบ direct input ให้ series
    });
    slider_rod = 30;
    slider_rod.event_send(EVENT_VALUE_CHANGED); 

}

void loop(){
  BlynkGO.update();
}
