#include <BlynkGOv5.h>
#include "GMyGauge.h"

GMultiScreen multiscreen;
  GMyGauge gauge[5];
    String title[5] = { "ห้องทำงาน", "ห้องครัว", "ห้องนอน", "ห้องนั่งเล่น", "ห้องรับแขก"};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  multiscreen.addScreen(5);
  for(int i= 0; i< 5; i++){
    gauge[i].parent(multiscreen[i]);
    gauge[i].title(title[i]);
    gauge[i].value( random(6000, 10000)/100.0);    // ตุ่มตัวเลข 60.00 -100.00 
  }
}

void loop(){
  BlynkGO.update();
}
