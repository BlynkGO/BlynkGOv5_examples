#include <BlynkGOv5.h>

GDropdownList ddlist[4];
 String ddlist_options[] = { "Apple","Banana","Orange", "Melon"};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  for(int i=0; i<4; i++){
    ddlist[i].options(ddlist_options , 4);
    ddlist[i].size(120,45);
  }

  ddlist[0].align(ALIGN_TOP,0,10);
  
  ddlist[1].align(ALIGN_BOTTOM,0,-10);
  ddlist[1].dir(DIR_BOTTOM);
  ddlist[1].symbol(SYMBOL_UP);

  ddlist[2].align(ALIGN_LEFT,10,0);
  ddlist[2].dir(DIR_RIGHT);
  ddlist[2].symbol(SYMBOL_RIGHT);

  ddlist[3].align(ALIGN_RIGHT,-10,0);
  ddlist[3].dir(DIR_LEFT);
  ddlist[3].symbol(SYMBOL_LEFT);

}

void loop(){
  BlynkGO.update();
}
