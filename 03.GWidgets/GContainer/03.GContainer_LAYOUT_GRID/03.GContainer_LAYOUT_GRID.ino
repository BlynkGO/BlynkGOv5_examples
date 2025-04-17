#include <BlynkGOv5.h>

IMAGE_DECLARE(img_deadpool);
IMAGE_DECLARE(img_captain_america);
IMAGE_DECLARE(img_spiderman);
IMAGE_DECLARE(img_hulk);
IMAGE_DECLARE(img_thor);

GContainer cont_icons(LAYOUT_GRID,10,10,5,5,4);
  GIcon icon[]={
    {img_deadpool         , "Deadpool"  , cont_icons},
    {img_captain_america  , "Captain A" , cont_icons},
    {img_spiderman        , "SpiderMan" , cont_icons},
    {img_hulk             , "Hulk"      , cont_icons},
    {img_thor             , "Thor"      , cont_icons},
  };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  cont_icons.width(320);          // ให้กำหนด fix ความกว้างด้วยเสมอ   เมื่อใช้ LAYOUT แบบ GRID 
  cont_icons.bg_opa(0);
  for(int i=0; i<5; i++){
    icon[i].width(100);           // วิตเจ็ตลูกๆ ให้กำหนดกว้างให้เท่าๆกัน  เมื่อใช้ LAYOUT แบบ GRID 
    icon[i].onClicked(GWIDGET_CB{
      int i = (GIcon*)widget - &icon[0];
      Serial.println(icon[i].text());
    });
  }
}

