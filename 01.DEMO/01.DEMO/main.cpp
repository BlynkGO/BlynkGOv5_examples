#include <BlynkGOv5.h>

FONT_DECLARE(material_icon_30);
FONT_DECLARE(material_icon_40);
FONT_DECLARE(BebasNeueBold_num_45);
FONT_DECLARE(prasanmit_15);
FONT_DECLARE(prasanmit_18);
FONT_DECLARE(AftikaBold_50);

IMAGE_DECLARE(img_background_800x480);
IMAGE_DECLARE(img_hvac_800x480);

#define SYMBOL_DASHBOARD_OUTLINE     "\uE871"
#define SYMBOL_OVERVIEW              "\uE4A7"
#define SYMBOL_MONITORING            "\uF190"
#define SYMBOL_FOLDER                "\uE2C7"
#define SYMBOL_GROUP                 "\uF233"
#define SYMBOL_MOVE_UP               "\uEB64"
#define SYMBOL_FLAG                  "\uE153"
#define SYMBOL_NOTIFICATION_ACTIVE   "\uE7F7"
#define SYMBOL_ACCOUNT_CIRCLE        "\uE853"
#define SYMBOL_SETTINGS              "\uE8B8"
#define SYMBOL_LOGOUT                "\uE9BA"
#define SYMBOL_DARK_MODE             "\uE51C"
#define SYMBOL_POWER_SETTINGS_NEW    "\ue8ac"
#define SYMBOL_POWER_SETTINGS_CIRCLE "\uF418"
#define SYMBOL_PLUG                  "\uE63C"
#define SYMBOL_HOME                  "\ue88a"
#define SYMBOL_INFO                  "\ue88e"
#define SYMBOL_BACKSPACE             "\ue14a"
#define SYMBOL_HVAC                  "\uf10e"
#define SYMBOL_HVAC                  "\uf10e"
#define SYMBOL_TRACKPAD_INPUT        "\uf4c7"
#define SYMBOL_SUNNY                 "\ue81a"
#define SYMBOL_AC_UNIT               "\ueb3b"

#define RECT_CELL_COLOR             TFT_COLOR_HEX(0x13161b)
#define PASSCODE                    135790

class GGaugeGRAFANA : GScale {      // สร้าง class แตกมาจากวิตเจ็ต GScale
  public:
    GGaugeGRAFANA(GWidget&parent= GScreen) : GScale(parent), _ranges{0, 40, 60, 80, 100}, _sect_colors{ TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED} {}

    void create(){
      if(GScale::isCreated()) return;
      GScale::create();
      GScale::size(200,200); 
      GScale::range(0,100);
      GScale::angle_offset(170);
      GScale::angle_range(200);
      GScale::mode(SCALE_MODE_ROUND);
      GScale::tick_opa(0); 
      GScale::showLabel(false);

      for(int i=0; i<4; i++) {
        GScaleSection* section = GScale::addSection();
        section->range(this->_ranges[i],this->_ranges[i+1]);
        section->thickness(5, this->_sect_colors[i]);
      }

      arc.parent(this);
      arc.size(PCT(90), PCT(90));
      arc.thickness( 0.1* GScale::width(), TFT_GREEN);
      arc.angle_offset(GScale::angle_offset());
      arc.angle_range(GScale::angle_range());
      arc.bg_enable(true);
      arc.bg_color(TFT_COLOR_HEX(0x2d2f34));
      arc.range(0, 100);
      arc.clickable(false);

      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value = 0;

      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_SILVER);
      lb_title = "Title";

      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,0,0);

      this->onSizeChanged(GWIDGET_CB{
        GGaugeGRAFANA* _gauge = (GGaugeGRAFANA*) widget;
        _gauge->refresh_size();
      });

      this->refresh_size();
    }

    void title(String title){
      create();
      lb_title = title;
      this->refresh_size();
    }
    String title(){ create(); return lb_title.c_str(); }

    void value(float value){  // value 0-100
      create();
      _value = value;
      lb_value = String(_value,(unsigned int)_decimal);
      arc = (int32_t) value;
      this->refresh_size();
    }
    
    float value()             { create(); return _value;  }

    void decimal(uint8_t d)   { create(); _decimal = d; this->value(_value); }
    uint8_t decimal()         { create(); return _decimal; }

    void range(int32_t min_val, int32_t max_val)  {
      create();
      GScale::range(min_val, max_val);
      arc.range(min_val, max_val);
      this->value(this->_value);
    }

    void section(const std::vector<uint32_t>& ranges, const std::vector<color_t>&sect_colors)
    {
      create();
      GScale::clearSection();
      this->_ranges = ranges;
      this->_sect_colors = sect_colors;
      int8_t section_num = min( this->_ranges.size() - 1, this->_sect_colors.size());
      for(int i=0; i< section_num; i++){
        GScaleSection* section = GScale::addSection();
        section->range(ranges[i], ranges[i+1]);
        section->thickness(5, sect_colors[i]);
      }
      this->value(this->_value);
    }

    void operator=(float f) {
      create();
      this->value(f);
    }

    void font(const font_t& font)   { create(); lb_value.font(font); lb_title.align(lb_value, ALIGN_TOP,0,0); }

    void refresh_size(){
      if(!this->isCreated()) return;
      arc.size(PCT(90), PCT(90));
      arc.thickness( 0.1* this->width(), TFT_GREEN);
      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,0,0);
      int8_t section_num = min( _ranges.size() - 1, _sect_colors.size());
      for(int i=1; i<=section_num; i++){
        if( _value <= _ranges[i]) {
          arc.color(_sect_colors[i-1]) ;
          break;
        }
      }
    }

    GArc arc;
    GLabel lb_value;
    GLabel lb_title;

    using GObject::size;
    using GObject::parent;
    using GObject::align;
    using GObject::width;
    using GObject::height;

  private:
    std::vector<uint32_t> _ranges;
    std::vector<color_t>  _sect_colors;    
    float _value;
    uint8_t _decimal = 2;
};


class GSwitchMod : public GSwitch {
  public:
    GSwitchMod(GWidget&parent=GScreen):GSwitch(parent) {}
    void create(){
      if(GSwitch::isCreated()) return;
      GSwitch::create();
      GSwitch::size(90, 40);
      GSwitch::color(TFT_WHITE,(gstate_t)(GPART_INDICATOR | GSTATE_ON ));
      GSwitch::color(TFT_WHITE,(gstate_t)(GPART_INDICATOR | GSTATE_OFF ));
      GSwitch::color(TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW,2) , (gstate_t)(GPART_KNOB | GSTATE_ON ));
      GSwitch::color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,2), (gstate_t)(GPART_KNOB | GSTATE_OFF));
      GSwitch::shadow(10,2, TFT_BLACK);
      GSwitch::shadow_opa(120);
      GSwitch::shadow_offset(2,2);
      GSwitch::outline_opa(0);

      GSwitch::hookDrawTask(true);          /* ให้สามารถ hook ดัก ขณะกราฟิก กำลัง draw ระดับล่าง ได้ด้วย */
      GSwitch::onDrawTask(GWIDGET_CB{      /* เมื่อ ขณะกราฟิก กำลัง draw วิตเจ็ตระดับล่าง */
        GSwitchMod * p_sw = (GSwitchMod*) widget;
        draw_label_dsc_t  label_dsc;
        area_t            label_area;
        draw_label_dsc_init(&label_dsc);

        /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_KNOB */
        if(DRAW_PART == DRAW_PART_KNOB) {
          label_dsc.font = &material_icon_30;
          label_dsc.color = TFT_COLOR(TFT_WHITE);
          label_dsc.text = SYMBOL_PLUG;
          text_get_area(&label_area, &label_dsc);
          area_align(DRAW_AREA , &label_area, ALIGN_CENTER, 0, 0);
          draw_label(DRAW_LAYER, &label_dsc , &label_area);
        }
        else
        /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_INDICATOR และ sw มีสถานะ OFF */
        if(DRAW_PART == DRAW_PART_INDICATOR && p_sw->isON()){
          label_dsc.font = &prasanmit_25;
          label_dsc.color = TFT_COLOR(TFT_BLACK);
          label_dsc.text = "เปิด";
          text_get_area(&label_area, &label_dsc);
          area_align(DRAW_AREA , &label_area, ALIGN_LEFT_MID, 15, 0);
          draw_label(DRAW_LAYER, &label_dsc , &label_area);
        }
        else
        /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_MAIN และ sw มีสถานะ OFF */
        if(DRAW_PART == DRAW_PART_MAIN && p_sw->isOFF()){
          label_dsc.font = &prasanmit_25;
          label_dsc.color = TFT_COLOR(TFT_BLACK);
          label_dsc.text = "ปิด"; 
          text_get_area(&label_area, label_dsc.text, label_dsc.font);
          area_align(DRAW_AREA , &label_area, ALIGN_RIGHT_MID, -18, 0);
          draw_label(DRAW_LAYER, &label_dsc , &label_area);
        }
      });
    }
};

class GHomeSwitch : public GRect {
  public:
  GHomeSwitch(GWidget&parent=GScreen) : GRect(parent)   { }

  GLabel lb_power_icon;
  GLabel lb_name;
  GLabel lb_sub_name;
  GSwitchMod sw;

  void create(){
    if(GRect::isCreated()) return;
    GRect::create();
    GRect::size(300, SIZE_CONTENT);
    GRect::GRID_CELL(GRID_COL{ 45, GRID_FR(1), 90}, GRID_ROW { 35, 25 });
    GRect::padding(5,12,20,20,0,3);
    GRect::color(TFT_WHITE);
    GRect::radius(10);
    GRect::shadow(8, 2, TFT_BLACK);
    GRect::shadow_opa(60);
    GRect::shadow_offset(1,1);
      lb_power_icon.parent(this);
      lb_power_icon = SYMBOL_POWER_SETTINGS_NEW;
      lb_power_icon.font(material_icon_40);
      lb_power_icon.grid_cell(0, 0, 1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER,0,2);

      lb_name.parent(this);
      lb_name = "ปั๊มน้ำ";
      lb_name.font(prasanmit_20, TFT_BLACK);
      lb_name.grid_cell(1,0, GRID_ALIGN_START, GRID_ALIGN_END,0,5);

      lb_sub_name.create();
      lb_sub_name.parent(this);
      lb_sub_name = "SweetHome";
      lb_sub_name.font(prasanmit_18, TFT_SILVER);
      lb_sub_name.grid_cell(1,1, GRID_ALIGN_START, GRID_ALIGN_START,0,-5);

      sw.parent(this);
      sw.grid_cell(2,0, 1, 2, GRID_ALIGN_START, GRID_ALIGN_CENTER);
      sw.onValueChanged(GWIDGET_CB{
        GHomeSwitch* home_sw = (GHomeSwitch*) widget->parent();
        color_t color_on  = home_sw->sw.color((gstate_t) (GPART_KNOB| GSTATE_ON));
        color_t color_off = home_sw->sw.color((gstate_t) (GPART_KNOB| GSTATE_OFF));
        home_sw->lb_power_icon.color( home_sw->sw.isON()? color_on :color_off );
        home_sw->event_send(EVENT_VALUE_CHANGED);
      });
  }

  inline void ON()                { create(); sw.ON();  sw.event_send(EVENT_VALUE_CHANGED); }
  inline void OFF()               { create(); sw.OFF(); sw.event_send(EVENT_VALUE_CHANGED); }
  inline bool isON()              { create(); return sw.isON();                             }
  inline bool isOFF()             { create(); return sw.isOFF();                            }
  inline void name(String v)      { create(); lb_name     = v;                              }
  inline String name()            { create(); return lb_name.text();                        }
  inline void subname(String v)   { create(); lb_sub_name = v;                              }
  inline String subname()         { create(); return lb_sub_name.text();                    }
  inline void color(color_t c)    { create(); sw.color(c , (gstate_t)(GPART_KNOB | GSTATE_ON )); sw.event_send(EVENT_VALUE_CHANGED); }

  using GObject::color;
};

class GValue : public GRect {
  public:
  GValue(GWidget&parent=GScreen) : GRect(parent)   { }

  GLabel lb_value;
  GLabel lb_unit;

  void create(){
    if(GRect::isCreated()) return;
    GRect::create();
    GRect::size(45, 25);
    GRect::radius(7);
    GRect::color(TFT_COLOR_HEX(0x2b383f));
    GRect::padding_inner(2);
    GRect::clickable(false);
      lb_value.parent(this);
      lb_value = 0;
      lb_value.font(prasanmit_20, TFT_WHITE);
      lb_value.align(ALIGN_CENTER,-4);
      lb_unit.parent(this);
      lb_unit = SYMBOL_DEGREE "C";
      lb_unit.font(prasanmit_15, TFT_WHITE);
      lb_unit.align(lb_value, ALIGN_RIGHT, this->padding_inner(), 2);
  }

  void value(int32_t v)               { create(); lb_value = v; lb_unit.align(lb_value, ALIGN_RIGHT, this->padding_inner(), 2);                }
  void value(int32_t v, String unit)  { create(); lb_value = v; lb_unit =unit; lb_unit.align(lb_value, ALIGN_RIGHT,this->padding_inner(), 2);  }
  int32_t value()                     { create(); return lb_value.toInt();    }  
  GValue&  operator=(int32_t v)       { this->value(v); return *this;         }
};

class GArcValue : public GArc {
  public:
  GArcValue(GWidget&parent=GScreen) : GArc(parent)   { }

  GLabel lb_value;
  GLabel lb_unit;

  void create(){
    if(GArc::isCreated()) return;
    GArc::create();
    GArc::size(50,50);
    GArc::thickness(3, TFT_GREEN);
    GArc::angle_offset(270);
    GArc::angle_range(360);
    GArc::range(0, 100);
    GArc::bg_enable(true);
    GArc::bg_color(TFT_COLOR_HEX(0x3a4044));
    GArc::value(50);
      lb_value.parent(this);
      lb_value = 0;
      lb_value.font(prasanmit_20, TFT_WHITE);
      lb_value.align(ALIGN_CENTER,0,-5);
      lb_unit.parent(this);
      lb_unit = "%";
      lb_unit.font(prasanmit_15, TFT_SILVER);
      lb_unit.align(lb_value, ALIGN_BOTTOM,0,-10);
  }

  void value(int32_t v)               { create(); GArc::value(v); lb_value = v; lb_unit.align(lb_value, ALIGN_BOTTOM,0,-10);                }
  void value(int32_t v, String unit)  { create(); GArc::value(v); lb_value = v; lb_unit =unit; lb_unit.align(lb_value, ALIGN_BOTTOM,0,-10);  }
  int32_t value()                     { create(); return lb_value.toInt();    }  
  GArcValue&  operator=(int32_t v)    { this->value(v); return *this;         }
};



GContainer cont_frame;
  GRect rect_gauge[4];
    GGaugeGRAFANA gauge[4];
    String gauge_title[] = {"อุณหภูมิ", "ความชื้น", "ความสว่าง", "กดอากาศ"};
  
    GHomeSwitch sw_homeswitch[4];
    String sw_homeswitch_name[] = {"ปั๊มน้ำ", "แอร์", "พัดลม", "รดน้ำ"};
    color_t sw_hoemswitch_color[] = { 
      TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW,2), 
      TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,2),
      TFT_PALETTE_DARKEN(TFT_PALETTE_RED,2),
      TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2)
    };
  GRect rect_datetime(cont_frame);
    GLabel lb_time(rect_datetime);
    GLabel lb_date(rect_datetime);

  GRect rect_chart_circle(cont_frame);
    GScale scale_chart_circle(rect_chart_circle);
  
  GRect rect_water_tank(cont_frame);
    GBar bar_water_tank(rect_water_tank);
    GScale scale_water_tank(rect_water_tank);

  GRect rect_slider(cont_frame);
    GSlider slider(rect_slider);
    GLabel  lb_slider(rect_slider);

  GRect rect_slider2(cont_frame);
    GSlider slider2(rect_slider2);
    GLabel  lb_slider2(rect_slider2);

  GLabel lb_cardgame_menu(cont_frame);

  GWiFiManager wifi_manager(cont_frame);
    GRect  rect_wifi_manager;
    GPasscodeLock passcodelock;

  GLabel lb_config(cont_frame);
  
  GLabel lb_hvac(cont_frame);

  GLabel lb_folder(cont_frame);

  GLabel lb_touch(cont_frame);
  
  GLabel lb_info(cont_frame);

//----------------------------------  Touch Tester -------------------------

GImage image_touch_tester(img_background_800x480);
  GButton btn_touch_tester_back(image_touch_tester);
  GCircle dot_touch(image_touch_tester);
  GLabel lb_touch_info(image_touch_tester);

//----------------------------------  Touch Tester -------------------------

GImage image_hvac_background(img_hvac_800x480);
  GLabel lb_hvac_title(image_hvac_background);
  GLabel lb_have_title_detail(image_hvac_background);
  GLabel lb_hvac_status_info(image_hvac_background);
  GLabel lb_hvac_status(image_hvac_background);
  GLabel lb_hvac_power_info(image_hvac_background);
  GSwitch sw_hvac_power(image_hvac_background);
  GLabel lb_hvac_mode_info(image_hvac_background);
  GButton btn_hvac_mode_winter(image_hvac_background);
  GLabel lb_hvac_mode_winter(image_hvac_background);
  GButton btn_hvac_mode_summer(image_hvac_background);
  GLabel lb_hvac_mode_summer(image_hvac_background);

  //----------------------------------
  GLabel lb_hvac_outdoor_temperature(image_hvac_background);
  GValue v_hvac_outdoor_temperature(image_hvac_background);

  GLabel lb_hvac_indoor_temperature(image_hvac_background);
  GValue v_hvac_indoor_temperature(image_hvac_background);

  GLabel lb_hvac_filter_pressure_drop1(image_hvac_background);
  GArcValue   arcv_hvac_filter_pressure_drop1(image_hvac_background);

  GLabel lb_hvac_filter_pressure_drop2(image_hvac_background);
  GArcValue   arcv_hvac_filter_pressure_drop2(image_hvac_background);

  GLabel lb_hvac_engine_output1(image_hvac_background);
  GArcValue   arcv_hvac_engine_output1(image_hvac_background);

  GLabel lb_hvac_engine_output2(image_hvac_background);
  GArcValue   arcv_hvac_engine_output2(image_hvac_background);

  GLabel lb_hvac_exhault_temperature(image_hvac_background);
  GValue v_hvac_exhault_temperature(image_hvac_background);

  GLabel lb_hvac_water_heater_output(image_hvac_background);
  GArcValue arcv_hvac_water_heater_output(image_hvac_background);

  GLabel lb_hvac_return_water_temperature(image_hvac_background);
  GValue v_hvac_return_water_temperature(image_hvac_background);

  GLabel lb_hvac_humidifier_output(image_hvac_background);
  GArcValue arcv_hvac_humidifier_output(image_hvac_background);

//----------------------------------  CARD MENU -------------------------

IMAGE_DECLARE(img_cards_00);
IMAGE_DECLARE(img_cards_01);
IMAGE_DECLARE(img_cards_02);
IMAGE_DECLARE(img_cards_03);
IMAGE_DECLARE(img_cards_04);
IMAGE_DECLARE(img_cards_05);
IMAGE_DECLARE(img_cards_06);
IMAGE_DECLARE(img_cards_07);
IMAGE_DECLARE(img_cards_08);
IMAGE_DECLARE(img_cards_09);
IMAGE_DECLARE(img_cards_10);
IMAGE_DECLARE(img_cards_11);


GRect rect_cardgame;
  GButton btn_cardgame_back(rect_cardgame);

  GLabel lb_item_name(rect_cardgame);

  GRect menu(rect_cardgame);
    GContainer cont_item(menu);
      size_t item_num;
      GImage item[] = {
        {img_cards_00}, {img_cards_01}, {img_cards_02}, {img_cards_03}, 
        {img_cards_04}, {img_cards_05}, {img_cards_06}, {img_cards_07}, 
        {img_cards_08}, {img_cards_09}, {img_cards_10}, {img_cards_11}, 
      };
      String item_name[] = {
        "ฆ้อนมิโยนีร์ เทพธอร์", "สายฟ้า เทพซีอุส", "กุญแจอมตะ เทพโอซีริส", "คฑาโกมุท พระศิวะ",
        "ขวาน พระพิคเณศ", "ศรีศูล เทพโพซีดอน", "มีดคาตานะ แห่งอะมะคุนิ", "คันธนู เทพคิวปิด",
        "งูไฟซิวยูโคอาตัล เทพสงคราม", "ดาวกระจาย ซูริเค็น", "ลูกดอกพิษ ซาร์บาคาน", "พระแสงหอก กุงนีร์"
      };
    GRect rect_selector(menu);

  GButton btn_right(rect_cardgame);
  GButton btn_left(rect_cardgame);
  int cur_id;

void anim_item(int id){
  static GAnimation anim_x;
  static GAnimation anim_zoom;
  uint32_t anim_time = 300; /* ระยะเวลาเล่น animation (ms) */
  static uint8_t zm  = 70;  /* ภาพที่่จะซูมย่อเหลือกี่ %        */

  cur_id = constrain(id,0,item_num-1);

  int32_t start_x = cont_item.posX();
  int32_t end_x   = menu.width()/2 - item[cur_id].center().x - (menu.pad_left() + cont_item.pad_left());
  anim_x.init(cont_item, ANIM_X, start_x, end_x, anim_time);
  anim_x.onCompleted([](GAnimation* a){ lb_item_name = item_name[cur_id]; });
  anim_x.start();

  anim_zoom.init(item[0], [](GWIDGET, int32_t v){    
    for(int i=0;i< item_num; i++){
      if(i==cur_id) {
        item[i].zoom(map_f(v, 0, 1024, zm, 100));
      }else
      if(i==cur_id-1 || i == cur_id+1) {
        item[i].zoom(map_f(v, 0, 1024, 100, zm));
      }else{
        item[i].zoom(zm);
      }
    }
  }, 0, 1024, anim_time);
  anim_zoom.onCompleted([](GAnimation*a){ 
    for(int i=0;i< item_num; i++){
      item[i].zoom((i==cur_id)? 100 : zm);
    }
  });
  anim_zoom.start();
}


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  //------------------------------------------------------------------  
  cont_frame.size(PCT(100), PCT(100));
  cont_frame.GRID_CELL( 
    // แบ่ง GRID 16 x 9 ช่อง
    GRID_COL{GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1)}, 
    GRID_ROW {130, GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1) }
  );
  cont_frame.padding(10,10,10,10,10);
  cont_frame.bg_opa(0);
  //------------------------------------------------------------------  
  for(int i=0;i <4 ;i++){
    rect_gauge[i].parent(cont_frame);
    rect_gauge[i].color(RECT_CELL_COLOR);
    rect_gauge[i].radius(10);
    rect_gauge[i].grid_cell(i*4, 0, 4, 1);                                          // ไว้ที่ grid_cell (i*4,0) ขยาย grid แนวนอน 4 ช่อง แนวตั้ง 1 ช่อง
      gauge[i].parent(rect_gauge[i]);
      gauge[i].width(PCT(90));
      gauge[i].height(gauge[i].width());
      gauge[i].align(ALIGN_TOP,0,20);
      gauge[i].range(0,100);
      gauge[i].value(random(100));
      gauge[i].title(gauge_title[i]);
  }
  //------------------------------------------------------------------  
  for(int i=0; i< 4; i++){
    sw_homeswitch[i].parent(cont_frame);
    sw_homeswitch[i].grid_cell(10,2*i+1,6,2,GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH); // ไว้ที่ grid_cell (10,2*i+1) ขยาย grid แนวนอน 6 ช่อง แนวตั้ง 2 ช่อง แบบเต็มช่อง grid
    sw_homeswitch[i].color(sw_hoemswitch_color[i]);
    sw_homeswitch[i].name(sw_homeswitch_name[i]);
    ( random(3,10)>5)?  sw_homeswitch[i].ON() : sw_homeswitch[i].OFF();
  }
  //------------------------------------------------------------------  
  rect_datetime.color(RECT_CELL_COLOR);
  rect_datetime.radius(10);
  rect_datetime.grid_cell(0,1,4,2);                                                 // ไว้ที่ grid_cell (0,1) ขยาย grid แนวนอน 4 ช่อง แนวตั้ง 2 ช่อง
    lb_time.font(BebasNeueBold_num_45, TFT_WHITE);
    lb_time.align_center(0,8);
    lb_date.font(prasanmit_20, TFT_WHITE);
    lb_date.align(lb_time, ALIGN_TOP,0,5);
    static SoftTimer timer_datetime;
    timer_datetime.setInterval(1000,[](){
      lb_time = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
      lb_date = StringX::printf("%02d/%02d/%04d", day(), month(), year());
    });
  
  //------------------------------------------------------------------  
  rect_chart_circle.color(RECT_CELL_COLOR);
  rect_chart_circle.radius(10);
  rect_chart_circle.grid_cell(4,1,5,6); 
    scale_chart_circle.size(PCT(80), PCT(80));
    scale_chart_circle.mode(SCALE_MODE_ROUND);
    scale_chart_circle.range(0,100);
    scale_chart_circle.angle_offset(270);
    scale_chart_circle.angle_range(360);
    scale_chart_circle.tick_opa(0); 
    scale_chart_circle.showLabel(false);
    scale_chart_circle.axis_thickness(50);
      uint8_t range_min[] = {0, 61, 76, 86};
      uint8_t range_max[] = {60,75, 85, 99};
      color_t range_color[] = { TFT_PALETTE(TFT_PALETTE_GREEN), TFT_PALETTE(TFT_PALETTE_YELLOW), TFT_PALETTE(TFT_PALETTE_BLUE), TFT_PALETTE(TFT_PALETTE_RED)};
      for(int i=0; i< 4; i++){
        GScaleSection * section = scale_chart_circle.addSection();
        section->range(range_min[i],range_max[i]);
        section->color(range_color[i]);
      }
    // scale_chart_circle.align_center(0,0);

  //------------------------------------------------------------------
  rect_water_tank.color(RECT_CELL_COLOR);
  rect_water_tank.radius(10);
  rect_water_tank.grid_cell(0,3,4,5); 

    bar_water_tank.orientation(BAR_ORIENTATION_VERTICAL);  // แนวตั้ง
    bar_water_tank.size(PCT(60), PCT(70));
    bar_water_tank.color(TFT_COLOR_HSV(220,100,100));
    bar_water_tank.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BLUE,1)); 
    bar_water_tank.padding(5);                             // ช่องไฟตัว bar กับ indicator
    bar_water_tank.border(2, TFT_WHITE);
    bar_water_tank.range(0,200);
    bar_water_tank.rounded(false);
    bar_water_tank.radius(10);                              // รัศมีมุมโค้งนอกสุด
    bar_water_tank.radius(5, GPART_INDICATOR);              // รัศมีมุมโค้ง ของ indicator
    bar_water_tank.align(ALIGN_LEFT,20);

    bar_water_tank = 124;

    scale_water_tank.size(100, bar_water_tank.height());
    scale_water_tank.range(bar_water_tank);                  // range เดียวกับ bar_water_tank
    scale_water_tank.mode(SCALE_MODE_VERTICAL_RIGHT, 21, 2); // แบบแนวตั้งด้านขวา มีเส้นขีด ทั้งหมด 21 ขีด เส้นขีดหลัก เว้นทุกๆ 4 ขีด
    scale_water_tank.tick_length(10,5);                      // ความยาว เส้นขีดหลัก 10px ความยาวเส้นขีดทั่วไป 5px
    scale_water_tank.label_show(true);                       // แสดงข้อความที่เส้นขีดหลักด้วย
    scale_water_tank.thickness(1, TFT_BLACK);

    scale_water_tank.color(TFT_WHITE);
    scale_water_tank.font(prasanmit_15, TFT_WHITE);
    scale_water_tank.align(bar_water_tank, ALIGN_RIGHT,15,0);

  //------------------------------------------------------------------  
  rect_slider.color(RECT_CELL_COLOR);
  rect_slider.radius(10);
  rect_slider.grid_cell(4,7,6,1); 
    slider.size(PCT(70),5);
    slider.color(TFT_PALETTE(TFT_PALETTE_CYAN));                    // สี slider
    slider.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_CYAN,1));       // สีพื้นหลัง
    slider.knob_color(TFT_PALETTE(TFT_PALETTE_CYAN));               // สีปุ่มเลื่อน
    slider.knob_border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_CYAN,3));  // ขอบปุ่มเลื่อน
    slider.knob_padding(6);                                         // ขยายพื้นที่ปุ่ม
    slider.range(0,100);
    slider.align(ALIGN_LEFT,30);
    slider.onValueChanged(GWIDGET_CB{
      lb_slider = slider.toInt();
    });
    lb_slider.font(prasanmit_20, TFT_WHITE);
    lb_slider.align(slider, ALIGN_RIGHT,20);

    slider = 63;
    slider.event_send(EVENT_VALUE_CHANGED);

  //------------------------------------------------------------------  
  rect_slider2.color(RECT_CELL_COLOR);
  rect_slider2.radius(10);
  rect_slider2.grid_cell(9,1,1,6); 
    slider2.size(5,PCT(70));
    slider2.color(TFT_PALETTE(TFT_PALETTE_LIME));                    // สี slider
    slider2.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_LIME,1));       // สีพื้นหลัง
    slider2.knob_color(TFT_PALETTE(TFT_PALETTE_LIME));               // สีปุ่มเลื่อน
    slider2.knob_border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_LIME,3));  // ขอบปุ่มเลื่อน
    slider2.knob_padding(6);                                         // ขยายพื้นที่ปุ่ม
    slider2.range(0,100);
    slider2.align(ALIGN_BOTTOM,0,-30);
    slider2.onValueChanged(GWIDGET_CB{
      lb_slider2 = slider2.toInt();
    });
    lb_slider2.font(prasanmit_20, TFT_WHITE);
    lb_slider2.align(slider2, ALIGN_TOP,0,-10);

    slider2 = 82;
    slider2.event_send(EVENT_VALUE_CHANGED);

  //------------------------------------------------------------------  
  lb_cardgame_menu.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_cardgame_menu.font(prasanmit_40, TFT_WHITE);
  lb_cardgame_menu.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_cardgame_menu.clickable(true);
  lb_cardgame_menu = SYMBOL_LIST;
  lb_cardgame_menu.align(ALIGN_BOTTOM_LEFT,10, cont_frame.padding_bottom()+2);
  lb_cardgame_menu.onClicked(GWIDGET_CB{
    rect_cardgame.show(true);
  });


  //------------------------------------------------------------------  
  wifi_manager.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  wifi_manager.align(lb_cardgame_menu, ALIGN_RIGHT, 20);

    // สร้าง สี่เหลี่ยมโปร่งใส ขนาด ให้เมื่อวาง บัง wifi_manager ได้
    rect_wifi_manager.size(80,80);
    rect_wifi_manager.bg_opa(0);
    rect_wifi_manager.align(wifi_manager, ALIGN_CENTER);


    rect_wifi_manager.onClicked(GWIDGET_CB{
      passcodelock.passcode(PASSCODE);

      // User ใส่รหัสถูกต้อง ให้ไปทำอะไรต่อ
      passcodelock.auth_cb([](){
        Serial.println("[GPasscodeLock] Authentication verified!");
        wifi_manager.event_send(EVENT_CLICKED);
      });

      // User ใส่รหัสไม่ถูกต้อง ครบ 3 ครั้ง หรือ User ได้กดปุ่ม Cancel ยกเลิก ให้ไปทำอะไรต่อ
      passcodelock.cancel_cb([](){
        Serial.println("[GPasscodeLock] Authentication failed or cancelled");
      });
    });

  //------------------------------------------------------------------  
  lb_config.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_config.font(material_icon_40, TFT_WHITE);
  lb_config.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_config.clickable(true);
  lb_config = SYMBOL_SETTINGS;
  lb_config.align(wifi_manager, ALIGN_RIGHT, 20);

  //------------------------------------------------------------------  
  lb_hvac.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_hvac.font(material_icon_40, TFT_WHITE);
  lb_hvac.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_hvac.clickable(true);
  lb_hvac = SYMBOL_HVAC; //SYMBOL_MONITORING;
  lb_hvac.align(lb_config, ALIGN_RIGHT, 30);
  lb_hvac.onClicked(GWIDGET_CB{
    image_hvac_background.show(true);
  });
    //-------------------------------------
    image_hvac_background.show(false);
    image_hvac_background.clickable(true);
    image_hvac_background.onClicked(GWIDGET_CB{
      image_hvac_background.show(false);
    });
      lb_hvac_title = "HVAC";
      lb_hvac_title.font(AftikaBold_50, TFT_WHITE);
      lb_hvac_title.position(35,60);

      lb_have_title_detail = "ระบบจ่ายและระบายอากาศ"; //"Supply and exhault\nventilation";
      lb_have_title_detail.font(prasanmit_20, TFT_DARKGRAY);
      lb_have_title_detail.align(lb_hvac_title, ALIGN_BOTTOM_LEFT,0,-10);

      lb_hvac_status_info = "Status";
      lb_hvac_status_info.font(prasanmit_18, TFT_WHITE);
      lb_hvac_status_info.align(lb_have_title_detail, ALIGN_BOTTOM_LEFT,0,25);

      lb_hvac_status = " • Normal";
      lb_hvac_status.font(prasanmit_40, TFT_GREEN);
      lb_hvac_status.align(lb_hvac_status_info, ALIGN_BOTTOM_LEFT);

      lb_hvac_power_info = "Power";
      lb_hvac_power_info.font(prasanmit_18, TFT_WHITE);
      lb_hvac_power_info.align(lb_hvac_status_info, ALIGN_BOTTOM_LEFT,0,70);

      sw_hvac_power.align(lb_hvac_power_info, ALIGN_RIGHT, 20 );
      sw_hvac_power.ON();


      lb_hvac_mode_info = "Mode";
      lb_hvac_mode_info.font(prasanmit_18, TFT_WHITE);
      lb_hvac_mode_info.align(lb_hvac_power_info, ALIGN_BOTTOM_LEFT,0,20);

      btn_hvac_mode_winter = SYMBOL_AC_UNIT;
      btn_hvac_mode_winter.font(material_icon_30);
      btn_hvac_mode_winter.size(45,45);
      btn_hvac_mode_winter.align(lb_hvac_mode_info, ALIGN_BOTTOM_LEFT,10,10);
      btn_hvac_mode_winter.button_type(BUTTON_SWITCH);
      btn_hvac_mode_winter.color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 2), GSTATE_ON);
      btn_hvac_mode_winter.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY, 3), GSTATE_OFF);
      btn_hvac_mode_winter.OFF();
      btn_hvac_mode_winter.onValueChanged(GWIDGET_CB{
        btn_hvac_mode_winter.isON()? btn_hvac_mode_summer.off() : btn_hvac_mode_summer.on();
        if( btn_hvac_mode_winter.isON() ){
          lb_hvac_mode_winter.color(TFT_WHITE);
          lb_hvac_mode_summer.color(TFT_DARKGRAY);
        }else{
          lb_hvac_mode_winter.color(TFT_DARKGRAY);
          lb_hvac_mode_summer.color(TFT_WHITE);
        }
      });

      lb_hvac_mode_winter = "Winter";
      lb_hvac_mode_winter.font(prasanmit_15, TFT_DARKGRAY);
      lb_hvac_mode_winter.align(btn_hvac_mode_winter, ALIGN_BOTTOM);


      btn_hvac_mode_summer = SYMBOL_SUNNY;
      btn_hvac_mode_summer.font(material_icon_30);
      btn_hvac_mode_summer.size(45,45);
      btn_hvac_mode_summer.align(btn_hvac_mode_winter, ALIGN_RIGHT,10,0);
      btn_hvac_mode_summer.button_type(BUTTON_SWITCH);
      btn_hvac_mode_summer.color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 2), GSTATE_ON);
      btn_hvac_mode_summer.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY, 3), GSTATE_OFF);
      btn_hvac_mode_summer.ON();
      btn_hvac_mode_summer.onValueChanged(GWIDGET_CB{
        btn_hvac_mode_summer.isON()? btn_hvac_mode_winter.off() : btn_hvac_mode_winter.on();
        if( btn_hvac_mode_summer.isON() ){
          lb_hvac_mode_winter.color(TFT_DARKGRAY);
          lb_hvac_mode_summer.color(TFT_WHITE);
        }else{
          lb_hvac_mode_winter.color(TFT_WHITE);
          lb_hvac_mode_summer.color(TFT_DARKGRAY);
        }
      });

      lb_hvac_mode_summer = "Summer";
      lb_hvac_mode_summer.font(prasanmit_15, TFT_WHITE);
      lb_hvac_mode_summer.align(btn_hvac_mode_summer, ALIGN_BOTTOM);


  // GLabel lb_hvac_outdoor_temperature(image_hvac_background);
      lb_hvac_outdoor_temperature = "อุณหภูมิ\nภายนอก";
      lb_hvac_outdoor_temperature.font(prasanmit_15, TFT_SILVER);
      lb_hvac_outdoor_temperature.line_space(-4);
      lb_hvac_outdoor_temperature.center(260,175);

      v_hvac_outdoor_temperature = 20;
      v_hvac_outdoor_temperature.align(lb_hvac_outdoor_temperature, ALIGN_BOTTOM );

      lb_hvac_indoor_temperature = "อุณหภูมิ\nภายใน";
      lb_hvac_indoor_temperature.font(prasanmit_15, TFT_SILVER);
      lb_hvac_indoor_temperature.line_space(-4);
      lb_hvac_indoor_temperature.position(725,60);

      v_hvac_indoor_temperature = 20;
      v_hvac_indoor_temperature.align(lb_hvac_indoor_temperature, ALIGN_TOP );

      lb_hvac_filter_pressure_drop1 = "ความดันตกคร่อม filter";
      lb_hvac_filter_pressure_drop1.font(prasanmit_15, TFT_SILVER);
      lb_hvac_filter_pressure_drop1.center(330,110);

      arcv_hvac_filter_pressure_drop1.align(lb_hvac_filter_pressure_drop1, ALIGN_BOTTOM_LEFT, 25,5);
      arcv_hvac_filter_pressure_drop1.value(52, "Pa");
      
      lb_hvac_filter_pressure_drop2 = "ความดันตกคร่อม filter";
      lb_hvac_filter_pressure_drop2.font(prasanmit_15, TFT_SILVER);
      lb_hvac_filter_pressure_drop2.center(530,20);

      arcv_hvac_filter_pressure_drop2.align(lb_hvac_filter_pressure_drop2, ALIGN_BOTTOM_LEFT, 25,5);
      arcv_hvac_filter_pressure_drop2.value(52, "Pa");

      lb_hvac_engine_output1 = "Engine Output";
      lb_hvac_engine_output1.font(prasanmit_15, TFT_SILVER);
      lb_hvac_engine_output1.line_space(-4);
      lb_hvac_engine_output1.position(340,440);

      arcv_hvac_engine_output1.align(lb_hvac_engine_output1, ALIGN_TOP_LEFT, 0, -5);
      arcv_hvac_engine_output1 = 52;

      lb_hvac_engine_output2 = "Engine Output";
      lb_hvac_engine_output2.font(prasanmit_15, TFT_SILVER);
      lb_hvac_engine_output2.line_space(-4);
      lb_hvac_engine_output2.position(690,240);

      arcv_hvac_engine_output2.align(lb_hvac_engine_output2, ALIGN_TOP_LEFT, 0, -5);
      arcv_hvac_engine_output2 = 99;

      lb_hvac_exhault_temperature = "อุณหภูมิ\nอากาศระบายออก";
      lb_hvac_exhault_temperature.font(prasanmit_15, TFT_SILVER);
      lb_hvac_exhault_temperature.line_space(-4);
      lb_hvac_exhault_temperature.position(250,440);

      v_hvac_exhault_temperature = 20;
      v_hvac_exhault_temperature.align(lb_hvac_exhault_temperature, ALIGN_TOP_LEFT );

      
      lb_hvac_water_heater_output = "Water Heater Output";
      lb_hvac_water_heater_output.font(prasanmit_15, TFT_SILVER);
      lb_hvac_water_heater_output.line_space(-4);
      lb_hvac_water_heater_output.position(500,340);

      arcv_hvac_water_heater_output.align(lb_hvac_water_heater_output, ALIGN_BOTTOM_LEFT, 0, 5);
      arcv_hvac_water_heater_output = 98;

      lb_hvac_return_water_temperature = "อุณหภูมิ return water";
      lb_hvac_return_water_temperature.font(prasanmit_15, TFT_SILVER);
      lb_hvac_return_water_temperature.line_space(-4);
      lb_hvac_return_water_temperature.position(500,455);

      v_hvac_return_water_temperature = 20;
      v_hvac_return_water_temperature.align(lb_hvac_return_water_temperature, ALIGN_TOP_LEFT );

      lb_hvac_humidifier_output = "Humidifier Output";
      lb_hvac_humidifier_output.font(prasanmit_15, TFT_SILVER);
      lb_hvac_humidifier_output.line_space(-4);
      lb_hvac_humidifier_output.position(650,330);

      arcv_hvac_humidifier_output.align(lb_hvac_humidifier_output, ALIGN_TOP_LEFT, 0, -5);
      arcv_hvac_humidifier_output = 97;

  //------------------------------------------------------------------  
  lb_folder.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_folder.font(material_icon_40, TFT_WHITE);
  lb_folder.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_folder.clickable(true);
  lb_folder = SYMBOL_FOLDER;
  lb_folder.align(lb_hvac, ALIGN_RIGHT, 30);

  //------------------------------------------------------------------  
  lb_touch.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_touch.font(material_icon_40, TFT_WHITE);
  lb_touch.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_touch.clickable(true);
  lb_touch = SYMBOL_TRACKPAD_INPUT; // SYMBOL_OVERVIEW;
  lb_touch.align(lb_folder, ALIGN_RIGHT, 30);
  lb_touch.onClicked(GWIDGET_CB{
    image_touch_tester.show(true);
  });
    //-------------------------------------------------------
    image_touch_tester.show(false);
    image_touch_tester.clickable(true);
    image_touch_tester.event_cb([](GWIDGET, event_t event){
      static SoftTimer timer;
      Point p;
      switch(event){
        case EVENT_PRESSED:
          dot_touch.show(true);
          break;
        case EVENT_PRESSING:
          dot_touch.show(true);
          p = image_touch_tester.touch_point();
          //POINT_PRINTLN(p);
          dot_touch.center(p);
          lb_touch_info = StringX::printf("ทดสอบสัมผัส : %d, %d", (int)p.x, (int)p.y);
          break;
        case EVENT_RELEASED:
        case EVENT_PRESS_LOST:
          timer.delay(2000,[](){
            dot_touch.show(false);
          });
          break;
      }
    });

      btn_touch_tester_back = SYMBOL_ARROW_LEFT;
      btn_touch_tester_back.round_design();
      // btn_touch_tester_back.align(ALIGN_TOP_LEFT,10,10);
      btn_touch_tester_back.position(10,10);
      btn_touch_tester_back.onClicked(GWIDGET_CB{
        image_touch_tester.show(false);
        // rect_dashboard.show(true);
        // rect_weightscale.show(false);
        // rect_setting.show(false);
        // image_touch_tester.show(false);
      });

      dot_touch.color(TFT_YELLOW);
      dot_touch.radius(10);
      dot_touch.shadow(20,TFT_YELLOW);
      dot_touch.center(0,0);
      dot_touch.show(false);

      lb_touch_info = "ทดสอบสัมผัส";
      lb_touch_info.align(ALIGN_BOTTOM_LEFT,10,-10);
      lb_touch_info.color(TFT_WHITE);
    //-------------------------------------------------------

  //------------------------------------------------------------------  
  lb_info.ignore_layout(true);       // ไม่ต้องถูกจัด layout โดย แม่
  lb_info.font(material_icon_40, TFT_WHITE);
  lb_info.font_color(TFT_COLOR_HSV(0,0,20), GSTATE_PRESSED);
  lb_info.clickable(true);
  lb_info = SYMBOL_INFO;
  lb_info.align(lb_touch, ALIGN_RIGHT, 30);
  
  //------------------------------------------------------------------  
  
  //------------------------------------------------------------------  
  rect_cardgame.show(false);
  rect_cardgame.color(TFT_BLACK);

  btn_cardgame_back = SYMBOL_HOME;
  btn_cardgame_back.font(material_icon_30);
  btn_cardgame_back.round_design();
  btn_cardgame_back.align(ALIGN_TOP_LEFT, 10,10);
  btn_cardgame_back.onClicked(GWIDGET_CB{
    rect_cardgame.show(false);
  });

  menu.size(400, SIZE_CONTENT);
  menu.padding(20,20, 0, 0);
  menu.bg_opa(0);
    cont_item.layout(LAYOUT_ROW_M,5,5,5,5,10);
    cont_item.bg_opa(0);
    item_num = sizeof(item)/sizeof(item[0]);
    for(int i=0; i< item_num; i++){
      item[i].parent(cont_item);
      item[i].clickable(true);
      item[i].image_recolor(TFT_BLACK, GSTATE_PRESSED);
      item[i].image_recolor_opa(180, GSTATE_PRESSED);
      item[i].onClicked(GWIDGET_CB{
        int ii = (GImage*)widget-&item[0];
        if(cur_id != ii) anim_item(ii);
      });
    }

    rect_selector.size(item[0].width()+20, item[0].height()+20);
    rect_selector.center(menu.centerpoint() - Point(menu.pad_left(), menu.pad_top()));
    rect_selector.bg_opa(0);
    rect_selector.border(4, TFT_RED);
    rect_selector.radius(10);
    rect_selector.clickable(false);

  lb_item_name.align(menu, ALIGN_TOP,0, -5);
  lb_item_name.font(prasanmit_40);

  btn_right = SYMBOL_ARROW_RIGHT;
  btn_right.align(menu, ALIGN_RIGHT, 30);
  btn_right.onClicked(GWIDGET_CB{
    if (cur_id <= item_num - 2) 
      anim_item(++cur_id);
  });

  btn_left  = SYMBOL_ARROW_LEFT;
  btn_left.align(menu, ALIGN_LEFT, -30);
  btn_left.onClicked(GWIDGET_CB{
    if (cur_id > 0)
      anim_item(--cur_id);
  });
  anim_item(cur_id=0);  

}

void loop(){
  BlynkGO.update();
}
