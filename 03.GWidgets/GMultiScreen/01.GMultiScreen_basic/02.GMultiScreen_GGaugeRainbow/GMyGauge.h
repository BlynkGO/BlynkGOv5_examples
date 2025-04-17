#pragma once

#include <BlynkGOv5.h>

class GMyGauge : public GContainer {
  public:
    GMyGauge(GWidget&parent=GScreen) : GContainer(parent) {}
    GLabel label;
    GGaugeRainbow gauge;
    void create(){
      if(this->isCreated()) return;
      GContainer::create();
      GContainer::layout(LAYOUT_COL_M,0,0,0,0,10); 
      GContainer::color(TFT_BLACK);
        label.parent(this);
        gauge.parent(this);
        gauge.size(180,180);
        gauge.color(TFT_RED, TFT_BLUE);
    }
    void value(float val)   { create(); gauge = val;  }
    void title(String text) { create(); label = text; }
};

