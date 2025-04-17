#include <BlynkGOv5.h>

IMAGE_DECLARE(img_beenext);

GIcon beenext(img_beenext, "BeeNeXT");

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // beenext.font(prasanmit_20);
  beenext.onClicked(GWIDGET_CB{
    Serial.println( "Clicked" );
  });
}

void loop(){
  BlynkGO.update();
}


