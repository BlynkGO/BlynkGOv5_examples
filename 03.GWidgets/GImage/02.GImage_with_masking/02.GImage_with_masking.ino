#include <BlynkGOv5.h>

IMAGE_DECLARE(img_mask_icon_rounded_rect);

IMAGE_DECLARE(img_icon_computer);
IMAGE_DECLARE(img_icon_hdd);
IMAGE_DECLARE(img_icon_keyboard);

GContainer cont_icon1;
  GImage icons_1[3] = { {img_icon_computer} , {img_icon_hdd}, {img_icon_keyboard} };

GContainer cont_icon2;
  GImage icons_2[3] = { {img_icon_computer} , {img_icon_hdd}, {img_icon_keyboard} };

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_icon1.layout(LAYOUT_ROW_M,3,3,3,3,20);
  cont_icon1.bg_opa(0);
  cont_icon1.clickable(false);
  for(int i=0;i<3; i++){
    icons_1[i].parent(cont_icon1);
    icons_1[i].clickable(true);
    icons_1[i].image_recolor(TFT_BLACK, GSTATE_PRESSED);
    icons_1[i].image_recolor_opa(180, GSTATE_PRESSED);
    icons_1[i].onClicked(GWIDGET_CB{
      int ii = (GImage*) widget -&icons_1[0];
      label = StringX::printf("icons_1[%d]", ii);
    });
  }
  cont_icon1.align(ALIGN_CENTER,0,-50);

  cont_icon2.layout(LAYOUT_ROW_M,3,3,3,3,20);
  cont_icon2.bg_opa(0);
  cont_icon2.clickable(false);
  for(int i=0;i<3; i++){
    icons_2[i].parent(cont_icon2);
    icons_2[i].mask(img_mask_icon_rounded_rect);                // ใส่ mask เพื่อแสดงรูปเฉพาะตามที่ mask
    icons_2[i].clickable(true);
    icons_2[i].image_recolor(TFT_BLACK, GSTATE_PRESSED);
    icons_2[i].image_recolor_opa(180, GSTATE_PRESSED);
    icons_2[i].onClicked(GWIDGET_CB{
      int ii = (GImage*) widget -&icons_2[0];
      label = StringX::printf("icons_2[%d]", ii);
    });
  }
  cont_icon2.align(ALIGN_CENTER,0,50);

  label.align(cont_icon1, ALIGN_TOP,0,-20);
}

void loop(){
  BlynkGO.update();
}