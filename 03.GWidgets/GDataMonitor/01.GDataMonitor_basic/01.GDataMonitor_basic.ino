#include <BlynkGOv5.h>
#include "GDataMonitor.h"

GDataMonitor DataMon;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  DataMon.size(300, 200);
  // DataMon.font(prasanmit_30);  // เปลี่ยน font
  
  static SoftTimer timer;
  timer.setInterval(1000,[](){
    DataMon.printf("Data %d\n", random(0xFFFF));
  });
}

void loop(){
  BlynkGO.update();
}

