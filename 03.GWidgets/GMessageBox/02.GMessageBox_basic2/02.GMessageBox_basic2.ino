#include <BlynkGOv5.h>

GButton btn;

GMessageBox msgbox;
  GButton btn_apply(msgbox);
  GButton btn_cancel(msgbox);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  btn = "ปุ่มทดสอบ";
  btn.onClicked(GWIDGET_CB{
    msgbox.title("กล่องข้อความ");
    msgbox.message("นี่คือกล่องข้อความ พร้อม ปุ่ม 2 ปุ่ม");
    btn_apply  = msgbox.footer_button("ตกลง");      // GButton btn_apply  สามารถไปตบแต่งเพิ่มเติม หรือ กำหนดสัมผัส ภายหลังได้
    btn_cancel = msgbox.footer_button("ยกเลิก");     // GButton btn_cancel  สามารถไปตบแต่งเพิ่มเติม หรือ กำหนดสัมผัส ภายหลังได้

    btn_apply.onClicked(GWIDGET_CB{
      Serial.println("[msgbox] Apply!");
      msgbox.close();
    });

    btn_cancel.onClicked(GWIDGET_CB{
      Serial.println("[msgbox] Cancel!");
      msgbox.close();
    });
  });


}

void loop()
{
  BlynkGO.update();
}
