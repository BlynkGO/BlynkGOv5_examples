/* ออกแบบสำหรับ 480x640  หรือ 480x800 ก็ได้ */

#include <BlynkGOv5.h>
#include "fonts/Material_Icon_List.h"

FONT_DECLARE(Material_Icon_25);
FONT_DECLARE(Material_Icon_40);
FONT_DECLARE(BebasNeue_Bold_35);

#define MY_GREEN  TFT_COLOR_HEX(0x9ae445)
#define MY_CYAN   TFT_PALETTE(TFT_PALETTE_BLUE)

class GCardValue : public GRect {
  public:
    GCardValue(GWidget&parent=GScreen): GRect(parent)   {}
    GLabel lb_title;
    GLabel lb_value;
    float _v;
    int _d=1;
    void create(){
      if(isCreated()) return;
      GRect::create();
      lb_value.parent(this);
      lb_value.font(BebasNeue_Bold_35, TFT_BLACK);

      lb_title.parent(this);
      lb_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
      lb_title.align(ALIGN_BOTTOM,0,-2);
    }
    inline GCardValue& name(String title)    { create(); lb_title = title;              return *this; }
    inline GCardValue& value(float val)      { create(); lb_value = String(_v=val, _d); return *this; }
    inline GCardValue& decimal(int d)        { create(); lb_value = String(_v, _d=d);   return *this; }
};

class GItemStatus : public GRect {
  public:
    GItemStatus(GWidget&parent=GScreen): GRect(parent)   {}
    GRect box;
    GLabel lb;
    bool _state = true;
    void create(){
      if(isCreated()) return;
      GRect::create();
      GRect::GRID_CELL(GRID_COL{GRID_CONTENT, 15}, GRID_ROW{GRID_CONTENT});
      GRect::padding(2,2,2,2,2);
      GRect::grid_align(GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      GRect::bg_opa(60);
      GRect::radius(3);
      GRect::clickable(false);
        box.parent(this);
        box.color(TFT_BLUE);
        box.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
        box.size(10,10);

        lb.parent(this);
        lb.grid_cell(1,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);
        lb.font(prasanmit_18, TFT_BLACK);
        lb = "B1";
    }
    inline GItemStatus& name(String name)   { create(); lb = name; return *this; }
    inline void ON() {
      this->create();
      box.color(TFT_BLUE);
      lb.color(TFT_BLACK);
      _state = true;
    }
    inline void OFF(){
      this->create();
      box.color(TFT_RED);
      lb.color(TFT_RED);
      _state = false;
    }
    inline void toggle(){
      this->create();
      (_state = !_state) ? ON() : OFF();
    }
      
};

class GCardMachineState : public GRect {
  public:
    GCardMachineState(GWidget&parent=GScreen): GRect(parent)   {}
    GLabel lb_title;
    GLabel lb_symbol;
    void create(){
      if(isCreated()) return;
      GRect::create();
      GRect::GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_CONTENT});
      GRect::grid_align(GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      GRect::pad_top(10);
      GRect::pad_bottom(5);
      GRect::pad_inner(0);
      
      lb_symbol.parent(this);
      lb_symbol.font(Material_Icon_40, TFT_BLACK);
      lb_symbol = SYMBOL_B_CIRCLE;
      lb_symbol.grid_cell(0,0);
      lb_symbol.text_align(TEXT_ALIGN_CENTER);

      lb_title.parent(this);
      lb_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
      lb_title = "title";
      lb_title.grid_cell(0,1);
      lb_title.text_align(TEXT_ALIGN_CENTER);
    }

    inline GCardMachineState& name(String title)          { create(); lb_title = title; return *this;     }
    inline GCardMachineState& symbol_state(String symbol) { create(); lb_symbol = symbol; return * this;  }
};


class G_MyICON : public GContainer {
  public:
    G_MyICON(GWidget&parent=GScreen): GContainer(parent)   {}
    GLabel lb_symbol;
    GLabel lb_name;
    void create(){
      if(isCreated()) return;
      GContainer::create();
      GContainer::layout(LAYOUT_COL_M, 10, 2, 10, 10, -3);
      GContainer::color(TFT_BLACK);
      GContainer::color(TFT_COLOR_HEX(0x333333), GSTATE_PRESSED);
      GContainer::border(2, TFT_BLACK);
      GContainer::border(2, MY_CYAN, GSTATE_HOVERED);
      GContainer::text_color(MY_CYAN, GSTATE_ANY);
      GContainer::event_cb([](GWIDGET, event_t e){
        G_MyICON* myicon = (G_MyICON*)widget;
        if(e== EVENT_HOVER_OVER || e== EVENT_HOVER_LEAVE){
          myicon->lb_symbol.event_send(e);
          myicon->lb_name.event_send(e);
        }
      });
        lb_symbol = SYMBOL_HOME;
        lb_symbol.parent(this);
        lb_symbol.font(Material_Icon_25);
        lb_symbol.remove_color();
        lb_symbol.text_color(MY_CYAN, GSTATE_HOVERED);

        lb_name = "Home";
        lb_name.parent(this);
        lb_name.font(prasanmit_15);
        lb_name.remove_color();
        lb_name.text_color(MY_CYAN, GSTATE_HOVERED);

      }

    inline void set(String sym, String name){
      this->create();
      lb_symbol = sym;
      lb_name = name;
    }
};

GPage page;
  GContainer cont_grid(page);
    GLabel lb_title(cont_grid);

    GRect rect_gauge_bar(cont_grid);
      GLabel lb_gauge_title(rect_gauge_bar);
      GBar gauge_bar(rect_gauge_bar);
      GScale gauge_scale(rect_gauge_bar);
      GLabel gauge_value(rect_gauge_bar);
    GRect rect_btn(cont_grid);
      GLabel lb_btn_title(rect_btn);
      GButton btn(rect_btn);
    GRect rect_btn2(cont_grid);
      GLabel lb_btn2_title(rect_btn2);
      GButton btn2(rect_btn2);
    GRect rect_slider(cont_grid);
      GLabel lb_slider_title(rect_slider);
      GSlider slider(rect_slider);
      GScale  slider_scale(rect_slider);
      GLabel  lb_slider_val(rect_slider);
    GRect rect_led(cont_grid);
      GLed led(rect_led);
      GLabel lb_led_title(rect_led);
    GCardValue rect_v1(cont_grid);
    GCardValue rect_v2(cont_grid);
    GCardValue rect_v3(cont_grid);
    GRect rect_status(cont_grid);
      GLabel lb_status_title(rect_status);
      GItemStatus item_status[16];
    GCardMachineState rect_state1(cont_grid);
    GCardMachineState rect_state2(cont_grid);

GContainer cont_bottom_menu;
  G_MyICON icon[3];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  page.size(PCT(100), PCT(100));
  page.padding(0,60,0,0,0);
  page.bg_opa(0);
  page.scrollbar(SCROLLBAR_AUTO, false, true);  
  page.opa(0, GPART_SCROLLBAR);

    cont_grid.size(PCT(100), SIZE_CONTENT);
    cont_grid.ignore_layout(true);
    cont_grid.align(ALIGN_TOP);
    cont_grid.bg_opa(0);
    cont_grid.GRID_CELL(
      GRID_COL{GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1)}, 
      GRID_ROW{GRID_CONTENT, 120, 120, 120, 120, 80, GRID_CONTENT}
    );
    cont_grid.padding(0,8,8,8,8);
      lb_title = "BeeNeXT Farm v.987";
      lb_title.color(MY_GREEN);
      lb_title.grid_cell(0,0,4,1, GRID_ALIGN_START, GRID_ALIGN_CENTER, 20, 0);

      static GStyle style_main, style_hover;
        style_main.radius(10);
        style_main.bg_color(MY_GREEN);

        style_hover.bg_color(MY_CYAN);
        style_hover.shadow(10, MY_CYAN);
        style_hover.shadow_spread(3);
        style_hover.shadow_offset(0,0);

      auto Set_CardStyle = [](GRect& rect){
        rect.remove_local_style_prop(GSTYLE_BG_COLOR);
        rect.remove_local_style_prop(GSTYLE_RADIUS);
        rect.addStyle(style_main, GPART_MAIN);
        rect.addStyle(style_hover, GSTATE_HOVERED);
      };

      rect_gauge_bar.grid_cell(0,1,2,2);  Set_CardStyle(rect_gauge_bar);
        lb_gauge_title = "Gauge";
        lb_gauge_title.rotation(90);
        lb_gauge_title.align(ALIGN_LEFT,0);
        gauge_bar.size(10, PCT(80));
        gauge_bar.orientation(BAR_ORIENTATION_VERTICAL);
        gauge_bar.color(TFT_RED);
        gauge_bar.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED, 2));
        gauge_bar.border(1, TFT_BLACK, GPART_MAIN);
        gauge_bar.border_opa(180);
        gauge_bar.align(ALIGN_CENTER,-20,0);
        gauge_bar.onValueChanged(GWIDGET_CB{
          gauge_value = gauge_bar.toInt();
        });
        gauge_bar = 80;

        gauge_scale.height(gauge_bar);
        gauge_scale.mode(SCALE_MODE_VERTICAL_RIGHT, 5, 1);
        gauge_scale.range(0,100);
        gauge_scale.padding(4, GPART_INDICATOR);
        gauge_scale.axis_opa(0);
        gauge_scale.align(gauge_bar, ALIGN_RIGHT, 2, 0);

        gauge_value.font(BebasNeueBold_num_45, TFT_BLACK);
        gauge_value.align(gauge_bar, ALIGN_RIGHT, 60);
        gauge_value = gauge_bar.toInt();

      rect_btn.grid_cell(2,1);            Set_CardStyle(rect_btn);
        lb_btn_title = "สวิตซ์ปั๊ม";
        lb_btn_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
        lb_btn_title.position(10,2);
        btn = "OFF";
        btn.size(70,70);
        btn.align_center(0,5);
        btn.radius(RADIUS_CIRCLE);
        btn.button_type(BUTTON_SWITCH);
        btn.color(TFT_GREEN, GSTATE_ON);
        btn.color(TFT_RED, GSTATE_OFF);
        btn.font_color(TFT_BLACK, GSTATE_ON);
        btn.font_color(TFT_WHITE, GSTATE_OFF);
        btn.onValueChanged(GWIDGET_CB{
          btn = btn.isON()? "ON" : "OFF";
        });

      rect_btn2.grid_cell(3,1);           Set_CardStyle(rect_btn2);
        lb_btn2_title = "สวิตซ์ปุ๋ย AB";
        lb_btn2_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
        lb_btn2_title.position(10,2);
        btn2 = "OFF";
        btn2.size(70,70);
        btn2.align_center(0,5);
        btn2.radius(RADIUS_CIRCLE);
        btn2.button_type(BUTTON_SWITCH);
        btn2.color(TFT_GREEN, GSTATE_ON);
        btn2.color(TFT_RED, GSTATE_OFF);
        btn2.font_color(TFT_BLACK, GSTATE_ON);
        btn2.font_color(TFT_WHITE, GSTATE_OFF);
        btn2.onValueChanged(GWIDGET_CB{
          btn2 = btn2.isON()? "ON" : "OFF";
        });

      rect_slider.grid_cell(2,2,2,1);     Set_CardStyle(rect_slider);
        lb_slider_title = "ความสว่าง";
        lb_slider_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
        lb_slider_title.align(ALIGN_BOTTOM_LEFT,10,-2);
        slider.size( PCT(75), 10);
        slider.color(TFT_VIOLET);
        slider.knob_color(TFT_PALETTE_DARKEN(TFT_PALETTE_DEEP_PURPLE, 2));
        slider.bg_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_DEEP_PURPLE, 1));
        slider.align_center(0, 5);
        slider.onValueChanged(GWIDGET_CB{
          lb_slider_val = slider.toInt();
        });
        slider = 75;

        slider_scale.width(slider);
        slider_scale.mode(SCALE_MODE_HORIZONTAL_BOTTOM, 2, 1);
        slider_scale.range(0,100);
        slider_scale.axis_opa(0);
        slider_scale.tick_opa(0);
        slider_scale.align(slider, ALIGN_BOTTOM,0,-5);

        lb_slider_val.align(slider, ALIGN_TOP, 0,-2);
        lb_slider_val.font(prasanmit_40, TFT_BLACK);
        lb_slider_val = slider.toInt();

      rect_led.grid_cell(0,3);            Set_CardStyle(rect_led);
        lb_led_title = "LED";
        lb_led_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
        lb_led_title.align(ALIGN_BOTTOM,0,-2);

        led.color(TFT_ORANGE);
        led.checkable(true);
        led.onValueChanged(GWIDGET_CB{
          Serial.println(led.isON());
        });

      rect_v1.grid_cell(1,3);             Set_CardStyle(rect_v1);
        rect_v1.name("Volt").value(230);

      rect_v2.grid_cell(2,3);             Set_CardStyle(rect_v2);
        rect_v2.name("Amp").value(22.1);

      rect_v3.grid_cell(3,3);             Set_CardStyle(rect_v3);
        rect_v3.name("Power").value(432);

      rect_status.grid_cell(0,4,4,1);     Set_CardStyle(rect_status);
      rect_status.GRID_CELL(GRID_COL{GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1)}, GRID_ROW{GRID_CONTENT, 40, 40});
      rect_status.padding(0,0,10,10,3);
        lb_status_title = "Status";
        lb_status_title.font(prasanmit_18, TFT_COLOR_HEX(0x444444));
        lb_status_title.grid_cell(0,0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 10);

        for(int i=0; i<16; i++){
          item_status[i].parent(rect_status);
          item_status[i].grid_cell(i%8, i/8 + 1);
          item_status[i].name(String("B")+String(i));
          (random(2))? item_status[i].ON(): item_status[i].OFF();
        }

        
      rect_state1.grid_cell(0,5);         Set_CardStyle(rect_state1);
        rect_state1.name("Machine1 State").symbol_state(SYMBOL_HISTORY_TOGGLE_OFF);

      rect_state2.grid_cell(1,5);         Set_CardStyle(rect_state2);
        rect_state2.name("Machine2 State");


  cont_bottom_menu.size(PCT(100), SIZE_CONTENT);
  cont_bottom_menu.padding(5);
  cont_bottom_menu.GRID_CELL(GRID_COL{GRID_FR(1),GRID_FR(1),GRID_FR(1)}, GRID_ROW{GRID_CONTENT});
  cont_bottom_menu.align(ALIGN_BOTTOM);
  cont_bottom_menu.color(TFT_COLOR_HEX(0x222222));
  cont_bottom_menu.radius(0);
  for(int i=0; i<3; i++){
    icon[i].parent(cont_bottom_menu);
    icon[i].grid_cell(i,0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    if(i==0)
      icon[i].set(SYMBOL_DASHBOARD, "Main");
    else if(i>0)
      icon[i].set(SYMBOL_DASHBOARD, String("Page")+String(i+1));
  }

  page.padding_bottom(cont_bottom_menu.height());
}

void loop(){
  BlynkGO.update();
}
