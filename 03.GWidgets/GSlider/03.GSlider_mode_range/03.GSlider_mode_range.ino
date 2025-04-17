#include <BlynkGOv5.h>

GSlider slider;
GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  slider.mode(SLIDER_MODE_RANGE);
  slider.range(0,100);
  slider.value_range(20,70);
  slider.onValueChanged(GWIDGET_CB{
    GArea area = slider.indic_area();
    label = StringX::printf("%d - %d", slider.left_value(), slider.value());
    label.center((area.p[0].x+ area.p[1].x)/2, (area.p[0].y-20));
  });
  
  static SoftTimer timer;
  timer.delay(100,[](){
    slider.event_send(EVENT_VALUE_CHANGED);
  });
}
}

void loop(){
  BlynkGO.update();
}
