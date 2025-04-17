#include <BlynkGOv5.h>

GPage page(LAYOUT_ROW_M);
  GButton btn[10];

GPage page2(LAYOUT_COL_M);
  GButton btn2[10];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  page.size(PCT(80),75);
  for(int i=0; i<10; i++){
    btn[i].parent(page);
    btn[i].height(40);
    btn[i] = String("Item : ") + String(i);
    btn[i].onClicked(GWIDGET_CB{
      int ii = (GButton*)widget -&btn[0];
      Serial.printf("[btn] %d ถูกกด\n", ii);
    });
  }
  page.align(ALIGN_TOP,0,5);

  page2.height(150);
  for(int i=0; i<10; i++){
    btn2[i].parent(page2);
    btn2[i].height(40);
    btn2[i] = String("Item : ") + String(i);
    btn2[i].onClicked(GWIDGET_CB{
      int ii = (GButton*)widget -&btn2[0];
      Serial.printf("[btn2] %d ถูกกด\n", ii);      
    });
  }
  page2.align(page, ALIGN_BOTTOM,0,5);
}

void loop(){
  BlynkGO.update();
}