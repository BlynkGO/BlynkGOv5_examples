#include <BlynkGOv5.h>

FONT_DECLARE(material_icon_30);
FONT_DECLARE(material_icon_40);

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
    GRect::size(SIZE_CONTENT, SIZE_CONTENT);
    GRect::GRID_CELL(GRID_COL{ 45, 120, 90}, GRID_ROW { 35, 25 });
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

GContainer cont_home( 
  GRID_COL { GRID_FR(1), GRID_FR(1)}, 
  GRID_ROW { 40, GRID_CONTENT }, 
  OPA_0
);
  GLabel lb_title("Home Sweet Home", cont_home);
  GPage page_floor1( LAYOUT_COL_M, 5, 5, 5, 5, 5, cont_home );
  GHomeSwitch homesw_floor1[5];
    String homesw_floor1_name[5] = { "ห้องนอน", "ห้องนั่งเล่น", "ห้องครัว", "ห้องรับแขก", "โฮมเธียเตอร์"};

  GPage page_floor2( LAYOUT_COL_M, 5, 5, 5, 5, 5, cont_home);
  GHomeSwitch homesw_floor2[4];
    String homesw_floor2_name[4] = { "ห้องนอน #1", "ห้องนอน #2", "ห้องน้ำ", "ตู้เสื้อผ้า"};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_COLOR_HSV(0,0,90));

  cont_home.padding(0, 0, 0, 0, 5, 20);
    lb_title.grid_cell(0, 0, 2, 1);
    lb_title.text_align(TEXT_ALIGN_CENTER);
    page_floor1.bg_opa(0);
    page_floor1.grid_cell(0,1);
    for(int i=0; i< 5; i++){
      homesw_floor1[i].parent(page_floor1);
      homesw_floor1[i].name( homesw_floor1_name[i]);
      homesw_floor1[i].subname( "SweetHome ชั้น#1");

      /* ใช้งานจริง ให้เอา ส่วน random(..) นี้ ออก กำหนด สถานะ ON(), OFF() และ สี ของแต่ละ homeswเอง */
      if(random(100)>=35) homesw_floor1[i].ON();

      if(random(100) >= 40) {
        homesw_floor1[i].color(random(0xFFFF));
      } 
    }

    page_floor2.bg_opa(0);
    page_floor2.grid_cell(1,1);
    for(int i=0; i< 4; i++){
      homesw_floor2[i].parent(page_floor2);
      homesw_floor2[i].name( homesw_floor2_name[i]);
      homesw_floor2[i].subname( "SweetHome ชั้น#2");

      /* ใช้งานจริง ให้เอา ส่วน random(..) นี้ ออก กำหนด สถานะ ON(), OFF() และ สี ของแต่ละ homeswเอง */
      if(random(100)>=40) homesw_floor2[i].ON();

      if(random(100) >= 40) {
        homesw_floor2[i].color(random(0xFFFF) * random(0xFFFF));
      } 
    }
}

void loop(){
  BlynkGO.update();
}

