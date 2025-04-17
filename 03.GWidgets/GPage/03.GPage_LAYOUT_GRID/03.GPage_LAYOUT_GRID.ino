#include <BlynkGOv5.h>

GPage page(LAYOUT_GRID,10,10,25,25,15);
  GContainer box[8];
    GLabel   label[8];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  page.size(300,200);
  page.border(2);
  page.color(TFT_WHITE);
  for(int i=0; i<8; i++){
    box[i].parent(page);
    box[i].width(70);
    box[i].border(2);
    box[i].color(TFT_WHITE);
      label[i].parent(box[i]);
      label[i] = String(i+1);
  }
}

void loop(){
  BlynkGO.update();
}
