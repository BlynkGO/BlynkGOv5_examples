#include <BlynkGOv5.h>

FONT_DECLARE(google_icon_trend_20);

#define SYMBOL_TRENDING_UP        "\ue8e5"
#define SYMBOL_TRENDING_FLAT      "\ue8e4"
#define SYMBOL_TRENDING_DOWN      "\ue8e3"
#define SYMBOL_TIMELINE           "\ue922"
#define SYMBOL_CHART_DATA         "\ue473"
#define SYMBOL_MENU               "\ue5d2"

GButton btn;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  btn.setSymbol(SYMBOL_TRENDING_UP);
  btn.symbol_font(google_icon_trend_20, TFT_RED);
  btn.symbol_translate_y(2);
  btn = "Hello";

  btn.onClicked(GWIDGET_CB{
    static int c=0;
    c = (++c)%5;
    switch(c){
      case 0: btn.setSymbol(SYMBOL_TRENDING_UP);    break;
      case 1: btn.setSymbol(SYMBOL_TRENDING_FLAT);  break;
      case 2: btn.setSymbol(SYMBOL_TRENDING_DOWN);  break;
      case 3: btn.setSymbol(SYMBOL_TIMELINE);       break;
      case 4: btn.setSymbol(SYMBOL_CHART_DATA);     break;
      case 5: btn.setSymbol(SYMBOL_MENU);           break;
    }
  });
}

void loop(){
  BlynkGO.update();  
}