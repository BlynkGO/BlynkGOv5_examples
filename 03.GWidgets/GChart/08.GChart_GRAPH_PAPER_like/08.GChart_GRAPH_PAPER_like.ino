#include <BlynkGOv5.h>


#define ECG_SAMPLE_RATE             250     // Hz (AHA standard)
#define ECG_BPM                     75
#define ECG_BEAT_SAMPLES            (60 * ECG_SAMPLE_RATE / ECG_BPM)  // = 200 samples/beat

#define CHART_CELL_WIDTH            5       // ขนาดกว้าง px ของ ช่องเล็กๆของเส้นกระดาษกราฟ
#define CHART_CELL_HOR_BOX_NUM      120     // จำนวนช่อง ในแนวนอนทั้งหมด   (แทน 120 วินาที)
#define CHART_CELL_VER_BOX_NUM      80      // จำนวนช่อง ในแนวตั้งทั้งหมด
#define CHART_Y_MIN                 -200
#define CHART_Y_MAX                 200
#define CHART_MAX_POINT_COUNT       480

#define SIM_MODE                    true

#if SIM_MODE
  int32_t ECG_Waveform_Generator ();
  int32_t ECG_sensor_read(){
    return ECG_Waveform_Generator();
  }
#else
  int32_t ECG_sensor_read(){
    return analogRead(....); // ให้อ่านจาก sensor จริง
  }
#endif

GChart chart;
  chart_series_t *series;

GScale scale_axis_y;  // เส้น scale แกน y สำหรับ chart


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  Serial.println(CHART_MAX_POINT_COUNT);

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
  chart.onDrawTask(GWIDGET_CB{                         // เมื่อมีการวาดกราฟิกระดับล่าง
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
  chart.line_width(2, GPART_ITEMS);                 // ความหนาของเส้นกราฟ (เส้น series)
  chart.line_opa(250, GPART_ITEMS);                 // ความโปร่งใสของเส้นกราฟ (เส้น series);

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

  // จุดรับข้อมูล 250Hz → เก็บใน buffer
  static SoftTimer timer_input_data;
  static std::vector<int32_t> series_value_buffer;
  timer_input_data.setInterval(1000 / ECG_SAMPLE_RATE, []() {   // ทุก 4ms  (250Hz)
    int32_t val = ECG_sensor_read();
    series_value_buffer.push_back(val);
  });

  // flush ทุก 100ms → วาด chart ครั้งเดียว
  static SoftTimer timer_add_to_chart;
  timer_add_to_chart.setInterval(100, []() {
    if (series_value_buffer.empty()) return;
    // วาดข้อมูลทั้งหมดที่เก็บมาในรอบ 100ms
    for (int i = 0; i < series_value_buffer.size(); i++) {
      chart.addPoint(series, series_value_buffer[i]);
    }
    // ล้าง Buffer เตรียมรับรอบถัดไป
    series_value_buffer.clear();
  }); 

}

void loop() {
  BlynkGO.update();
}

#if SIM_MODE

// ==================== ECG Waveform Generator ====================
// มาตรฐาน AHA: 250Hz sampling, 75 BPM, 1 beat = 200 samples
// P-QRS-T wave lookup table (200 จุด = 1 รอบการเต้น)
// ค่าอยู่ระหว่าง -200 ถึง 200 ตรงกับ CHART_Y_MIN/MAX

#ifdef ESP32
  // MCU: เก็บใน Flash
  static const int16_t ECG_BEAT[200] PROGMEM = {
#else
  // PC/Windows: RAM ธรรมดา 
  static const int16_t ECG_BEAT[200]  = {
#endif
  // --- Baseline (isoelectric line) ก่อน P wave ---
  0,0,0,0,0,0,0,0,0,0,                          // [0-9]   baseline
  0,0,0,0,0,0,0,0,0,0,                          // [10-19] baseline

  // --- P wave (ขนาดเล็ก กลม ~20 sample) ---
   4, 8,12,18,24,28,30,28,24,18,               // [20-29] P wave ขึ้น
  12, 8, 4, 2, 0, 0, 0, 0, 0, 0,               // [30-39] P wave ลง

  // --- PR segment (baseline ก่อน QRS) ---
  0,0,0,0,0,0,0,0,0,0,                          // [40-49]
  0,0,0,0,0,0,0,0,0,0,                          // [50-59]

  // --- QRS Complex (คม แหลม สูง ~15 sample) ---
   0, -5,-12,-20,-30,-20,                       // [60-65] Q wave ลงเล็กน้อย
 100,160,200,180,                               // [66-69] R wave ขึ้นสูงสุด
 120, 60, 10,-15,-30,                           // [70-74] S wave ลงต่ำ
 -20,-10, -5,  0,  0,                           // [75-79] กลับ baseline

  // --- ST segment (baseline หลัง QRS) ---
  0,0,0,0,0,0,0,0,0,0,                          // [80-89]
  0,0,0,0,0,0,0,0,0,0,                          // [90-99]

  // --- T wave (กลม นูน ~40 sample) ---
   2, 5,10,16,22,28,34,40,46,50,               // [100-109] T wave ขึ้น
  52,50,46,40,34,28,22,16,10, 6,               // [110-119] T wave ลง
   3, 1, 0, 0, 0, 0, 0, 0, 0, 0,               // [120-129] กลับ baseline

  // --- TP segment (baseline พัก ก่อนรอบถัดไป) ---
  0,0,0,0,0,0,0,0,0,0,                          // [130-139]
  0,0,0,0,0,0,0,0,0,0,                          // [140-149]
  0,0,0,0,0,0,0,0,0,0,                          // [150-159]
  0,0,0,0,0,0,0,0,0,0,                          // [160-169]
  0,0,0,0,0,0,0,0,0,0,                          // [170-179]
  0,0,0,0,0,0,0,0,0,0,                          // [180-189]
  0,0,0,0,0,0,0,0,0,0,                          // [190-199]
};

#ifdef ESP32
  // MCU: เก็บใน Flash
  #define ECG_READ(i)  ((int32_t)pgm_read_word(&ECG_BEAT[i]))
#else
  // PC/Windows: RAM ธรรมดา 
  #define ECG_READ(i)  ((int32_t)ECG_BEAT[i])
#endif

int32_t ECG_Waveform_Generator (){
  static int16_t ecg_index = -1;
  int32_t val = ECG_READ(ecg_index++ % ECG_BEAT_SAMPLES) * 60 / 100;   
  return val;
}

#endif // SIM_MODE
