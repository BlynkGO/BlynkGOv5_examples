#include <BlynkGOv5.h>

IMAGE_DECLARE(ironman);

GImage image(ironman);

GContainer cont_btn(LAYOUT_ROW_M,10,10,10,10,5);
  GButton btn[3];
    const char* btn_str[3] = {"rotate", "normal", "scale x 1.5"};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  image.align(ALIGN_CENTER,0,-30);

  cont_btn.align(ALIGN_BOTTOM,0,-20);
  for(int i=0; i<3; i++ ){
    btn[i].parent(cont_btn);
    btn[i] = btn_str[i];
    btn[i].font(prasanmit_20);
    btn[i].height(35);
    btn[i].onClicked(GWIDGET_CB{
      static int ang = 0;
      int i= (GButton*)widget - &btn[0];
      switch(i){
        case 0:  image.angle(ang = (ang+30)%360);         break;
        case 1:  image.angle(ang = 0); image.zoom(100.0); break;
        case 2:  image.zoom(150.0);                       break;
      }
    });
  }
}

void loop(){
  BlynkGO.update();
}