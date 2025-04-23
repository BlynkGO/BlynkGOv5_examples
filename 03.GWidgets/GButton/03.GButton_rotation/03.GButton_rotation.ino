#include <BlynkGOv5.h>

GButton btn;
GLabel  label;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  btn = "ปุ่มกด";
  btn.pivot(btn.centerpoint());   // จุดหมุนที่จุดกึ่งกลางปุ่ม
  btn.rotation(90.0);             // หมุนไป 90 องศา (การหมุนจะมีการกินพื้นที่ เม็ม ควรใช้บน BeeNeXT ขนาด 4.3 นิ้วขึ้นไป)
  btn.align(ALIGN_LEFT, 10);      // วางชิดซ้ายจอ เข้าไป 10px
  btn.onClicked(GWIDGET_CB {
    static int c;
    label = String("ปุ่มกด ") + String(++c);
  });

}

void loop()
{
  BlynkGO.update();
}

