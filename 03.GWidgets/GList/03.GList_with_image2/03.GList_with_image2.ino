#include <BlynkGOv5.h>

IMAGE_DECLARE(img_ico_calc);
IMAGE_DECLARE(img_ico_camera);
IMAGE_DECLARE(img_ico_gauge);
IMAGE_DECLARE(img_ico_setting);

GList list;
  list_options_t list_opts = {
    { &img_ico_setting , "ตั้งค่า"},
    { &img_ico_calc    , "เครื่องคิดเลข"},
    { &img_ico_camera  , "กล้องถ่ายรูป"},
    { &img_ico_gauge   , "เกจจ์"},
  };
  GLabel lb_list_item;
  GButton btn_list_itemX;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  list.size(250, PCT(100));
  list.padding(10);
  list.align(ALIGN_LEFT);
  for(int i=0; i < list_opts.size(); i++){
    if(i==2) {
      lb_list_item = list.add_text("เมนู");
      lb_list_item.font(prasanmit_40);
    }

    btn_list_itemX = list.add(list_opts[i]);
    btn_list_itemX.button_type(BUTTON_SWITCH);
    btn_list_itemX.border(0);
    btn_list_itemX.radius(RADIUS_CIRCLE);
    btn_list_itemX.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN, 2), GSTATE_ON);
    btn_list_itemX.font_color(TFT_WHITE, GSTATE_ON);
  }
  list.onValueChanged(GWIDGET_CB{
    if(list.selected() =="") return; 
    Serial.println(list.selected());
    if(list.selected() == "ตั้งค่า"){
      // ให้ทำอะไร ...
    }else
    if(list.selected() == "เครื่องคิดเลข"){
      // ให้ทำอะไร ...
    }else
    if(list.selected() == "กล้องถ่ายรูป"){
      // ให้ทำอะไร ...
    }else
    if(list.selected() == "เกจจ์"){
      // ให้ทำอะไร ...
    }
  });
}

void loop() {
  BlynkGO.update();
}

