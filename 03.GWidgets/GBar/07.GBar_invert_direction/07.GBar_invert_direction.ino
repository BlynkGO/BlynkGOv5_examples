#include <BlynkGOv5.h>

GBar bar;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  bar.size(20, 200);
  bar.range(100, 0);    // แบบนี้วาดจากบนลงล่าง (หากกำหนดขนาดเป็นแนวตั้ง);  วาดจากขวาไปซ้าย (หากกำหนดขนาดเป็นแนวนอน)
  // bar.range(0,100);  // แบบนี้วาดจากล่างขึ้นบน (หากกำหนดขนาดเป็นแนวตั้ง);  วาดจากซ้ายไปขวา (หากกำหนดขนาดเป็นแนวนอน)
  bar = 70;
}

void loop()
{
  BlynkGO.update();
}

