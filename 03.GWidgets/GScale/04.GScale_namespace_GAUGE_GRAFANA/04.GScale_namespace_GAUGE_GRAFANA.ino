#include <BlynkGOv5.h>

namespace GAUGE_GRAFANA {
  void value(uint8_t value);
  uint8_t value();
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  GAUGE_GRAFANA::value(82);
}

void loop(){
  BlynkGO.update();
}

//---------------------------------------------------------------------------
namespace GAUGE_GRAFANA {
  GScale scale;
    uint32_t _range[] = { 0, 40, 60, 80, 100};
      color_t sect_color[4] = { TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_RED};

    GArc arc(scale);
    GLabel lb_value(scale);
    uint8_t _value;

  void create(){
    if(scale.isCreated()) return;
    scale.size(200,200); 
    scale.mode(SCALE_MODE_ROUND);
    scale.range(0,100);
    scale.angle_offset(170);
    scale.angle_range(200);
    scale.tick_opa(0); 
    scale.showLabel(false);

    for(int i=0; i<4; i++) {
      GScaleSection *section = scale.addSection();
      section->thickness(5, sect_color[i]);
      section->range(_range[i],_range[i+1]);
    }

    arc.size(PCT(90),PCT(90));
    arc.thickness( 0.1* scale.width(), TFT_GREEN);
    arc.range(0,100);
    arc.angle_offset(scale.angle_offset());
    arc.angle_range(scale.angle_range());
    arc.bg_enable(true);
    arc.bg_color(TFT_COLOR_HEX(0x2d2f34));

    lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
    lb_value = 90;
  }

  void value(uint8_t value){
    create();
    _value = value;
    lb_value = _value;
    arc = _value;
    for(int i=1; i<=4; i++){
      if( _value <= _range[i]) {
        arc.color(sect_color[i-1]) ;
        break;
      }
    }
  }
  
  uint8_t value()      { create(); return _value;    }
}
