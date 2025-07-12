#include <BlynkGOv5.h>

GMultiScreen multiscreen;
  GLabel label[5];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // multiscreen.size(320,240);                 // กำหนดขนาดที่ต้องการ ไม่กำหนดจะเต็มจอ
  // multiscreen.color(TFT_BLUE);               // ใส่สีพื้นหลัง
  // multiscreen.image(your_background_image);  // ใส่รูปที่ต้องการทำพื้นหลัง
  multiscreen.addScreen(5);                     // เพิ่ม 5 screen ย่อย
  Serial.println( multiscreen.num());
  for(int i=0; i< 5; i++){
    label[i] = "หน้า" + String(i+1);
    label[i].parent(multiscreen[i]);            // วิตเจ็ตลูกที่ต้องการไปวางไว้ตาม หน้า screen ที่ต้องการ
  }
}

void loop(){
  BlynkGO.update();
}

