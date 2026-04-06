// ตัวอย่างเฉพาะสำหรับ บน PC มีการใช้เมาส์ เมื่อเมาส์เลื่อนมาบน วิตเจ็ต
// มีการปรับ design วิตเจ็ต เมื่อเมาส์เลื่อนมาบน วิตเจ็ตนั้นๆ

#include <BlynkGOv5.h>

GRect card;
  GRect card2(card);
  GLabel label(card);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  card.size(SIZE_CONTENT, SIZE_CONTENT);
  card.padding(6,0,0,0);
  card.color(TFT_CYAN);
  card.border(0);
  card.shadow(0);
  card.font(prasanmit_40, TFT_WHITE);
  card.radius(10);

  // กำหนด attribute ต่างๆ สำหรับเมื่อเมาส์เลื่อนมาบน card (สำหรับ BlynkGO กระเพราหมูกรอบ บน PC)
  card.border(1, TFT_CYAN, GSTATE_HOVERED);
  card.shadow(10, 3, TFT_CYAN, GSTATE_HOVERED); // กำหนด ระยะกว้างแสงเงา และ ระยะฟุ้ง แสงเงา และสีแสงเงา เมื่อเมาส์เลื่อนมาบน card
  card.shadow_offset(0, 0, GSTATE_HOVERED);     // กำหนดค่า ความเยื้องของแสงเงา เมื่อเมาส์เลื่อนมาบน card
  card.transform_scale(270, GSTATE_HOVERED);  // 255 ขนาดเท่าเดิม  270 ขยายใหญ่ขึ้น เมื่อเมาส์เลื่อนมาบน card
  card.font_color(TFT_WHITE, GSTATE_DEFAULT); // สีฟอนต์ ขณะปกติ
  card.font_color(TFT_RED, GSTATE_HOVERED);   // สีฟอนต์ ขณะเมาส์เลื่อนมาบน card

    label = "Hello ทดสอบ";                    // label จะมีสีฟอนต์ ตาม card ที่กำหนด

    card2.size(label.width()+40, label.height());
    card2.color(TFT_BLACK);
    card2.radius(card.radius());
    card2.clickable(false);
    card2.border(1, TFT_CYAN);
    card2.border_side(BORDER_SIDE_BOTTOM | BORDER_SIDE_RIGHT | BORDER_SIDE_LEFT);
    card2.border_opa(120);
    card2.layer_id(0);      // ย้ายไปไว้ล่างสุด ของ parent 'card'

}

void loop(){
  BlynkGO.update();
}

