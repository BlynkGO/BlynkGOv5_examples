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

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  list.size(300,220);
  list.options(list_opts);
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

