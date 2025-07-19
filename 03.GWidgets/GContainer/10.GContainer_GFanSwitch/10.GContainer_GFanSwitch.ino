#include <BlynkGOv5.h>

class GFanSwitch : public GContainer {
  public:
    GFanSwitch(GWidget&parent=GScreen) : GContainer(parent) {}
    GLabel Label;
    GButton btn[4];
      color_t btn_color_ON[4] =  { TFT_SILVER, TFT_RED, TFT_GREEN, TFT_BLUE};
      uint8_t _selected =0;
    void create() {
      if(this->isCreated()) return;
      GContainer::create();
      GContainer::layout(LAYOUT_COL_M, 10);
      GContainer::radius(20);
      GContainer::border(1, TFT_SILVER);
      GContainer::font_color(TFT_BLACK);
        Label.parent(this);
        Label = "FAN";
        for(int j=1; j<=4; j++){
          int i= (j<4)? j: 0;
          btn[i].parent(this);
          btn[i].mode(BUTTON_MODE_TOGGLE);
          btn[i].size(120,45);
          btn[i].rounded(true);
          btn[i] = (i)? String(i) : String("OFF");
          btn[i].color(btn_color_ON[i], GSTATE_ON);
          color_t btn_color_OFF = (i==0)? TFT_SILVER : TFT_COLOR_MIX(TFT_BLACK, btn_color_ON[i], 210);
          btn[i].color(btn_color_OFF, GSTATE_OFF);
          btn[i].onClicked(GWIDGET_CB{
            GFanSwitch *fan_sw = (GFanSwitch*) widget->parent();
            int ii= (GButton*)widget -&fan_sw->btn[0];
            if(fan_sw->_selected != ii) {
              fan_sw->_selected = ii;
              fan_sw->event_send(EVENT_VALUE_CHANGED);
            }
            for(int i=0; i<4; i++) (i==ii)? fan_sw->btn[i].ON() : fan_sw->btn[i].OFF();
            fan_sw->btn[0].OFF();
          });
        }
    }
    inline uint8_t selected()    { create(); return this->_selected; }
};

GFanSwitch sw;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  sw.onValueChanged(GWIDGET_CB{
    Serial.println(sw.selected());
  });
}

void loop(){
  BlynkGO.update();
}

