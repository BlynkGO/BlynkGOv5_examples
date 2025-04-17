#include <BlynkGOv5.h>

GButtonMatrix btnm;
GRect rect_screen[5];
  GLabel label[5];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  static const char * map[] = {SYMBOL_LEFT, "1", "2", "3", "4", "5", SYMBOL_RIGHT, ""};
  btnm.map(map);

  btnm.size(280, 30);
  btnm.font(montserrat_14);
  btnm.align(ALIGN_BOTTOM,0, -PCT(3));
  btnm.button_checkable_all(true);
  btnm.button_checkable(0, false);
  btnm.button_checkable(6, false);
  btnm.one_checked(true);
  btnm.button_checked(1);

  btnm.radius(RADIUS_CIRCLE);
  btnm.padding(3);
  btnm.padding_inner(5);
  btnm.radius(RADIUS_CIRCLE);
  btnm.border(1, TFT_BLACK);

  btnm.button_radius(RADIUS_CIRCLE);
  btnm.button_border(1, TFT_PALETTE(TFT_PALETTE_GRAY));
  btnm.button_border_side(BORDER_SIDE_INTERNAL);

  btnm.onValueChanged(GWIDGET_CB{
    int id          = btnm.selected_id();
    int checked_id  = btnm.checked_id();
    bool is_prev    = (id==0);
    bool is_next    = (id== btnm.button_cnt()-1);
    if(is_prev || is_next) {
      if (is_prev && checked_id > 1)                        btnm.checked_id(checked_id-1);
      else if(is_next && checked_id < btnm.button_cnt()-2)  btnm.checked_id(checked_id+1);
      for(int i=0; i< 5; i++) rect_screen[i].show(i == (btnm.checked_id()-1));
    }else{
      for(int i=0; i< 5; i++) rect_screen[i].show(i == (btnm.selected_id()-1));
    }
  });


  for(int i=0; i<5; i++){
    rect_screen[i].color(random(0xFFFF));
    rect_screen[i].show(i==0);
      label[i].parent(rect_screen[i]);
      label[i] = "หน้า" + String(i+1);
  }

  btnm.toForeground();
}

void loop(){
  BlynkGO.update();
}


