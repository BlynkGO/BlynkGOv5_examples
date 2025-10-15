#include <BlynkGOv5.h>

GContainer cont_led;
  GLed led[5];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.window_title("My App");

  cont_led.bg_opa(0);
  cont_led.layout(LAYOUT_ROW_M, 10,10,10,10,20);
  for(int i=0; i<5; i++){
    led[i].parent(cont_led);
    led[i].checkable(true);
    led[i].onValueChanged(GWIDGET_CB{
      int ii = (GLed*) widget- &led[0];
      Serial.printf("[LED %d] %s\n", ii, led[ii].isON()? "ON": "OFF");
    });
  }
}

void loop(){
  BlynkGO.update();
}

