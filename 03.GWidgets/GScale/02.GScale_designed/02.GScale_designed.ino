#include <BlynkGOv5.h>

GScale scale;
  
void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  scale.size(70,200);
  scale.mode(SCALE_MODE_VERTICAL_RIGHT,21,5);   // แบบแนวตั้งด้านขวา มีเส้นขีด ทั้งหมด 21 ขีด เส้นขีดหลัก เว้นทุกๆ 5 ขีด
  scale.tick_length(10,5);                      // ความยาว เส้นขีดหลัก 10px ความยาวเส้นขีดทั่วไป 5px
  scale.range(0,100);                           // scale อยู่ในช่วง 0-100

  scale.label_show(true);                       // แสดงข้อความที่เส้นขีดหลักด้วย
  static const char * custom_labels[] = {"0 °C", "25 °C", "50 °C", "75 °C", "100 °C", NULL};
  scale.text_src(custom_labels);                // ข้อความ ที่ เส้นขีดหลัก

  scale.font_color(TFT_BLUE);   // สีข้อความ
  scale.thickness(2, TFT_BLUE); // ความหนาของเส้นขีด และสี

  static GScaleSection section;
    section = scale.add_section();
    section.range(75,100);
    section.font(prasanmit_20, TFT_RED);
    section.thickness(3, TFT_RED);

  /* ตบแต่ง พื้นหลัง */
  scale.bg_color(TFT_PALETTE(TFT_PALETTE_BLUE_GREY)); // สีพื้นหลัง
  scale.bg_opa(OPA_50);   // ความโปร่งใสพื้นหลัง
  scale.radius(8);        // รัศมีหัวมุม
  scale.pad_hor(8);       // ช่องไฟด้านซ้ายและขวา
  scale.pad_ver(20);      // ช่องไฟด้านบนและล่าง
  
}

void loop(){
  BlynkGO.update();
}

