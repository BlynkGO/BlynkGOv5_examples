
#include <BlynkGOv5.h>

IMAGE_DECLARE(img_gauge_mask);

GMask mask(img_gauge_mask);
  GContainer cont_bar(LAYOUT_ROW_M,5,5,5,5,3, mask);
    GRect bar[30];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  cont_bar.bg_opa(0);
  for(int i=0;i<30; i++){
    bar[i].parent(cont_bar);
    bar[i].size(10,100);
    bar[i].color(TFT_COLOR_HSV(map(i,0,29,0,240),100,100));
  }

  static GAnimation anim;
  anim.init(cont_bar,[](GWIDGET, int32_t v){
    for(int i=0; i<30; i++){
      bar[i].color( i <= map(v,0,100,0,29)? TFT_COLOR_HSV(map(i,0,29,0,240),100,100) : TFT_COLOR_HSV(0,0,15));      
    }
  },0,100,1000,true,true);
  anim.create();

}

void loop(){
  BlynkGO.update();
}
