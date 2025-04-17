// ตัวอย่าง GContainer  ที่มี LAYOUT_PRETTY กับการขยายขนาดลูกๆด้วย flex_grow(..)
// ให้ขยายขนาดอัตโนมัติ เต็มพื้นที่ด้านกว้าง ที่เหลือ ในบรรทัดนั้นๆ

#include <BlynkGOv5.h>

GContainer cont(LAYOUT_PRETTY);
  GRect rect1(cont);
  GRect rect2(cont);
  GRect rect3(cont);
  GRect rect4(cont);
  GRect rect5(cont);
  GRect rect6(cont);
  GRect rect7(cont);
  GRect rect8(cont);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.show(false);
  cont.width(300);                        // fix ขนาด
    //---------------------------------
    rect1.size(40,40);
    rect1.border(2,TFT_BLACK);

    rect2.flex_grow(2);                   // ขยายด้านกว้าง 2 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    rect2.height(40);
    rect2.border(2,TFT_BLACK);

    rect3.flex_grow(3);                   // ขยายด้านกว้าง 3 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    rect3.height(40);
    rect3.border(2,TFT_BLACK);

    rect4.size(40,40);                    // fix ขนาด  จะถูกขยับไปอยู่ริมขวาเต็มพื้นที่ (เมื่อหักลบ padding right)
    rect4.border(2,TFT_BLACK);

    //---------------------------------
    rect5.flex_new_line(true);            // ขึ้น flex บรรทัดใหม่
    rect5.size(PCT(100),40);              // ขยายด้านกว้าง 100% เต็ม แม่ (ภายในพื้นที่ เมื่อ หัก padding )
    rect5.border(2,TFT_BLACK);

    //---------------------------------
    rect6.flex_new_line(true);            // ขึ้น flex บรรทัดใหม่
    rect6.size(80,40);
    rect6.border(2,TFT_BLACK);

    rect7.flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    rect7.height(40);
    rect7.border(2,TFT_BLACK);

    rect8.size(60,40);
    rect8.border(2,TFT_BLACK);
    //---------------------------------

  cont.show(true);
}

void loop(){
  BlynkGO.update();
}
