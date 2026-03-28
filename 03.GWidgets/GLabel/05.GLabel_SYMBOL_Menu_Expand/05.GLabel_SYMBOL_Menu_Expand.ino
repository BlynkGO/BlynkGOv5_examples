#include <BlynkGOv5.h>
#include "Material_Icon_List.h"

FONT_DECLARE(Material_Icon_30);

GContainer cont;
  GLabel icon[] = {SYMBOL_MORE_HORIZ, SYMBOL_DEW_POINT, SYMBOL_DIRECTIONS_WALK, SYMBOL_FIRE_CHECK, SYMBOL_MODE_FAN_2, SYMBOL_CLIMATE_MINI_SPLIT};

int8_t last_icon = -1;
bool  icon_clicked = false;

void anim_icon(){
  static bool expanded = false;
  static GAnimation anim;
  static SoftTimer t;
  if(!expanded){
    // ขยายออก
    anim.init(cont, [](GWidget* widget, int32_t v){  cont.pad_column(v); }, -50, 20, 400);
    anim.completed_cb([](GAnimation* a){ expanded = true; t.delay(2000, anim_icon); });
    anim.path_type(ANIM_PATH_EASE_OUT);
    anim.start();
  }else{
    // หดกลับ
    t.del();
    anim.init(cont, [](GWidget* widget, int32_t v){ cont.pad_column(v); }, 20, -50, 400);
    anim.completed_cb([](GAnimation* a){ 
      expanded = false;
      if(icon_clicked && last_icon > 0){
        icon_clicked = false;
        switch(last_icon){
          case 1: Serial.println("SYMBOL_DEW_POINT"); break;
          case 2: Serial.println("SYMBOL_DIRECTIONS_WALK"); break;
          case 3: Serial.println("SYMBOL_FIRE_CHECK"); break;
          case 4: Serial.println("SYMBOL_MODE_FAN_2"); break;
          case 5: Serial.println("SYMBOL_CLIMATE_MINI_SPLIT"); break;
        }
        last_icon = -1;
      }
    });
    anim.path_type(ANIM_PATH_EASE_OUT);
    anim.start();
  }
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.bg_opa(0);
  cont.layout(LAYOUT_ROW_M_REVERSE, 0, 0, 0, 0, -50);
  cont.radius(RADIUS_CIRCLE);
  cont.border(0);
  cont.position(10,10);
  cont.top();
  cont.font(Material_Icon_30, TFT_WHITE);
  for(int i=0;i <6; i++){
    icon[i].parent(cont);
    icon[i].size(50,50);
    icon[i].max_height(50);
    icon[i].bg_opa(255);
    icon[i].bg_color(TFT_BLACK);
    icon[i].border(1, TFT_SILVER);
    icon[i].border_opa(120);
    icon[i].pad_top(10);
    icon[i].radius(RADIUS_CIRCLE);
    icon[i].text_align(TEXT_ALIGN_CENTER);
    icon[i].toBackground();
    icon[i].clickable(true);
    icon[i].bg_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY, 2), GSTATE_PRESSED);
    icon[i].onClicked(GWIDGET_CB{
      int ii = (GLabel*)widget-&icon[0];
      // Serial.printf("icon %d clicked\n", ii);
      last_icon = ii;
      icon_clicked = true;
      anim_icon();
    });
  }
}

void loop(){
  BlynkGO.update();
}
