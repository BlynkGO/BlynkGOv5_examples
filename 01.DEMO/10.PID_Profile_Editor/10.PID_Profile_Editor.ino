#include <BlynkGOv5.h>

FONT_DECLARE(google_icon_arrow_20);

#define SYMBOL_ARROW_DROP_UP      "\ue5c7"
#define SYMBOL_ARROW_DROP_DOWN    "\ue5c5"
#define SYMBOL_ARROW_LEFT         "\ue5de"
#define SYMBOL_ARROW_RIGHT        "\ue5df"
#define SYMBOL_ARROW_CIRCLE_UP    "\uf182"
#define SYMBOL_ARROW_CIRCLE_DOWN  "\uf181"
#define SYMBOL_ARROW_CIRCLE_LEFT  "\ueaa7"
#define SYMBOL_ARROW_CIRCLE_RIGHT "\ueaaa"
#define SYMBOL_KEYBOARD_ARROW_UP  "\ue316"
#define SYMBOL_KEYBOARD_ARROW_DOWN "\ue313"
#define SYMBOL_KEYBOARD_ARROW_LEFT "\ue314"
#define SYMBOL_KEYBOARD_ARROW_RIGHT "\ue315"
#define SYMBOL_ARROW_UPWARD       "\ue5d8"
#define SYMBOL_ARROW_DOWNWARD     "\ue5db"
#define SYMBOL_ARROW_BACK         "\ue5c4"
#define SYMBOL_ARROW_FORWARD      "\ue5c8"
#define SYMBOL_NORTH              "\uf1e0"
#define SYMBOL_NORTH_EAST         "\uf1e1"
#define SYMBOL_EAST               "\uf1df"
#define SYMBOL_SOUTH_EAST         "\uf1e4"
#define SYMBOL_SOUTH              "\uf1e3"
#define SYMBOL_SOUTH_WEST         "\uf1e5"
#define SYMBOL_WEST               "\uf1e6"
#define SYMBOL_NORTH_WEST         "\uf1e2"

// วิตเจ็ตสำหรับ กดเพิ่มลด ตั้งค่า  ใช้เป็น ส่วนประกอบ ภายใน GPID_Step ไว้กำหนดตั้งค่า PID Profile's PID Step ต่อไป
class GSmartInput : public GContainer {
  public:
    GSmartInput(GWidget&parent=GScreen) : GContainer(parent) { }

    GTextArea ta;
    GButton btn[2];
    GLabel lb_unit;
    int32_t val =0;
    int32_t val_min = 0;
    int32_t val_max = 100;

    void create(){
      if(this->isCreated()) return;
      GContainer::create();
      GContainer::size(140,SIZE_CONTENT);
      GContainer::padding(0,0,0,0,1,0);
      GContainer::radius(5);
      GContainer::border(1, TFT_BLACK);
      GContainer::GRID_CELL( GRID_COL {GRID_FR(1), 30, 36},GRID_ROW{20,20});
        ta.parent(this);
        ta.grid_cell(0,0,1,2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
        ta.border(0);
        ta.bg_opa(0);
        ta.radius(5);
        ta.default_keyboard(true, KEYBOARD_MODE_NUMPAD);
        ta.text_align(TEXT_ALIGN_CENTER);
        ta.padding_top(1);
        ta.padding_bottom(0);
        ta = (val=0);
        ta.onValueChanged(GWIDGET_CB{
          GSmartInput* _input = (GSmartInput*) widget->parent();
          _input->value( _input->ta.toInt() );   // ภายในจะมีการเช็คอยู่ใน range และส่ง EVENT_VALUE_CHANGED หากค่าไม่ตรงค่าเดิม;
        });
        for(int i=0; i<2; i++){
          btn[i].parent(this);
          btn[i].grid_cell(1,i, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
          btn[i] = (i==0)? SYMBOL_ARROW_DROP_UP : SYMBOL_ARROW_DROP_DOWN;
          btn[i].color(TFT_PALETTE(TFT_PALETTE_GREEN));
          btn[i].font(google_icon_arrow_20, TFT_WHITE);
          btn[i].border(0);
          btn[i].shadow(0);
          btn[i].radius(0);
          static bool long_pressed=false;
          static int32_t _t;
          static int16_t _step;
          btn[i].onPressed(GWIDGET_CB{
            long_pressed=false;
            _t = millis() + 2000;
            _step = 1;
          });
          btn[i].onPressing(GWIDGET_CB{
            GSmartInput* _input = (GSmartInput*) widget->parent();
            int ii = (GButton *)widget -&_input->btn[0];
            if(_t <= millis() ) {
              _step += 5;
              _t = millis() + 2000;
            }
            if(long_pressed){
              _input->value(_input->val + ((ii==0)? +_step : -_step));
            }
          });
          btn[i].onClicked(GWIDGET_CB{
            GSmartInput* _input = (GSmartInput*) widget->parent();
            int ii = (GButton *)widget -&_input->btn[0];
            _input->value(_input->val + ((ii==0)? +1 : -1));
          });
          btn[i].onLongPressed(GWIDGET_CB{
            long_pressed= true;
          });
        }
        lb_unit.parent(this);
        lb_unit.grid_cell(2,0,1,2, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
        lb_unit.text_align(TEXT_ALIGN_CENTER);
        lb_unit.font(prasanmit_18);
        lb_unit = "นาที";//SYMBOL_DEGREE "C";
        lb_unit.clickable(true);
        lb_unit.onLongPressed(GWIDGET_CB{
          GSmartInput* _input = (GSmartInput*) widget->parent();
          _input->ta =  (_input->val = _input->val_min);
          _input->event_send(EVENT_VALUE_CHANGED);
        });
    }

    inline void     unit(String unit)   { create(); lb_unit = unit; }
    inline void     value(int32_t v)    { 
      create(); 
      v = constrain(v, val_min, val_max);
      if( v != val) {
        val = v;
        ta = val;
        this->event_send(EVENT_VALUE_CHANGED);
      }
    }
    inline int32_t  value()             { create(); return ta.toInt();  }
    inline int32_t  toInt()             { return this->value(); }
    inline void range(int32_t v_min, int32_t v_max) {
      create(); val_min = min(v_min, v_max); val_max = max(v_min, v_max);
      ta = (val = constrain(val, val_min, val_max));
    }
    inline int32_t  value_min()         { return this->val_min; }
    inline int32_t  value_max()         { return this->val_max; }
};

// วิตเจ็ต PID Step สำหรับเป็น ตัว editor ปรับค่า step ของ PID Profile
class GPID_Step : public GContainer {
  public:
    GPID_Step(GWidget&parent=GScreen) : GContainer(parent) { }

    GLabel lb_id;
    GSmartInput smart_input[3];
    int32_t val_max[3] = { 1000, 60*24, 60*24};  // องศา C, นาที, นาที
    String units[3] = { SYMBOL_DEGREE "C", "นาที", "นาที" };
    int8_t _id;

    void create(){
      if(this->isCreated()) return;
      GContainer::create();
      GContainer::size(PCT(95), SIZE_CONTENT);
      GContainer::GRID_CELL(GRID_COL{30, GRID_FR(1), GRID_FR(1),GRID_FR(1)}, GRID_ROW{GRID_CONTENT});
      GContainer::radius(5);
      GContainer::padding(5,5,5,5,5);
      GContainer::border(1, TFT_SILVER);
        lb_id.parent(this);
        lb_id = "1";
        lb_id.font(prasanmit_20, TFT_BLACK);
        lb_id.grid_cell(0,0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
        lb_id.text_align(TEXT_ALIGN_CENTER);
        for(int i=0; i<3 ;i++){
          smart_input[i].parent(this);
          smart_input[i].grid_cell(i+1,0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
          smart_input[i].unit(units[i]);
          smart_input[i].range(0, val_max[i]);
          smart_input[i].onValueChanged(GWIDGET_CB{
            GPID_Step *_pid_step = (GPID_Step*)widget->parent();
            _pid_step->event_send(EVENT_VALUE_CHANGED);
          });
        }
    }

    inline void id(int8_t id)   { create(); lb_id = (_id=id)+1; }
    inline String json_output() { 
      create(); 
      return StringX::printf(R"({"step": %d,"to_temp": %d,"ramp_time": %d,"hold_time": %d})", _id, smart_input[0].toInt(), smart_input[1].toInt(), smart_input[2].toInt());
    }

    inline int8_t  id()           { create(); return _id;                     }
    inline int32_t to_temp()      { create(); return smart_input[0].toInt();  }    // Degree C
    inline int32_t ramp_time()    { create(); return smart_input[1].toInt();  }    // Minutes
    inline int32_t hold_time()    { create(); return smart_input[2].toInt();  }    // Minutes
};

#define COLOR_BODY        TFT_COLOR_HEX(0x121212)
#define COLOR_BOX         TFT_COLOR_HEX(0x1e1e1e)
#define COLOR_BORDER      TFT_COLOR_HEX(0x212121)
#define COLOR_SCALE_FONT  TFT_COLOR_HEX(0x7a7a7a)
#define COLOR_PROGRAM_BG  TFT_COLOR_HEX(0x2d2d2d)
#define COLOR_GREEN       TFT_COLOR_HEX(0x03dac6)

// พื้นที่ส่วนแสดง sv chart
GRect rect_chart_sv_profile;
  GLabel lb_chart_sv_profile_title("SV Graph", rect_chart_sv_profile);
  GPage  page_chart_scroller(rect_chart_sv_profile);    // ไว้ทำ scroll ให้ GChart ที่ใหญ่กว่าได้ scroll ได้
    GChart chart_sv_profile(page_chart_scroller);       
      GLine line_sv_profile(chart_sv_profile);          // เส้น temp ตาม sv profile
        Points sv_profile_points;
      GLine line_temp_guildline(chart_sv_profile);      // เส้นประ guildline สำหรับ to_temp
        Points temp_guildline_points;
    GScale scale_x_chart_sv_profile(page_chart_scroller);
  GScale scale_y_chart_sv_profile(rect_chart_sv_profile);      
  GRect rect_border(rect_chart_sv_profile);             // ไว้ทำกรอบล้อม chart
  GRect rect_invisible_zone_l(rect_chart_sv_profile);   // ไว้บังส่วนเกิน
  GRect rect_invisible_zone_r(rect_chart_sv_profile);   // ไว้บังส่วนเกิน


// วิตเจ็ตกลุ่มสำหรับเป็น PID Profile Editor 16 PID_Steps
GContainer cont;
  GContainer cont_header;
    GLabel lb_header[4];
    String str_header[4] = {"Step", "To Temp(" SYMBOL_DEGREE "C)", "Ramp (นาที)", "Hold (นาที)"};
  GPage page;
  GPID_Step  pid_steps[16];
  int8_t     cur_pid_steps_id=-1;

GLabel lb_myinfo;

int32_t ADJUST_Y = -2;

// สำหรับ update ตัว SV Graph ตาม sv profile ที่กำหนด
void sv_graph_update(){
  int16_t sum_period=0; // minutes
  sv_profile_points.clear();
  Point p;
  // จุดเริ่มต้น สำหรับ เส้น SV Graph (line_sv_profile)
  p.x = map(0, 0, 20*60+0, 0, chart_sv_profile.width());
  if(pid_steps[0].ramp_time() == 0) {
    p.y = map(pid_steps[0].to_temp(), 0, 1000, chart_sv_profile.height(), 0) + ADJUST_Y;
  }else{
    p.y = map(0, 0, 1000, chart_sv_profile.height(), 0) + ADJUST_Y;   // ให้จุดเริ่มต้น เริ่มที่ (0,0)
  }
  sv_profile_points.push_back(p);

  for(int i=0; i < sizeof(pid_steps)/sizeof(pid_steps[0]); i++){
    if(i > 0 && pid_steps[i].ramp_time() == 0 && pid_steps[i].hold_time() == 0) break;
    if(pid_steps[i].ramp_time() != 0) {
      sum_period += pid_steps[i].ramp_time();
      p.x = map(sum_period, 0, 20*60+0, 0, chart_sv_profile.width());
      p.y = map(pid_steps[i].to_temp(), 0, 1000, chart_sv_profile.height(),0) + ADJUST_Y;
      sv_profile_points.push_back(p);
      Serial.printf("[update] ramp : %d, %d  (%d,%d)\n", pid_steps[i].to_temp(), sum_period, p.x, p.y);// p.x, p.y);
    }

    if(pid_steps[i].hold_time() != 0) {
      sum_period += pid_steps[i].hold_time();
      p.x = map(sum_period, 0, 20*60+0, 0, chart_sv_profile.width());
      p.y = map(pid_steps[i].to_temp(), 0, 1000, chart_sv_profile.height(),0) + ADJUST_Y;
      sv_profile_points.push_back(p);
      Serial.printf("[update] hold : %d, %d  (%d,%d)\n", pid_steps[i].to_temp(), sum_period, p.x, p.y); //, p.x, p.y);
    }
  }
  line_sv_profile.setPoints(sv_profile_points);
}

// สำหรับ update ตัว เส้นประ guildline ของ to_temp ที่กำหนดตั้งค่า
void temp_guildline_update(){
  int32_t cur_to_temp = pid_steps[cur_pid_steps_id].to_temp();
  temp_guildline_points.clear();
  Point p;

  // จุดเริ่มต้น สำหรับ เส้น guildline ของ to_temp
  p.x = map(0, 0, 20*60+0, 0, chart_sv_profile.width());
  p.y = map(cur_to_temp, 0, 1000, chart_sv_profile.height(), 0) + ADJUST_Y;
  temp_guildline_points.push_back(p);

  // จุดสิ้นสุด สำหรับ เส้น guildline ของ to_temp
  p.x = map(20*60+0, 0, 20*60+0, 0, chart_sv_profile.width());
  p.y = map(cur_to_temp, 0, 1000, chart_sv_profile.height(), 0) + ADJUST_Y;
  temp_guildline_points.push_back(p);

  line_temp_guildline.setPoints(temp_guildline_points);

  static SoftTimer timer;
  line_temp_guildline.show(true);                               // แสดงเส้น guildline ของ to_temp
  timer.delay(15000,[](){ line_temp_guildline.show(false); });  // 15 วินาที ค่อยให้ซ่อนเส้น guildline ของ to temp
}

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  rect_chart_sv_profile.align(ALIGN_TOP,0,10);
  rect_chart_sv_profile.radius(10);
  rect_chart_sv_profile.color(COLOR_BOX);
  rect_chart_sv_profile.border(2, COLOR_BORDER);
  rect_chart_sv_profile.GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{40, 10, GRID_FR(1)});
  rect_chart_sv_profile.pad(0);
    lb_chart_sv_profile_title.font(prasanmit_20, TFT_WHITE);
    lb_chart_sv_profile_title.grid_cell(0,0, GRID_ALIGN_START, GRID_ALIGN_CENTER,20);

    page_chart_scroller.size(400+20,240+25);
    page_chart_scroller.bg_opa(0);
    page_chart_scroller.scrollbar(SCROLLBAR_MODE_AUTO, true, false);
    page_chart_scroller.grid_cell(0,2,GRID_ALIGN_CENTER, GRID_ALIGN_START,10);
    page_chart_scroller.opa(0, GPART_SCROLLBAR);
    page_chart_scroller.radius(0);
    page_chart_scroller.layout(LAYOUT_NONE);
    page_chart_scroller.padding(0,0,5,15,0);

      chart_sv_profile.size(800,240);
      chart_sv_profile.position(0,0);
      chart_sv_profile.point_count(41);                    // จำนวนจุดที่ plot ในหน้า char
      chart_sv_profile.div_line_count(11,21);              // เส้นแบ่งช่วง เส้นนอน 10+1 เส้น และ เส้นตั้ง 60+1 เส้น
      chart_sv_profile.bg_opa(0);
      chart_sv_profile.line_color(TFT_COLOR_HEX(0x292929));
      chart_sv_profile.clickable(false);
      chart_sv_profile.border(1, TFT_COLOR_HEX(0x292929));
      chart_sv_profile.scrollable(false);

      scale_x_chart_sv_profile.mode(SCALE_MODE_HOR_BOTTOM);
      scale_x_chart_sv_profile.size(chart_sv_profile.width(),25);
      scale_x_chart_sv_profile.align(chart_sv_profile, ALIGN_BOTTOM_LEFT);
      scale_x_chart_sv_profile.thickness(0);
      scale_x_chart_sv_profile.font(prasanmit_20, COLOR_SCALE_FONT);
      scale_x_chart_sv_profile.range(0,20);
      scale_x_chart_sv_profile.tick(21, 2);
      scale_x_chart_sv_profile.label_show(true);
      static const char * x_chart_text[] = {"0", "2hr", "4hr", "6hr", "8hr", "10hr", "12hr", "14hr", "16hr", "18hr", "20hr", NULL};
      scale_x_chart_sv_profile.text_src(x_chart_text);

      line_temp_guildline.thickness(1, TFT_ORANGE);
      line_temp_guildline.opa(80);
      line_temp_guildline.dash(2,2);

      line_sv_profile.thickness(1, COLOR_GREEN);

    rect_border.size(400,240);
    rect_border.bg_opa(0);
    rect_border.border(1, TFT_WHITE);
    rect_border.clickable(false);
    rect_border.grid_cell(0,2,GRID_ALIGN_CENTER, GRID_ALIGN_START, 5);

    rect_invisible_zone_r.size(20,240);
    rect_invisible_zone_r.color(COLOR_BOX);
    rect_invisible_zone_r.grid_cell(0,2, GRID_ALIGN_END, GRID_ALIGN_START, -10-3);

    rect_invisible_zone_l.size(25,240);
    rect_invisible_zone_l.color(COLOR_BOX);
    rect_invisible_zone_l.grid_cell(0,2, GRID_ALIGN_START, GRID_ALIGN_START, 10+8);

    scale_y_chart_sv_profile.mode(SCALE_MODE_VER_LEFT);
    scale_y_chart_sv_profile.size(25,240);
    scale_y_chart_sv_profile.grid_cell(0,2, GRID_ALIGN_START, GRID_ALIGN_START,8);
    scale_y_chart_sv_profile.tick(11, 2);
    scale_y_chart_sv_profile.thickness(0);
    scale_y_chart_sv_profile.font(prasanmit_15, COLOR_SCALE_FONT);
    scale_y_chart_sv_profile.range(0,1000);
    scale_y_chart_sv_profile.label_show(true);

  
  //-------------------------------------------------------------------
  cont.size(PCT(95), 400);
  cont.align(ALIGN_BOTTOM,0,-5);
  cont.GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_FR(1) } );
  cont.padding(5);
  cont.radius(5);
  cont.onValueChanged(GWIDGET_CB{
    
  });
    cont_header.parent(cont);
    cont_header.size(PCT(100), 40);
    cont_header.grid_cell(0,0);
    cont_header.GRID_CELL(GRID_COL{30, GRID_FR(1), GRID_FR(1),GRID_FR(1)}, GRID_ROW{GRID_FR(1)});
    cont_header.padding(5);
    cont_header.color(TFT_PALETTE(TFT_PALETTE_GREEN));
    for(int i=0;i<4; i++){
      lb_header[i].parent(cont_header);
      lb_header[i].grid_cell(i,0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER, 0, -2);
      lb_header[i].text_align(TEXT_ALIGN_CENTER);
      lb_header[i].font(prasanmit_20, TFT_BLACK);
      lb_header[i] = str_header[i];
    }

    page.parent(cont);
    page.grid_cell(0,1,GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    page.layout(LAYOUT_COL_M, 0, 0, 0, 5, 5);
    page.radius(5);
    // page.color(TFT_COLOR_HSV(0,0,40));
      for(int i=0; i< sizeof(pid_steps)/sizeof(pid_steps[0]); i++){
        pid_steps[i].parent(page);
        pid_steps[i].size(PCT(100),SIZE_CONTENT);
        pid_steps[i].id(i);
        pid_steps[i].onValueChanged(GWIDGET_CB{
          int ii = (GPID_Step*)widget-&pid_steps[0];
          cur_pid_steps_id = ii;   // pid_steps ปัจจุบันที่มีการแก้ไข
          // lb_myinfo = pid_steps[ii].json_output();

          if( cur_pid_steps_id == 0 || pid_steps[ii].to_temp() != 0) {
            temp_guildline_update();
          }
          if( cur_pid_steps_id == 0 || pid_steps[ii].ramp_time() != 0 || pid_steps[ii].hold_time() != 0) {
            sv_graph_update();
            cont.event_send(EVENT_VALUE_CHANGED);
          }
        });
        pid_steps[i].onClicked(GWIDGET_CB{
          int ii = (GPID_Step*)widget-&pid_steps[0];
          cur_pid_steps_id = ii;   // pid_steps ปัจจุบันที่มีการแก้ไข
          if( cur_pid_steps_id == 0 || pid_steps[ii].to_temp() != 0) {
            temp_guildline_update();
          }
        });
      }

  // lb_myinfo.align(ALIGN_TOP);

}

void loop() {
  BlynkGO.update();
}
