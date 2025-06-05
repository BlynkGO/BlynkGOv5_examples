#include <BlynkGOv5.h>

FONT_DECLARE(AftikaBold_35);
FONT_DECLARE(AftikaBold_30);
FONT_DECLARE(AftikaBold_20);
FONT_DECLARE(AftikaBold_15);

IMAGE_DECLARE(img_heart_rate);
IMAGE_DECLARE(img_heart_rate2);

#define CHART_MAX_POINT_COUNT       200

#define CHART_CELL_WIDTH            5
#define CHART_CELL_HOR_BOX_NUM      60
#define CHART_CELL_VER_BOX_NUM      25

GContainer cont_ecg_monitor;
  GLabel lb_ecg_monitor("ECG POCKET MONITOR", cont_ecg_monitor);          // วิตเจ็ต ข้อความ title
  GButton btn_home( SYMBOL_HOME " Home", cont_ecg_monitor);               // วิตเจ็ต ปุ่ม Home
  GButton btn_save( SYMBOL_SAVE " Save", cont_ecg_monitor);               // วิตเจ็ต ปุ่ม Save
  GChart chart[6];                                                        // วิตเจ็ต Chart 6 ชุด
    chart_series_t *series[6];                                            // อนุกรมของ chart สำหรับเก็บข้อมูล chart
    enum {CHART_I, CHART_aVR, CHART_II, CHART_aVL, CHART_III, CHART_aVF}; // enum สำหรับระบุ chart อะไร
    GLabel lb_chart_info[6] = { "I", "aVR", "II", "aVL", "III", "aVF"};
  GScale chart_scale[3];                                                  // วิตเจ็ต Scale 3 ชุด

  GRect box_HR(cont_ecg_monitor);                                         // กล่อง HR
    GLabel lb_HR("HR", box_HR);
    GLabel lb_HR_val(box_HR);                                             // GLabel สำหรับแสดงค่า HR
    GLabel lb_HR_unit("BPM", box_HR);
    GImage image_HR(img_heart_rate, box_HR);

  GRect box_SpO2(cont_ecg_monitor);                                       // กล่อง SpO2
    GLabel lb_SpO2("SpO2", box_SpO2);
    GLabel lb_SpO2_val(box_SpO2);                                         // GLabel สำหรับแสดงค่า SpO2
    GLabel lb_SpO2_unit("%", box_SpO2);

    GContainer box_SpO2_index(box_SpO2);                                  // วิตเจ็ตสำหรับเป็น index สี สำหรับ SpO2 (บอกระดับ SpO2 ด้วยสี)
      GRect rect_SpO2_index[6];


  GButton btn_I_II_III("I-II-III",cont_ecg_monitor);
  GButton btn_R_L_F("R-L-F",cont_ecg_monitor);

namespace HR {
  void value(uint16_t v){
    lb_HR_val = v;
  }
}

namespace SpO2 {
  void value(float v){  // 0-100.0
    v = constrain(v,0,100);
    lb_SpO2_val = v;
    uint8_t SpO2_index =  floor(v*6/100.0);
    for(int i=0; i<6; i++) {
      if(i<=SpO2_index)
        rect_SpO2_index[i].color( TFT_COLOR_HSV( map(i,0,5,0,120), 100, map(i,0,5,100,70)));
      else
        rect_SpO2_index[i].color( TFT_COLOR_HSV( map(i,0,5,0,120),  10, map(i,0,5,100,60)));
    }
  }
};


// ค่าคงที่ของ ECG
#define SAMPLE_RATE 100  // 100 Hz (ตัวอย่างต่อวินาที)
#define PERIOD 1.0       // 1 วินาทีต่อรอบ ECG

// Gaussian ฟังก์ชันสำหรับ QRS Complex
float gaussian(float x, float mean, float sigma, float amplitude) {
  return amplitude * exp(-pow(x - mean, 2) / (2 * pow(sigma, 2)));
}

// Exponential Decay สำหรับ P-wave และ T-wave
float exp_decay(float x, float mean, float scale, float amplitude) {
  return amplitude * exp(-fabs(x - mean) / scale);
}

// ฟังก์ชันสร้างค่าคลื่น ECG ให้ใกล้เคียงความเป็นจริง
float generateECG(float t, float A_P, float A_QRS, float A_T) {
  float ecg = 0;

  // P-wave: ใช้ Exponential Decay เพื่อให้ลักษณะโค้งสมจริง
  ecg += exp_decay(t, 0.2, 0.15, A_P);

  // QRS Complex: ใช้ Gaussian Mixture Model
  ecg += gaussian(t, 0.5, 0.02, A_QRS)  // Q
       - gaussian(t, 0.52, 0.01, A_QRS * 1.5)  // R (Peak สูง)
       + gaussian(t, 0.54, 0.015, A_QRS * 0.7); // S

  // T-wave: ใช้ Exponential Decay
  ecg += exp_decay(t, 0.75, 0.2, A_T);

  return constrain(ecg, -80, 80);
}


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_ecg_monitor.size(PCT(100), PCT(100));
  cont_ecg_monitor.GRID_CELL( GRID_COL{ GRID_FR(1), GRID_CONTENT, 1, GRID_CONTENT, 100 }, GRID_ROW {GRID_FR(1), GRID_CONTENT, GRID_CONTENT, GRID_CONTENT});
  cont_ecg_monitor.padding(20,30,15,15,10,10);            // ช่องไฟ บน ล่าง ซ้าย ขวา ระหว่างแนวตัง แนวนอน
  cont_ecg_monitor.color(TFT_COLOR_HSV(210,96,45), TFT_COLOR_HSV(210,96,25));  // สีเฉด
  cont_ecg_monitor.grad_dir(GRAD_DIR_HOR);                // เล่นเฉดสี ซ้ายไปขวา
    lb_ecg_monitor.grid_cell(0,0,5,1);
    lb_ecg_monitor.text_align(TEXT_ALIGN_CENTER);
    lb_ecg_monitor.font(AftikaBold_35, TFT_WHITE);

    btn_home.grid_cell(0,0,5,1, GRID_ALIGN_START, GRID_ALIGN_CENTER);
    btn_home.size(140,35);
    btn_home.radius(8);
    btn_home.onClicked(GWIDGET_CB{

    });

    btn_save.grid_cell(0,0,5,1, GRID_ALIGN_END, GRID_ALIGN_CENTER);
    btn_save.size(btn_home);
    btn_save.radius(8);
    btn_save.onClicked(GWIDGET_CB{

    });

    for(int i=0; i< 6; i++){
      chart[i].parent(cont_ecg_monitor);
      chart[i].size(CHART_CELL_WIDTH*CHART_CELL_HOR_BOX_NUM, CHART_CELL_WIDTH*CHART_CELL_VER_BOX_NUM);  // ช่องละ 5 pixel จำนวน 60ช่อง x 24 ช่อง
      int8_t r = 1 + (i/2);
      int8_t c = (i%2)? 3 : 1;
      chart[i].grid_cell(c, r);                               // วางไว้ที่ grid_cell ที่กำหนด
      chart[i].type(CHART_TYPE_LINE);                         // ให้วาด chart เป็น กราฟเส้น
      chart[i].point_count(CHART_MAX_POINT_COUNT);            // จำนวนจุดทั้งหมดใน chart
      chart[i].range(CHART_AXIS_PRIMARY_Y, -100, 150);        // ค่าแกน Y อยุ่ระหว่าง -120 ถึง 120  ( ให้ค่าที่รับมา x100 เสียก่อน ค่อยกำหนดใส่ chart)
      chart[i].div_line_count(CHART_CELL_VER_BOX_NUM+1, CHART_CELL_HOR_BOX_NUM+1);    // ตีเส้นตารางจำนวน 24+1 เส้นแนวนอน 60+1 เส้นแนวตั้ง
      chart[i].border(0);                                     // ไม่ต้องมีขอบ
      chart[i].line_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,3)); // ตีเส้นตารางด้วยพาเลทสีแดง สีอ่อน 3 ระดับ
      chart[i].line_width(1);                                 // หนา 1 px
      chart[i].line_opa(50);                                  // ให้ โปร่งใส 50 (จะทำให้เส้นดูบางลง กว่า 1px ปกติ)
      // ทำการ hook การวาดกราฟิกระดับล่าง
      chart[i].hookDrawTask(true);
      chart[i].onDrawTask(GWIDGET_CB{                        // เมื่อมีการวาดกราฟิกระดับล่าง
        int ii = (GChart*) widget -&chart[0];                 // ลำดับ ii นี้จะตรงกับ i พอดี (เนื่องจาก i จะเข้ามาใน นี้ไม่ได้คำนวณผ่านทาง pointer แทน)
        if( chart[ii].draw_part() == GPART_MAIN &&            // ขณะ กำลังวาดระดับล่าง ในส่วน main ของ chart
            chart[ii].draw_type() == DRAW_TASK_TYPE_LINE )    // ขณะ กำลังวาดระดับล่าง ในส่วนตี line ของ chart
        {
          auto line_dsc = chart[ii].draw_line_dsc();          // รายละเอียด line ที่กำลังจะวาดระดับล่าง
          if(line_dsc->p1.x == chart[ii].coords()->x1 &&      // แสดงว่า กำลังวาดเส้นแนวนอน
             line_dsc->p2.x == chart[ii].coords()->x2 )
          {
            // หากกราฟิกระดับล่าง กำลังตีเส้นแนวนอน เส้นที่ 2, 12, 22
            // if(line_dsc->base.id1 == 2 || line_dsc->base.id1 == 12 || line_dsc->base.id1 == 22) {
            if(line_dsc->base.id1%5 == 0 && line_dsc->base.id1 > 0) {
              line_dsc->opa = 180;                            // ให้โปร่งใสมากกว่าปกติที่เคยกำหนดไว้ 50 ขึ้นมาเป็น 180 (จะทำให้ดูเหมือนเข้มขึ้น)
              // if(line_dsc->base.id1==15){                     // เส้นที่ id 15 ให้เป็นเส้นสีดำ
              //   line_dsc->color = TFT_COLOR(TFT_BLACK);
              // }
            }
          }
          else
          if(line_dsc->p1.y == chart[ii].coords()->y1 &&      // แสดงว่า กำลังวาดเส้นแนวตั้ง
             line_dsc->p2.y == chart[ii].coords()->y2 )
          {
            // หากกราฟิกระดับล่าง กำลังตีเส้นแนวตั้ง ทุกๆ เส้นที่ 10 ที่ไม่ใช่เส้นแรก
            if(line_dsc->base.id1%5 == 0 && line_dsc->base.id1 > 0) {
              line_dsc->opa = 180;                            // ให้โปร่งใสมากกว่าปกติที่เคยกำหนดไว้ 50 ขึ้นมาเป็น 180 (จะทำให้ดูเหมือนเข้มขึ้น)
            }
          }
        }
      });
      series[i] = chart[i].createSerie(TFT_BLACK);

      lb_chart_info[i].ignore_layout(true);
      lb_chart_info[i].parent(chart[i]);
      lb_chart_info[i].font( AftikaBold_15, TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,3));
      lb_chart_info[i].position(10,3);
    }

    // ตี เส้น scale สำหรับ chart 
    for(int i=0; i< 3 ;i++){
      chart_scale[i].parent(cont_ecg_monitor);
      int8_t r = 1 + i;
      int8_t c = 0;
      chart_scale[i].grid_cell(c,r, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER,9,0);
      // chart_scale[i].height(CHART_CELL_WIDTH* (CHART_CELL_VER_BOX_NUM-4));
      chart_scale[i].height(CHART_CELL_WIDTH* (CHART_CELL_VER_BOX_NUM));
      chart_scale[i].range(-10, 15);
      chart_scale[i].mode(SCALE_MODE_VER_LEFT, 6, 1);
      chart_scale[i].tick_length(3,0);
      chart_scale[i].axis_thickness(0);
      chart_scale[i].label_show(true);
      static const char * custom_labels[] = {"-1.0", "", "0", "", "1.0", "", NULL};
      chart_scale[i].text_src(custom_labels);                // ข้อความ ที่ เส้นขีดหลัก
      chart_scale[i].color(TFT_WHITE);
      chart_scale[i].font(prasanmit_25, TFT_WHITE);
      // ทำการ hook การวาดกราฟิกระดับล่าง
      chart_scale[i].hookDrawTask(true);
      chart_scale[i].onDrawTask(GWIDGET_CB{                        // เมื่อมีการวาดกราฟิกระดับล่าง
        int ii = (GScale*) widget -&chart_scale[0];                 // ลำดับ ii นี้จะตรงกับ i พอดี (เนื่องจาก i จะเข้ามาใน นี้ไม่ได้คำนวณผ่านทาง pointer แทน)
        if( chart_scale[ii].draw_part() == GPART_INDICATOR &&       // ขณะ กำลังวาดระดับล่าง ในส่วนของ indicator ของ scale
            chart_scale[ii].draw_type() == DRAW_TASK_TYPE_LABEL )   // ขณะ กำลังวาดระดับล่าง ในส่วน label ของ scale
        {
          auto label_dsc = chart_scale[ii].draw_label_dsc();        // รายละเอียด label ที่กำลังจะวาดระดับล่าง
          label_dsc->ofs_x = -10;                                   // ให้เลื่อน ไปทางซ้าย 10px
          if(String(label_dsc->text) == "-1.0" ){                   // หากกำลังวาด "-1.0" ในกราฟิกระดับล่าง
            label_dsc->ofs_y = -5;                                  // ให้ปรับเลื่อนขึ้น 5px
          }
          // else
          // if(String(label_dsc->text) == "1.0" ){                    // หากกำลังวาด "1.0" ในกราฟิกระดับล่าง
          //   label_dsc->ofs_y =  5;                                  // ให้ปรับเลื่อนลง 5px
          // }
        }
      });
    }

    box_HR.border(5, TFT_BLACK);
    box_HR.radius(20);
    box_HR.size(100,115);
    box_HR.grid_cell(4,1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_HR.color(TFT_BLACK);
      lb_HR.font(AftikaBold_15, TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,3));
      lb_HR.align(ALIGN_TOP_LEFT,8,8);
      lb_HR_val.font(AftikaBold_30, TFT_BLACK);
      lb_HR_val = 120;
      lb_HR_unit.font(prasanmit_20);
      lb_HR_unit.align(ALIGN_BOTTOM, 0,-10);

      image_HR.align(lb_HR, ALIGN_RIGHT, 3);

    box_SpO2.border(5, TFT_BLACK);
    box_SpO2.radius(20);
    box_SpO2.size(box_HR);
    box_SpO2.grid_cell(4,2, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_SpO2.color(TFT_BLACK);
      lb_SpO2.font(AftikaBold_15, TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,3));
      lb_SpO2.align(ALIGN_TOP_LEFT,8,8);
      lb_SpO2_val.font(AftikaBold_30, TFT_BLACK);
      lb_SpO2_val = 99;
      lb_SpO2_val.align(ALIGN_RIGHT,-30);
      lb_SpO2_unit.font(prasanmit_20);
      lb_SpO2_unit.align(ALIGN_BOTTOM,0, -10);

      box_SpO2_index.size(SIZE_CONTENT, SIZE_CONTENT);
      box_SpO2_index.layout(LAYOUT_COL_M, 0,0,0,0,1);
      box_SpO2_index.bg_opa(0);
      box_SpO2_index.radius(6);
      box_SpO2_index.corner_mask(true);
      box_SpO2_index.align(ALIGN_RIGHT, -5);
      for(int i=5; i>=0; i--){
        rect_SpO2_index[i].color( TFT_COLOR_HSV( map(i,0,5,0,120), 100, map(i,0,5,100,70)));
        rect_SpO2_index[i].size(12,12);
        rect_SpO2_index[i].parent(box_SpO2_index);
      }

    btn_I_II_III.height(45);
    btn_I_II_III.grid_cell(4,3, GRID_ALIGN_STRETCH, GRID_ALIGN_START,0,5);
    btn_I_II_III.border(5, TFT_BLACK);
    btn_I_II_III.clickable(false);
    btn_I_II_III.color(TFT_WHITE);
    btn_I_II_III.font( AftikaBold_20, TFT_BLACK);
    btn_I_II_III.shadow(0);
    btn_I_II_III.radius(20);
    btn_I_II_III.bg_image_src(&img_heart_rate2);

    btn_R_L_F.height(45);
    btn_R_L_F.grid_cell(4,3, GRID_ALIGN_STRETCH, GRID_ALIGN_END,0,-5);
    btn_R_L_F.border(5, TFT_BLACK);
    btn_R_L_F.clickable(false);
    btn_R_L_F.color(TFT_WHITE);
    btn_R_L_F.font( AftikaBold_20, TFT_BLACK);
    btn_R_L_F.shadow(0);
    btn_R_L_F.radius(20);
    btn_R_L_F.image(&img_heart_rate2);

    //------------------------------------------------------
    static SoftTimer timer_chart;
    timer_chart.setInterval(5,[](){
      static int s=-1;
      s = (s+1)%SAMPLE_RATE;
      float t = s / (float)SAMPLE_RATE;

      static int32_t cur_idx = -1;    
      cur_idx = (cur_idx+1)% CHART_MAX_POINT_COUNT;
      for(int i=0; i< 6; i++){
        // ปรับความแรงของแต่ละส่วนของคลื่น
        float A_P = random(5,20);     //10;  // ความแรงของ P-wave
        float A_QRS = random(70,110); //80; // ความแรงของ QRS complex
        float A_T = random(10,30);    //15;  // ความแรงของ T-wave
        float ecg_value = generateECG(t, A_P, A_QRS, A_T);

        series[i]->y_points[cur_idx] = ecg_value;
        // ใส่ ไม่มีข้อมูลให้ chart พ่วงท้าย 10 ข้อมูล
        for(int j=cur_idx+1; j<= cur_idx+10; j++) {
          series[i]->y_points[ j%CHART_MAX_POINT_COUNT] = CHART_POINT_NONE;
        }
        chart[i].invalidate();
      }
    });

    HR::value(120);
    SpO2::value(36);
}

void loop(){
  BlynkGO.update();
}
