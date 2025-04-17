#include <BlynkGOv5.h>

GRoller roller1, roller2, roller3;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);
  
  static GStyle style_selected;
    style_selected.font(prasanmit_40);
    style_selected.color(TFT_COLOR_HEX(0xff8888));
    style_selected.border(2, TFT_COLOR_HEX(0xff0000));

  static auto roller_onClicked = 
    GWIDGET_CB{
      GRoller * roller = (GRoller*) widget;
      Serial.println( roller->toString());
    };

  roller1.options(1,10,true, ROLLER_MODE_NORMAL);
  roller1.visible_rows(2);
  roller1.width(100);
  roller1.addStyle(style_selected, GPART_SELECTED);
  roller1.bg_color( TFT_COLOR_HEX(0x00ff00), TFT_COLOR_HEX(0xaaffaa));
  roller1.align(ALIGN_LEFT,10);
  roller1.selected(2);
  roller1.onClicked( roller_onClicked );

  roller2.options(1,10,true, ROLLER_MODE_NORMAL);
  roller2.visible_rows(3);
  roller2.addStyle(style_selected, GPART_SELECTED);
  roller2.selected(5);
  roller2.onClicked( roller_onClicked );

  roller3.options(1,10,true, ROLLER_MODE_NORMAL);
  roller3.visible_rows(4);
  roller3.width(80);
  roller3.addStyle(style_selected, GPART_SELECTED);
  roller3.text_align(TEXT_ALIGN_RIGHT);
  roller3.align(ALIGN_RIGHT, -10);
  roller3.selected(8);
  roller3.onClicked( roller_onClicked );

}

void loop(){
  BlynkGO.update();
}

