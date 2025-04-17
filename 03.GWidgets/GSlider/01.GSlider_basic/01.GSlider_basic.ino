#include <BlynkGOv5.h>

GSlider slider;
GLabel  label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  // slider.color(TFT_RED);
  // slider.range(0,200);
  slider.onValueChanged(GWIDGET_CB{
    label = String(slider.toInt()) + String("%");
  });
  label = "0%";
  label.align(slider, ALIGN_BOTTOM,0,10);

}

void loop(){
  BlynkGO.update();
}

