#include <BlynkGOv5.h>

GLine line;
  Point points[] = { {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10} };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  line.setPoints(points,5);
  line.addPoint(200,150);
  line.addPoint(250.3,50);
  line.thickness(8, TFT_RED);
  line.rounded(true);
}

void loop(){
  BlynkGO.update();
}

