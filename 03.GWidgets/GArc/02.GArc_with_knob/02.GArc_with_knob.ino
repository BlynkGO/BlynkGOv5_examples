#include <BlynkGOv5.h>

GArc arc_with_knob;
GLabel label;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  //-----------------------------------
  // คำสั่ง ขนาด และตำแหน่ง
  //-------------------------
  arc_with_knob.size(100, 100);
  arc_with_knob.align(ALIGN_CENTER);

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  arc_with_knob.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED, 3));    // สี ของ arc
  arc_with_knob.bg_enable(true);
  arc_with_knob.rounded(true);
  arc_with_knob.knob.enable(true);  // ให้มีปุ่ม ให้การปรับค่าหรือไม่ ถ้ามี knob หากมีการดัก onValueChanged(...) เมื่อมีการเปลี่ยนแปลงค่า ก็จะสามารถนำไปทำอะไรๆได้
  arc_with_knob.knob.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_CYAN, 2)); // สีของปุ่ม knob
  // arc_with_knob.knob.opa(0);     // ให้ knob มองไม่เห็น แต่ปรับค่าด้วยสัมผัสได้อยู่เมื่อ knob_enable(true);
  // arc_with_knob.knob.padding(5); // ปรับขยายขนาด ปุ่ม knob เพิ่มจาก ระยะ thick ของ arc
  // arc_with_knob.knob.border(3, TFT_WHITE); // ขอบ และสีขอบของ ปุ่ม knob

  //-----------------------------------
  // คำสั่ง เฉพาะตัว สำหรับ วิตเจ็ต GArc
  //-------------------------
  arc_with_knob = 70;

  //-----------------------------------
  // คำสั่งดักเหตุการณ์ : เมื่อมีการเปลี่ยนแปลงค่า
  //-------------------------
  arc_with_knob.onValueChanged(GWIDGET_CB{
    label = arc_with_knob.toInt();                        // ดึงค่าของ arc_with_knob มาใส่วิตเจ็ตข้อความ
    arc_with_knob.align_GWidget_to_angle(label, 25);      // จัดเรียง วิตเจ็ต ข้อความ  โดยอยู่ตำแหน่ง ตาม มุมของ arc_with_knob ห่าง 25px จากรัศมี arc_with_knob  (ใส่ระยะติดลบได้)
    // arc_with_knob.rotate_GWidget_to_angle(label, 25);  // หมุนเอียง วิตเจ็ต ข้อความ โดยอยู่ตำแหน่ง ตาม มุมของ arc_with_knob ห่าง 25px จากรัศมี arc_with_knob  (ใส่ระยะติดลบได้)
  });

  //------------------------------------
  // วิตเจ็ตข้อความ
  //--------------------------

  label.font(prasanmit_25, TFT_BLACK);

}

void loop()
{
  BlynkGO.update();
}

