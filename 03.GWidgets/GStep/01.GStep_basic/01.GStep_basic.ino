#include <BlynkGOv5.h>

GStep step;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  step = 20;
  // step.range(0,1000);                  // กำหนด range
  // step.decimal(2);                     // กำหนด ให้แสดงทศนิยม กี่ตำแหน่ง
  // step.step(0.1);                      // กำหนด ให้ step ไปค่าทีละ 0.1  (ปกติ คือ 1)
  // step.font(prasanmit_40, TFT_RED);    // เปลี่ยน font
  // step.button_size(50,50);             // เปลียนขนาดปุ่มกด
  // step.button_color(TFT_GREEN);        // เปลี่ยนสีพื้นปุ่มกด
  // step.button_radius(5);               // เปลี่ยนรัศมีปุ่มกด
  step.onValueChanged(GWIDGET_CB{
    int my_step = step.toInt();           // step.toInt(), step.toFloat(), step.toDouble() 
    Serial.println(my_step);
  });

}

void loop(){
  BlynkGO.update();
}

