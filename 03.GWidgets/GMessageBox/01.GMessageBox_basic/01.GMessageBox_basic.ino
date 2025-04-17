#include <BlynkGOv5.h>

GMessageBox msgbox;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  msgbox.title("กล่องข้อความ");                       // ข้อความบน titlebar
  msgbox.message("นี่คือกล่องข้อความ พร้อม ปุ่ม 2 ปุ่ม");    // ข้อความเนื้อหา ของ message box
  msgbox.close_button();                            // สร้าง ปุ่มปิด บน titlebar
  msgbox.footer_button("ตกลง");                     // สร้าง ปุ่ม "ตกลง" บน พื้นที่ footer ด้านล่าง
  msgbox.footer_button("ยกเลิก");                    // สร้าง ปุ่ม "ยกเลิก" บน พื้นที่ footer ด้านล่าง
}

void loop()
{
  BlynkGO.update();
}

