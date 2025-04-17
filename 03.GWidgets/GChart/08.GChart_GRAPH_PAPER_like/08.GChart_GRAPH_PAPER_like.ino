#include <BlynkGOv5.h>

#define DATA_SAMPLING_HZ            4       // จำนวนรับข้อมูล กี่ครั้งต่อวินาที (Hz) 

#define CHART_CELL_WIDTH            5       // ขนาดกว้าง px ของ ช่องเล็กๆของเส้นกระดาษกราฟ
#define CHART_CELL_HOR_BOX_NUM      120     // จำนวนช่อง ในแนวนอนทั้งหมด   (แทน 120 วินาที)
#define CHART_CELL_VER_BOX_NUM      80      // จำนวนช่อง ในแนวตั้งทั้งหมด

#define CHART_Y_MIN                 -200
#define CHART_Y_MAX                 200

#define CHART_MAX_POINT_COUNT       (DATA_SAMPLING_HZ*CHART_CELL_HOR_BOX_NUM )  // ให้มี กี่จุด ใน serie ของ chart


GChart chart;
  chart_series_t *series;

GScale scale_axis_y;  // เส้น scale แกน y สำหรับ chart

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  chart.size(CHART_CELL_WIDTH*CHART_CELL_HOR_BOX_NUM, CHART_CELL_WIDTH*CHART_CELL_VER_BOX_NUM);  // ช่องละ 5 pixel จำนวน 120ช่อง x 80 ช่อง
  chart.type(CHART_TYPE_LINE);                         // ให้วาด chart เป็น กราฟเส้น
  chart.point_count(CHART_MAX_POINT_COUNT);            // จำนวนจุดทั้งหมดใน chart
  chart.range(CHART_AXIS_PRIMARY_Y, CHART_Y_MIN, CHART_Y_MAX);  // ค่าแกน Y ค่าที่จะใส่เข้ามาใน serie อยุ่ระหว่าง range -200 ถึง 200  ( หากค่าจริงเป็นทศนิยม -2 ถึง 2  ให้ค่าที่รับมานี้ x100เท่า เสียก่อน ค่อยกำหนดใส่ serie ของ chart)
  chart.div_line_count(CHART_CELL_VER_BOX_NUM+1, CHART_CELL_HOR_BOX_NUM+1);    // ตีเส้นตารางจำนวน 120+1 เส้นแนวนอน 80+1 เส้นแนวตั้ง
  chart.border(0);                                     // ไม่ต้องมีขอบ
  chart.line_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,3)); // ตีเส้นตารางด้วยพาเลทสีแดง สีอ่อน 3 ระดับ
  chart.line_width(1);                                 // หนา 1 px
  chart.line_opa(50);                                  // ให้ โปร่งใส 50 (จะทำให้เส้นดูบางลง กว่า 1px ปกติ)
  // ทำการ hook การวาดกราฟิกขณะกำลังวาดระดับล่าง
  chart.hookDrawTask(true);                            // เปิดให้สามารถดักการวาดกราฟิกระดับล่างได้ด้วย
  chart.onDrawTask(GWIDGET_CB{                        // เมื่อมีการวาดกราฟิกระดับล่าง
    if( chart.draw_part() == GPART_MAIN &&             // ขณะ กราฟิกระดับล่าง กำลังวาด ในส่วน main ของ chart
        chart.draw_type() == DRAW_TASK_TYPE_LINE )     // ขณะ กราฟิกระดับล่าง กำลังวาด ตีเส้น line ของ chart
    {
      auto line_dsc = chart.draw_line_dsc();           // ดึงค่ารายละเอียดของ line ที่กำลังจะใช้วาดระดับล่าง
      if(line_dsc->p1.x == chart.coords()->x1 &&       // หาก กำลังวาดเส้นแนวนอน (เนื่องจากไปเท่าระยะต้นและปลายของแนวนอน)
          line_dsc->p2.x == chart.coords()->x2 )
      {
        // หากกำลังวาดกราฟิกระดับล่าง กำลังตีเส้นแนวนอน โดยนับจากเส้นที่ อยู่ตรงกลางแนวนอน
        if( line_dsc->base.id1 == CHART_CELL_VER_BOX_NUM/2) {
          line_dsc->width = 2;
          line_dsc->opa = 255;   
        }else // หากกำลังวาดกราฟิกระดับล่าง กำลังตีเส้นแนวนอน โดยนับจากเส้นที่ นับจากเส้นตรงกลางแนวนอน  นับถอยไป หรือ นับถัดไป ทุกๆ 10 เส้น
        if( abs( int(line_dsc->base.id1 - CHART_CELL_VER_BOX_NUM/2) ) %10 == 0) {
          line_dsc->opa = 180;                          // ให้ค่าโปร่งใส ปรับค่ามากกว่าปกติที่เคยกำหนดไว้ที่ 50 ขึ้นมาเป็น 180 (จะทำให้ เส้นแบ่งกราฟดูเหมือนเข้มขึ้น)
        }
      }
      else
      if(line_dsc->p1.y == chart.coords()->y1 &&        // หาก กำลังวาดเส้นแนวตั้ง (เนื่องจากไปเท่าระยะต้นและปลายของแนวตั้ง)
          line_dsc->p2.y == chart.coords()->y2 )
      {
        // หากกำลังวาดกราฟิกระดับล่าง กำลังตีเส้นแนวตั้ง ทุกๆ เส้นที่ 10 ที่ไม่ใช่เส้นแรก
        if(line_dsc->base.id1%10 == 0 && line_dsc->base.id1 > 0) {
          line_dsc->opa = 180;                          // ให้ค่าโปร่งใส ปรับค่ามากกว่าปกติที่เคยกำหนดไว้ที่ 50 ขึ้นมาเป็น 180 (จะทำให้ เส้นแบ่งกราฟดูเหมือนเข้มขึ้น)
        }
      }
    }
  });
  series = chart.createSerie(TFT_PALETTE(TFT_PALETTE_BLUE)); // สร้าง serie เส้นกราฟ ด้วยเส้นสีพาเลทน้ำเงิน
  chart.line_width(1, GPART_ITEMS);                 // ความหนาของเส้นกราฟ (เส้น series)
  chart.line_opa(180, GPART_ITEMS);                 // ความโปร่งใสของเส้นกราฟ (เส้น series);

  scale_axis_y.height(CHART_CELL_WIDTH* CHART_CELL_VER_BOX_NUM);
  scale_axis_y.range(CHART_Y_MIN, CHART_Y_MAX);
  scale_axis_y.mode(SCALE_MODE_VER_LEFT, 9, 2);     // เส้นสเกล แบบแนวตั้ง ด้านซ้าย มีขีดทั้งหมด 9 ขีด และ ขีดหลัก เว้นทุกๆ 2
  scale_axis_y.thickness(1);
  scale_axis_y.tick_length(6,3);                    // ความยาวของเส้นขีดหลัก และ รอง
  scale_axis_y.axis_thickness(0);                   // ความหนาของเส้นแกน ของสเกล
  scale_axis_y.label_show(true);                    // ให้แสดง label ของสเกลด้วย
  static const char * custom_labels_y[] = {"-2", "-1", "0", "1", "2", NULL};
  scale_axis_y.text_src(custom_labels_y);           // ข้อความ ที่ เส้นขีดหลัก แบบกำหนดเอง,
  scale_axis_y.pad_left(20, GPART_INDICATOR);       // ข้อความ ของ สเกล ห่างจาก ขีดของสเกล 20px
  scale_axis_y.color(TFT_WHITE);
  scale_axis_y.font(prasanmit_25, TFT_WHITE);
  scale_axis_y.align(chart, ALIGN_LEFT,-2);


  // จุดรับข้อมูล ทุกๆ กี่ HZ มาเก็บใน buffer ก่อนวาดจริง
  static SoftTimer timer_input_data;
  static std::vector<int32_t> series_value_buffer;
  timer_input_data.setInterval(1000/DATA_SAMPLING_HZ, [](){
    float sensor_value = random(-15,15)/10.0;         // ค่าจริงให้อ่านจาก sensor สมมติค่าที่อ่านได้อยู่ระหว่าง -1.5 ถึง 1.5
    int32_t series_value = map_f( sensor_value, -2.0, 2.0, CHART_Y_MIN, CHART_Y_MAX);  // ให้เปลี่ยนสัดส่วนเป็นตามค่าแกน Y ของ chart ก่อนกำหนดเข้าสู่ serie
    series_value_buffer.push_back(series_value);      // จะยังไม่มีการให้ chart มีการวาดกราฟออกมา ให้เก็บข้อมูลเข้ามาไว้ก่อน เนื่องจากความถี่ข้อมูลสูงมาก
  });

  // จุดถ่ายโอนข้อมูลใน buffer เข้าสู่ตัว series เพื่อวาดจริง (ความถี่จะต่ำลงมาจากการ render กราฟิกต้องใช้เวลา)
  static SoftTimer timer_add_to_chart;
  timer_add_to_chart.setInterval(1000,[](){
    for(int i=0; i< series_value_buffer.size(); i++){
      chart.addPoint(series, series_value_buffer[i]); // ดึงจาก buffer ที่เก็บ data ออกมาใส่ series ทั้งหมด แล้วค่อยวาด chart ทีเดียว ทุกๆ 1 วินาทีครั้ง
    }
    series_value_buffer.clear();                      // หลัง buffer นำมาใส่ serie แล้ว ล้าง buffer ทิ้ง
  });

}

void loop() {
  BlynkGO.update();
}