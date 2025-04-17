#include <BlynkGOv5.h>

FONT_DECLARE(material_icon_alarmtimer_40);

#define SYMBOL_TIMER_PLAY         "\uf4ba"
#define SYMBOL_TIMER_PAUSE        "\uf4bb"
#define SYMBOL_TIMER              "\ue425"
#define SYMBOL_TIMER_OFF          "\ue426"
#define SYMBOL_TIMER_ARROW_UP     "\uf377"
#define SYMBOL_TIMER_ARROW_DOWN   "\uf378"
#define SYMBOL_RESTART_ALT        "\uf053"

GContainer cont_toolbar;
  GLabel lb_icon[4] = { SYMBOL_TIMER_PLAY, SYMBOL_TIMER_PAUSE, SYMBOL_TIMER, SYMBOL_TIMER_OFF };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_toolbar.bg_opa(0);
  cont_toolbar.layout(LAYOUT_ROW_M, 5,5,20,20,20);
  cont_toolbar.align(ALIGN_BOTTOM_LEFT, 10,-10);
  cont_toolbar.font(material_icon_alarmtimer_40);   // กำหนด font จาก แม่ตรงนี้ได้เลย
  for(int i=0; i< 4 ;i++){
    lb_icon[i].parent(cont_toolbar);
    lb_icon[i].color(TFT_WHITE);
    lb_icon[i].color(TFT_COLOR_HSV(0,0,10), GSTATE_PRESSED);
    lb_icon[i].clickable(true);  // ให้สัมผัสได้
    lb_icon[i].onClicked(GWIDGET_CB{
      int ii = (GLabel*) widget - &lb_icon[0];
      Serial.println(ii);
      switch(ii){
        case 0: 
          // ให้ไปทำอะไร ...
          break;
        case 1: 
          break;
        case 2: 
          break;
        case 3: 
          break;
      }
    });
  }
}

void loop(){
  BlynkGO.update();
}
