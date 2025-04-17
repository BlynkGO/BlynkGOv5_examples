#include <BlynkGOv5.h>

GButton btn;                            // ปุ่มแบบ BUTTON_PUSH   (กดติด ปล่อยดับ)
GButton btn_switch(BUTTON_SWITCH);      // ปุ่มแบบ BUTTON_SWITCH (กดติด กดดับ)
GButton btn_home;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  btn = "ปุ่มกด";
  btn.align(ALIGN_CENTER, 0, -btn.height() / 2 - 10);
  btn.onClicked(GWIDGET_CB {                   // ปุ่มแบบ BUTTON_PUSH ให้ดักสัมผัส clicked ด้วย onClicked
    Serial.println("[btn] Clicked");
  });

  btn_switch = "ปุ่มสวิตซ์";
  btn_switch.align(btn, ALIGN_BOTTOM, 0, 20);
  btn_switch.ON();
  btn_switch.onValueChanged(GWIDGET_CB {       // ปุ่มแบบ BUTTON_SWITCH ให้ดักการเปลี่ยนแปลง ด้วย onValueChanged
    Serial.printf("[btn_switch] %s\n", btn_switch.isON() ? "ON" : "OFF");
  });

  btn_home = SYMBOL_HOME;                       // สัญลักษณ์ดูได้ที่ ตาราง ในหน้า https://github.com/BlynkGO/BlynkGO_font
  btn_home.round_design();                      // ออกแบบให้เป็นแบบโค้งมน
  btn_home.align(ALIGN_TOP_RIGHT, -10, 10);
  btn_home.onClicked(GWIDGET_CB {
    Serial.println("[HOME] clicked");
  });

}

void loop()
{
  BlynkGO.update();
}

