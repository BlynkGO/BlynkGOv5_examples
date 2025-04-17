#include <BlynkGOv5.h>

GSlider slider;
GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  slider.range(100,0); 
  slider.onValueChanged(GWIDGET_CB{
    label = String(slider.toInt())+ String("%");
  });
  label.align(slider, ALIGN_BOTTOM);

  slider = 35;
  slider.event_send(EVENT_VALUE_CHANGED);

}

void loop(){
  BlynkGO.update();
}
