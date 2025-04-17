/* WinApp : BlynKGO BMI  480x 800  แนวตั้ง */

#include <BlynkGOv5.h>

class GScalePicker : public GContainer {
  public:
    GScalePicker(GWidget &parent=GScreen) : GContainer(parent) {}
      GLabel lb_value;
      GRect rect_scroll;
        GRect rect_scale;
          GScale scale;
      GLine line_marker;
      GLabel lb_unit;
    int32_t _v_min = 75;
    int32_t _v_max = 200;
    double  _v;
    int8_t _tick_px = 4;
    String _scale_title = "ความสูง";
    String _scale_unit = "cm";

    void create(){
      if(GContainer::isCreated()) return;
      GContainer::create();
      GContainer::width(400);
      GContainer::GRID_CELL( GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, 80});
      GContainer::padding(0,0,0,0,10);
      GContainer::bg_opa(0);
      GContainer::font_color(TFT_WHITE);
        lb_value.parent(this);
        lb_value.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

        rect_scroll.parent(this);
        rect_scroll.grid_cell(0,1);
        rect_scroll.scrollable(true);
        rect_scroll.scrollbar(SCROLLBAR_ON, true, false);
        rect_scroll.anim_speed(500, GPART_SCROLLBAR);
        rect_scroll.opa(0, GPART_SCROLLBAR);
        rect_scroll.event_cb( GWIDGET_EVENT_CB {
          if(event != EVENT_SCROLL) return;
          auto _picker = GWIDGET_DATA(GScalePicker);
          double v = _picker->_v_min + _picker->rect_scroll.scroll_x()/(_picker->_tick_px*10.0);
          v = floor(v*10)/10.0;
          if( _picker->_v != v) {
            _picker->lb_value = _picker->_scale_title + " " + String(v,1) + " " + _picker->_scale_unit;
            _picker->_v = v;
            _picker->event_send(EVENT_VALUE_CHANGED);
          }
        }, this);
          rect_scale.parent(rect_scroll);
          rect_scale.size(SIZE_CONTENT, SIZE_CONTENT);
          rect_scale.pad_hor(GContainer::width()/2);
          rect_scale.bg_opa(0);
          rect_scale.position(0,0);
            scale.parent(rect_scale);
            scale.size((_v_max - _v_min) *10 * _tick_px, 80);
            scale.range(_v_min, _v_max);
            scale.mode(SCALE_MODE_BOTTOM, (_v_max-_v_min)*10 +1, 10);
            scale.tick_length(20, 10);
            scale.axis_thickness(0);
            scale.label_show(true);

        line_marker.parent(this);
        line_marker.setPoints(Point(0,0), 16, DIR_VER);  // ลากเส้นตรง ยาว 16px แนวตั้ง
        line_marker.thickness(3, TFT_RED);
        line_marker.grid_cell(0,1,GRID_ALIGN_CENTER, GRID_ALIGN_START);
        
        lb_unit.parent(this);
        lb_unit = _scale_unit;
        lb_unit.font(prasanmit_15, TFT_BLACK);
        lb_unit.grid_cell(0,1, GRID_ALIGN_END, GRID_ALIGN_END,-10,0);

        rect_scroll.scroll_to( value_2_scroll_px(150),0);

        this->onSizeChanged( GWIDGET_CB {
          auto _picker = GWIDGET_CAST(GScalePicker);
          _picker->rect_scale.pad_hor(_picker->width()/2);
        });
      }

      int32_t value_2_scroll_px(float v){
        int32_t tick_num = (int32_t) round( (v-_v_min) *10);    // เปลี่ยน ค่าที่ input เข้ามา เป็นจำนวน tick
        return tick_num*_tick_px;
      }

      inline double value()         { create(); return floor(this->_v*10)/10.0; }
      inline double toDouble()      { return this->value();           }
      inline int    toInt()         { return (int)this->value();      }
      inline float  val_min()       { create(); return this->_v_min;  }
      inline float  val_max()       { create(); return this->_v_max;  }

      inline void value(float v)  {
        create();
        v = constrain(v, this->_v_min, this->_v_max);
        rect_scroll.scroll_to( value_2_scroll_px(v),0);
        if(this->_v != v) {
          this->_v = v;
          this->event_send(EVENT_VALUE_CHANGED);
        }
      }

      inline void range(int32_t val_min, int32_t val_max){
        create();
        _v_min = val_min; this->_v_max = val_max;
        _v = (this->_v_max + this->_v_min)/2;
        scale.range(_v_min, _v_max);
        scale.width((_v_max - _v_min) *10 * _tick_px);
        scale.mode(SCALE_MODE_BOTTOM, (_v_max-_v_min)*10 +1, 10);
        this->value(this->_v);
      }

      inline void tick_px(uint8_t px){
        create();
        _tick_px = constrain(px, 3, 10);
        scale.width((_v_max - _v_min) *10 * _tick_px);
        rect_scroll.event_send(EVENT_SCROLL);
      }

      inline void title_and_unit(String title, String unit){
        create();
        this->_scale_title = title;
        this->_scale_unit = unit;
        this->lb_unit = this->_scale_unit;
        rect_scroll.event_send(EVENT_SCROLL);
      }
      inline void scale_height(int32_t h)                     { create(); GContainer::GRID_CELL( GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, h}); }
      inline void color(color_t c)                            { create(); rect_scroll.color(c); }
      GScalePicker& operator = (const float f)                { create(); this->value(f); return *this; }
};

class GNumpad : public GRect {
  public:
    GNumpad(GWidget&parent =GScreen) : GRect(parent)   {}   
    GLabel lb_title;
    GContainer cont_numpad;
    GTextArea ta_numpad;
    GButtonMatrix bnmx_numpad;
    double _val;
    double _val_prev;
    uint32_t _decimal = 2;

    void create(){
      if(this->isCreated()) return;
      GRect::create();
      // GRect::bg_opa(0);
      GRect::color(TFT_BLACK);
      GRect::padding(0);
      GRect::font_color(TFT_WHITE);

      int32_t sz_min = min(GScreen.width(), GScreen.height());

      if( GScreen.width() != GScreen.height()  && sz_min == GScreen.width()) {
        GRect::GRID_CELL(GRID_COL{GRID_CONTENT}, GRID_ROW{GRID_CONTENT, GRID_CONTENT}, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
        lb_title.parent(this);
        lb_title.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
        lb_title.margin_bottom(20);

        cont_numpad.parent(this);
        cont_numpad.grid_cell(0,1);
      }else{
        lb_title.parent(this);
        lb_title.align(ALIGN_TOP_LEFT,5,5);
        cont_numpad.parent(this);
      }

      cont_numpad.layout(LAYOUT_COL_M,10,10,10,10,10);
      cont_numpad.color(TFT_COLOR_HSV(0,0,10));
      cont_numpad.border(1,TFT_SILVER);

        #define KEY_DEL       SYMBOL_BACKSPACE
        static const char * numpad_map[] = 
        { "1", "2", "3", "\n",
          "4", "5", "6", "\n",
          "7", "8", "9", "\n",
          ".", "0", KEY_DEL, "\n",
          "Cancel", "Enter", ""
        };


        const font_t* ta_font = &BebasNeueBold_num_45;
          if( sz_min == 480) ta_font = &BebasNeueBold_num_45;
          else if( sz_min >= 320) ta_font = &prasanmit_40;
          else if( sz_min >= 240) ta_font = &prasanmit_35;
          else if( sz_min >= 180) ta_font = &prasanmit_30;
          else ta_font = &prasanmit_20;
        
        ta_numpad.parent(cont_numpad);
        ta_numpad.font(ta_font);
        
        int32_t numpad_sz;
        if(sz_min == GScreen.height()) {
          numpad_sz =  sz_min*90/100 - cont_numpad.pad_inner() - cont_numpad.pad_top() - cont_numpad.pad_bottom() - ta_numpad.height();
        }else{
          numpad_sz =  sz_min*90/100 - cont_numpad.pad_left() - cont_numpad.pad_right();
        }
        
        ta_numpad.width( numpad_sz);

        ta_numpad.default_keyboard(false);  // ไม่ใช้ default keyboard
        ta_numpad.text_align(TEXT_ALIGN_RIGHT); 
        ta_numpad.onValueChanged(GWIDGET_CB{              // หาก ta_numpad มีการเปลี่ยนแปลงค่า
          auto _numpad = GWIDGET_DATA(GNumpad);
          _numpad->_val = _numpad->ta_numpad.toDouble();   // ดึงค่า ta_numpad ออกมาเป็น float ให้ price
        },this);
        ta_numpad = 0;                                    // ค่าเริ่มต้นให้ ta_numpad

        bnmx_numpad.parent(cont_numpad);
        bnmx_numpad.size(numpad_sz, numpad_sz);
        bnmx_numpad.map(const_cast<const char**>(numpad_map));
        bnmx_numpad.hookDrawTask(true);
        bnmx_numpad.onDrawTask(GWIDGET_CB{
          auto _p_bnmx_numpad = GWIDGET_CAST(GButtonMatrix);
          if( _p_bnmx_numpad->draw_part() == GPART_ITEMS )             
          {
            int32_t btn_id          = _p_bnmx_numpad->draw_id1();
            int32_t pressed_btn_id  = _p_bnmx_numpad->pressed_button_id();
            bool    pressed         = _p_bnmx_numpad->pressed();
            if(_p_bnmx_numpad->draw_type() == DRAW_TYPE_FILL) {
              auto fill_dsc = _p_bnmx_numpad->draw_fill_dsc();              
              if(btn_id==12)      {
                fill_dsc->color = TFT_COLOR(TFT_COLOR_HSV(  9,72,(pressed && pressed_btn_id == btn_id)? 80 : 100));
              } else if(btn_id==13) { 
                fill_dsc->color = TFT_COLOR(TFT_COLOR_HSV(210,88,(pressed && pressed_btn_id == btn_id)? 80 : 100));
              }
            }
          }
        });
        bnmx_numpad.onValueChanged(GWIDGET_CB{
          auto _numpad = GWIDGET_DATA(GNumpad);
          String selected_button   = _numpad->bnmx_numpad.selected_button();
          String current_text      = _numpad->ta_numpad.toString();
        
          if (selected_button == "Enter") {
            _numpad->show(false);
            _numpad->event_send(EVENT_VALUE_CHANGED);
          } else if (selected_button == "Cancel") {
            _numpad->_val = _numpad->_val_prev;
            _numpad->show(false);
          } else if (selected_button == KEY_DEL) {
            _numpad->ta_numpad.backspace();
            if (_numpad->ta_numpad.length() == 0) {
              _numpad->ta_numpad = 0;
            }
          } else if (selected_button == ".") {
            if (_numpad->_decimal == 0) return;                     // ไม่ให้ใช้จุดทศนิยมถ้า _decimal == 0
            if (current_text.indexOf('.') != -1) return;            // ไม่ให้ใส่ . ซ้ำ
            _numpad->ta_numpad += ".";
        
          } else {
            // เช็คจำนวนตำแหน่งทศนิยม ถ้ามีจุดแล้ว และ _decimal > 0
            int dot_index = current_text.indexOf('.');
            if (dot_index != -1 && _numpad->_decimal > 0) {
              int digits_after_dot = current_text.length() - dot_index - 1;
              if (digits_after_dot >= _numpad->_decimal) {
                return; // เกินตำแหน่งทศนิยม
              }
            }
            if (current_text == "0") {
              _numpad->ta_numpad = selected_button;
            } else {
              _numpad->ta_numpad += selected_button;
            }
          }
        }, this);        
    }
    inline float toDouble()               { create(); return this->_val;          }
    inline float toFloat()                { create(); return this->_val;          }
    inline int   toInt()                  { create(); return (int) this->_val;    }
    GNumpad& operator = (const double f)  { this->value(f);  return *this;        }
    inline void value(double v)           { create(); this->_val = v; this->_val_prev = v; this->ta_numpad = String(v,_decimal); }
    inline void decimal(uint32_t d)       { create(); this->_decimal = d; this->value(this->_val); }
    inline void clear()                   { create(); ta_numpad = "0"; _val = 0;  } 
    inline void title(String t)           { create(); lb_title = t;               }
};

// ช่วงค่าของ BMI และสีที่ใช้
struct BMI_Level {
  float upper_limit;
  String label;
  color_t color;
};

// ตารางข้อมูล 10 ช่วง
BMI_Level bmi_levels[10] = {
  {16.0,  "น้ำหนักต่ำ ระดับ 3",  TFT_COLOR_HSV(240,100,75)}, // น้ำเงินเข้มมากกก (Midnight Blue)
  {17.0,  "น้ำหนักต่ำ ระดับ 2",  TFT_COLOR_HSV(220,100,80)}, // ฟ้าเข้ม
  {17.75, "น้ำหนักต่ำ ระดับ 1",  TFT_COLOR_HSV(200,100,85)}, // ฟ้ากลาง
  {18.5,  "น้ำหนักต่ำกว่าเกณฑ์", TFT_COLOR_HSV(180,100,85)}, // ฟ้าคราม
  {23,  "น้ำหนัก สมส่วน",      TFT_GREEN},
  {25,  "น้ำหนัก เริ่มอ้วน",     TFT_PALETTE_LIGHTEN(TFT_PALETTE_LIME,1)}, //TFT_YELLOW}, // เหลือง
  {30.0,  "น้ำหนักสูงกว่าเกณฑ์", TFT_COLOR_HEX(0xFF9800)}, // ส้มเข้ม
  {35.0,  "อ้วน ระดับ 1",     TFT_COLOR_HEX(0xF44336)}, // แดงสด
  {40.0,  "อ้วน ระดับ 2",     TFT_COLOR_HEX(0xB71C1C)}, // แดงเข้มมากๆ
  {50.0,  "อ้วน ระดับ 3",     TFT_COLOR_HEX(0x7A0000)}  // แดงเข้มที่สุด
};


namespace BMI {
  int calculateAngleFromBMI(float bmi); // ฟังก์ชันสำหรับคำนวณมุมจากค่า BMI
  uint16_t getColorFromBMI(float bmi);
  String getWarningFromBMI(float bmi);
}

GCircle BMI_Gauge;
  GScale scale_bmi(BMI_Gauge);
    int32_t scale_thick=40;
    int32_t scale_R = 150;
    int32_t scale_r = scale_R - scale_thick;
  GLabel lb_weight(BMI_Gauge);
  GLabel lb_weight_unit(BMI_Gauge);
  GLabel lb_bmi(BMI_Gauge);
  GLabel lb_bmi_info(BMI_Gauge);
  GLabel lb_weight_warning(BMI_Gauge);
  GArc arc_frame_decor(BMI_Gauge);
  GCircle bmi_indic(BMI_Gauge);

Point BMI_Gauge_center = Point(240, 250);

GContainer cont_picker;
  GScalePicker weight_picker(cont_picker);
  GScalePicker height_picker(cont_picker);

GNumpad numpad;

double bmi;

void update_BMI(){
  bmi = weight_picker.toDouble() / pow(height_picker.toDouble()/100.0, 2);
  Serial.printf("[BMI] %.2f  ( w %.1f; h %.2f )\n", bmi, weight_picker.toDouble(), height_picker.toDouble());

  int angle_from_bmi = BMI::calculateAngleFromBMI(bmi);  // bmi_value คือค่าของ BMI ที่ได้
  color_t color_from_bmi = BMI::getColorFromBMI(bmi);
  bmi_indic.center(Point(scale_R - scale_thick / 2, angle_from_bmi/10, 135, BMI_Gauge.centerpoint(), CLOCKWISE)); // วาง bmi_indic ตามมุมที่คำนวณได้
  
  lb_weight = String(weight_picker.toDouble(),1);
  lb_weight.font(BebasNeueBold_num_45, TFT_WHITE);
  lb_weight.align(ALIGN_CENTER,0,-30);
  lb_weight_unit = "กก.";
  lb_weight_unit.font(prasanmit_20, TFT_SILVER);
  lb_weight_unit.align(lb_weight, ALIGN_RIGHT_BOTTOM, 5, - (lb_weight.font_base_line()- lb_weight_unit.font_base_line())); 
  lb_bmi= String(bmi,2)+ " BMI";
  lb_bmi.font(prasanmit_35, TFT_COLOR_HSV(0,0,60));
  lb_bmi.align(lb_weight, ALIGN_BOTTOM,0,0);

  lb_weight_warning = BMI::getWarningFromBMI(bmi);
  lb_weight_warning.font(prasanmit_20, (bmi<17 || bmi>=35) ? TFT_WHITE : TFT_BLACK);
  lb_weight_warning.align(lb_bmi, ALIGN_BOTTOM,0,5);
  lb_weight_warning.bg_color(color_from_bmi);
  lb_weight_warning.bg_opa(255);                    // ให้วาดสีพื้นหลังเห็นได้ด้วย
  lb_weight_warning.radius(5);
  lb_weight_warning.pad_ver(2);                     // เพิ่มระยะช่องไฟบนล่าง 2px
  lb_weight_warning.pad_hor(10);                    // เพิ่มระยะช่องซ้ายขวา 10px
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(ROTATION_VERTICAL);
#if defined(_WIN32)
  BlynkGO.window_title("BlynkGO : ดัชนีมวลกาย BMI");
#endif


  BMI_Gauge.size(2*scale_R, 2*scale_R);
  BMI_Gauge.color(TFT_BLACK);
  BMI_Gauge.center(BMI_Gauge_center);
  BMI_Gauge.onClicked(GWIDGET_CB{
    weight_picker.lb_value.event_send(EVENT_CLICKED);
  });

  scale_bmi.size( 2*scale_R, 2*scale_R);
  scale_bmi.range(0, 2700);  // หน่วยเต็มที่แทน 270 องศา
  scale_bmi.angle_offset(135);
  scale_bmi.angle_range(270);
  scale_bmi.mode(SCALE_MODE_ROUND);
  scale_bmi.tick_opa(0);
  scale_bmi.showLabel(false);


  int angle_cursor = 0;

  // ===== โซนฟ้า (4 ช่วง) =====
  for (int i = 0; i < 4; i++) {
    GScaleSection* section = scale_bmi.addSection();
    section->range(angle_cursor, angle_cursor + 200);
    section->thickness(scale_thick, bmi_levels[i].color);
    angle_cursor += 200;
  }

  // ===== สีเขียว (สมส่วน) =====
  {
    GScaleSection* section = scale_bmi.addSection();
    section->range(angle_cursor, angle_cursor + 900);
    section->thickness(scale_thick, bmi_levels[4].color);
    angle_cursor += 900;
  }

  // ===== สีเหลือง (เริ่มอ้วน) =====
  {
    GScaleSection* section = scale_bmi.addSection();
    section->range(angle_cursor, angle_cursor + 200);
    section->thickness(scale_thick, bmi_levels[5].color);
    angle_cursor += 200;
  }

  // ===== โซนแดง-ส้ม (4 ช่วง) =====
  for (int i = 6; i < 10; i++) {
    GScaleSection* section = scale_bmi.addSection();
    section->range(angle_cursor, angle_cursor + 200);
    section->thickness(scale_thick, bmi_levels[i].color);
    angle_cursor += 200;
  }

  int32_t arc_R = scale_r + scale_thick*25/100 ;
  arc_frame_decor.size( 2*arc_R, 2*arc_R);
  arc_frame_decor.color(TFT_BLACK);
  arc_frame_decor.thickness(5);
  arc_frame_decor.angle_offset(135);
  arc_frame_decor.bg_angle(0, 270);
  arc_frame_decor.angle(0, 270);
  arc_frame_decor.bg_enable(false);
  arc_frame_decor = 100;

  bmi_indic.size(10,10);
  bmi_indic.color(TFT_WHITE);
  bmi_indic.border(1,TFT_BLACK);
  bmi_indic.center(Point(scale_R-scale_thick/2, 0, 135, BMI_Gauge.centerpoint(), CLOCKWISE));  // ใช้มุมที่คำนวณจาก BMI

  cont_picker.bg_opa(0);
  cont_picker.width(PCT(100));
  cont_picker.layout(LAYOUT_COL_M);
  cont_picker.padding(0,0,0,0,30);
  cont_picker.align(BMI_Gauge, ALIGN_BOTTOM,0,50);

    weight_picker.width(PCT(80));
    weight_picker.scale_height(60);
    weight_picker.range(20,150);
    weight_picker.tick_px(10);
    weight_picker.title_and_unit("น้ำหนัก", "กก.");
    weight_picker.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_CYAN,3));
    weight_picker.lb_value.clickable(true);
    weight_picker.lb_value.onClicked(GWIDGET_CB{
      numpad.title("น้ำหนัก (กก.)");
      numpad.decimal(1);
      numpad.show(true);
      numpad.onValueChanged(GWIDGET_CB{
        weight_picker = numpad.toDouble();
      });
      numpad = weight_picker.toDouble();
    });
    weight_picker.onValueChanged(GWIDGET_CB{
      update_BMI();
    });
    weight_picker = 60;

    height_picker.width(PCT(80));
    height_picker.scale_height(60);
    height_picker.range(40,220);
    height_picker.tick_px(4);
    height_picker.title_and_unit("ความสูง", "cm");
    height_picker.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_AMBER,3));
    height_picker.lb_value.clickable(true);
    height_picker.lb_value.onClicked(GWIDGET_CB{
      numpad.title("ความสูง (cm)");
      numpad.decimal(1);
      numpad.show(true);
      numpad.onValueChanged(GWIDGET_CB{
        height_picker = numpad.toDouble();
      });
      numpad = height_picker.toDouble();
    });
    height_picker.onValueChanged(GWIDGET_CB{
      update_BMI();
    });
    height_picker = 165;

    update_BMI();
}

void loop(){
  BlynkGO.update();
}

// ฟังก์ชันสำหรับคำนวณมุมจากค่า BMI
int BMI::calculateAngleFromBMI(float bmi) {
  int angle_cursor = 0;
  int angle = 0;

  if (bmi < bmi_levels[0].upper_limit) {
    angle = map(bmi * 100, 0, bmi_levels[0].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[1].upper_limit) {
    angle_cursor += 200;
    angle = map(bmi * 100, bmi_levels[0].upper_limit * 100, bmi_levels[1].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[2].upper_limit) {
    angle_cursor += 400;
    angle = map(bmi * 100, bmi_levels[1].upper_limit * 100, bmi_levels[2].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[3].upper_limit) {
    angle_cursor += 600;
    angle = map(bmi * 100, bmi_levels[2].upper_limit * 100, bmi_levels[3].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[4].upper_limit) {
    angle_cursor += 800;
    angle = map(bmi * 100, bmi_levels[3].upper_limit * 100, bmi_levels[4].upper_limit * 100, angle_cursor, angle_cursor + 900);
  }
  else if (bmi < bmi_levels[5].upper_limit) {
    angle_cursor += 1700;
    angle = map(bmi * 100, bmi_levels[4].upper_limit * 100, bmi_levels[5].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[6].upper_limit) {
    angle_cursor += 1900;
    angle = map(bmi * 100, bmi_levels[5].upper_limit * 100, bmi_levels[6].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[7].upper_limit) {
    angle_cursor += 2100;
    angle = map(bmi * 100, bmi_levels[6].upper_limit * 100, bmi_levels[7].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else if (bmi < bmi_levels[8].upper_limit) {
    angle_cursor += 2300;
    angle = map(bmi * 100, bmi_levels[7].upper_limit * 100, bmi_levels[8].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  else {  // BMI >= 40.0
    angle_cursor += 2500;
    angle = map(bmi * 100, bmi_levels[8].upper_limit * 100, bmi_levels[9].upper_limit * 100, angle_cursor, angle_cursor + 200);
  }
  angle = constrain(angle, 0, 2700);
  return angle;
}

uint16_t BMI::getColorFromBMI(float bmi) {
  for (int i = 0; i < 9; i++) {
    if (bmi < bmi_levels[i].upper_limit) {
      return bmi_levels[i].color;
    }
  }
  // ถ้า BMI ≥ 50
  return bmi_levels[9].color;
}

String BMI::getWarningFromBMI(float bmi) {
  for (int i = 0; i < 9; i++) {
    if (bmi < bmi_levels[i].upper_limit) {
      return bmi_levels[i].label;
    }
  }
  // ถ้า BMI ≥ 50
  return bmi_levels[9].label;
}
