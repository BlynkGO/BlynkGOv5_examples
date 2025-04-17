#include <BlynkGOv5.h>

GButton btn;

GMessageBox msgbox;
  GRect   titlebar(msgbox);
    GLabel title(titlebar);
  GContainer body(msgbox);
    GLabel message(body);
  GContainer footer(msgbox);
    GButton btn_ok(footer);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  btn = "ปุ่มทดสอบ";
  btn.onClicked(GWIDGET_CB{
    title       = msgbox.title("กล่องข้อความ");                      // GLabel title    สามารถไปตบแต่งเพิ่มเติมภายหลังได้
    message     = msgbox.message("นี่คือกล่องข้อความ พร้อม ปุ่มตกลง");   // GLabel message  สามารถไปตบแต่งเพิ่มเติมภายหลังได้
    btn_ok      = msgbox.footer_button("ตกลง");                    // GButton btn_ok  สามารถไปตบแต่งเพิ่มเติมภายหลังได้

    titlebar    = msgbox.titlebar();                               // GRect titlebar   สามารถไปตบแต่งเพิ่มเติมภายหลังได้
    body        = msgbox.body();                                   // GContainer body  ส่วน body ตรงกลาง สามารถไปตบแต่งเพิ่มเติมภายหลังได้
    footer      = msgbox.footer();                                 // GContainer footer ส่วนพื้นที่รวม ปุ่มด้านล่าง สามารถไปตบแต่งเพิ่มเติมภายหลังได้

    // ตบแต่งภายหลังได้ เช่น ปรับ layout ให้ footer
    footer.layout(LAYOUT_RIGHT);    // ให้ เลย์เอาท์ ชิดขวา
    footer.padding_right(20);

    btn_ok.onClicked(GWIDGET_CB{
      Serial.println("[msgbox] OK!");
      msgbox.close();
    });
  });

}

void loop()
{
  BlynkGO.update();
}
