#include <BlynkGOv5.h>

IMAGE_DECLARE(img_laundry);

GButton btn;
GButton btn_with_image(img_laundry, "เครื่องซักผ้า");

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  GScreen.layout(LAYOUT_COL_M, 0, 0, 0, 0, 30);
  GScreen.content_align(CONTENT_ALIGN_CENTER);

  btn = "ปุ่มกด";

  //-----------------------------------
  // คำสั่ง ขนาด และตำแหน่ง
  //-------------------------
  // btn.size(150,100);                   // เปลี่ยนขนาดที่ต้องการเอง
  // btn.width(150);                      // เปลี่ยนเฉพาะด้านกว้างเอง
  // btn.height(100);                     // เปลี่ยนเฉพาะด้านสูงเอง

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  // btn.font(prasanmit_40, TFT_BLACK);   // กำหนด font และสี font
  // btn.color(TFT_CYAN);                 // สีพื้นหลัง
  btn.color(TFT_VIOLET, GSTATE_PRESSED);  // สีพื้นหลัง ขณะมีสถานะถูกกด
  // btn.border(3, TFT_BLUE);             // ขอบ และ สีขอบ
  // btn.rounded(true);                   // ปุ่มทำแบบโค้งมน
  // btn.shadow(30, 5, btn.color());      // shadow width, spread, color  ( ตัวอย่าง shadow spread ประมาณ 1/6 ของ shadow width)
  // btn.shadow_offset(6,6);              // shadow offset

  //-----------------------------------
  // คำสั่งดักเหตุการณ์ : เมื่อมีการสัมผัส click เข้ามาที่ปุ่ม ให้ดักด้วย  onClicked(...)
  // หากเป็นปุ่มแบบ BUTTON_SWITCH ให้ดักเมื่อมีการเปลี่ยนแปลงค่า ด้วย onValueChanged(...)
  //-------------------------
  btn.onClicked(GWIDGET_CB {
    Serial.println("Clicked");
  });

  btn_with_image.rounded(true);
  btn_with_image.padding_hor(30);         // ระยะช่องไฟ แนวนอน (ซ้าย & ขวา)
  btn_with_image.onClicked(GWIDGET_CB {
    Serial.println("Laundry Clicked");
  });

}

void loop()
{
  BlynkGO.update();
}
