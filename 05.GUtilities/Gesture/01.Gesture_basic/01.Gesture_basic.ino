#include <BlynkGOv5.h>

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  GScreen.onGesture(GWIDGET_CB{
    auto gest_dir = GScreen.gesture_dir();
    Serial.println(gest_dir);
  });
}

void loop() {
  BlynkGO.update();
}