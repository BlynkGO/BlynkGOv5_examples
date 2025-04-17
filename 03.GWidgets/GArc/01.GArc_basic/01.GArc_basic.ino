#include <BlynkGOv5.h>

/*************************************************************
 * ตย.1 : arc แบบปกติ  กำหนดด้วย ค่า value ใน range ที่กำหนด
 * ตย.2 : arc แบบปกติ กำหนดด้วย มุม  ( ARC_MODE_REVERSE จะไม่มีผลอะไร )
 * ตย.3 : เหมือน ตย.1 แต่กลับทิศ
 * ตย.4 : arc แบบสมมาตร
 *************************************************************/

#define ARC_EXAMPLE    1    // เปลี่ยนค่าเป็น 1 หรือ 2 หรือ 3 หรือ 4

GArc arc;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  //-----------------------------------
  // คำสั่ง ขนาด และตำแหน่ง
  //-------------------------
  arc.size(100, 100);
  arc.align(ALIGN_CENTER);

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  // arc.bg_color(TFT_SILVER);      // สี ของ arc พื้นหลัง
  // arc.bg_opa(180);               // ความโปร่งใส ของ arc พื้นหลัง

  // arc.color(TFT_GREEN);          // สี ของ arc
  // arc.thickness(20);             // ปรับความหนาของ arc
  // arc.thickness(20, TFT_GREEN);  // กำหนดความหนา และสี ไปพร้อมๆกัน
  // arc.opa(200);                  // ความโปร่งใส ของ arc
  // arc.rounded(true);             // ให้ปลาย arc โค้งมน

  //-----------------------------------
  // คำสั่ง เฉพาะตัว สำหรับ วิตเจ็ต GArc
  //-------------------------
  // arc.mode(ARC_MODE_SYMMETRICAL);// กำหนด arc mode : ARC_MODE_NORMAL โหมดปกติ, ARC_MODE_REVERSE โหมดกลับทิศ, ARC_MODE_SYMMETRICAL โหมดสมมาตร
  // arc.bg_enable(true);           // ให้แสดง arc พื้นหลังด้วย
  // arc.angle_offset(120);         // มุมเริ่มต้น นับจากแกน X-axis ทางคณิตศาสตร์ (หมุนตามเข็ม) : ค่าเริ่มต้น 135 องศา
  // arc.bg_angle(0,270);           // arc พื้นหลัง เริ่มแสดงจาก มุม 0-270 องศา นับจาก มุมเริ่มต้น angle_offset (โดย arc.bg_enable(true); ด้วยถึงจะมองเห็น )
  // arc.angle(45,150);             // วาด arc แบบ กำหนดด้วยมุม โดยเริ่มจาก มุมเริ่มต้น angle_offset หมุนตามเข็มเสมอ ไม่ขึ้นกับ arc mode
  // arc.range(0,100);              // range ของค่าที่จะใช้กับ arc (หากไม่ใส่ จะเป็นช่วง 0-100 อัตโนมัติ)
  // arc = 70;                      // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด
  // Serial.println(arc.toInt())    // ดึงค่า ออกมาจาก arc

  //-----------------------------------

#if (ARC_EXAMPLE==1)
  // ตย.1 : arc แบบปกติ  กำหนดด้วย ค่า value ใน range ที่กำหนด
  arc.bg_enable(true);              // ให้แสดง arc พื้นหลังด้วย : ค่าเริ่มต้น arc พื้นหลัง จะไม่แสดง
  // arc.range(0, 100);             // กำหนด range : ค่าเริ่มต้น range 0-100
  arc = 0;                          // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด


  static SoftTimer timer_1;
  timer_1.setInterval(10, []() {
    static int v = 0;
    static int s = 1;
    if(s ==  1 && v >=  arc.max_value()) {
      s = -1;
    }
    if(s == -1 && v <=  arc.min_value()) {
      s = 1;
    }
    arc   = (v = v + s);
  });
#endif

#if (ARC_EXAMPLE==2)
  // ตย.2 : arc แบบปกติ กำหนดด้วย มุม  ( ARC_MODE_REVERSE จะไม่มีผลอะไร )
  arc.bg_enable(true);              // ให้แสดง arc พื้นหลังด้วย : ค่าเริ่มต้น arc พื้นหลัง จะไม่แสดง
  arc.angle_offset(180);            // มุมเริ่มต้น นับจากแกน X-axis ทางคณิตศาสตร์ (หมุนตามเข็ม)
  arc.bg_angle(0, 270);             // arc พื้นหลัง เริ่มแสดงจาก มุม 0-270 องศา นับจาก มุมเริ่มต้น angle_offset
  arc.angle(0, 0);                  // วาด arc แบบ กำหนดด้วยมุม 0 องศา ถึง 0 องศา โดยเริ่มจาก มุมเริ่มต้น angle_offset หมุนตามเข็ม

  static SoftTimer timer_2;
  timer_2.setInterval(10, []() {
    static int angle = 0;
    static int s = 2;
    if(s ==  2 && angle >=  270) {
      s = -2;
    }
    if(s == -2 && angle <=  0) {
      s = 2;
    }
    arc.angle(0, angle = angle + s);
  });
#endif

#if (ARC_EXAMPLE==3)
  // ตย.3 : เหมือน ตย.1 แต่กลับทิศ
  arc.mode(ARC_MODE_REVERSE);       // กำหนด arc mode : ARC_MODE_REVERSE โหมดกลับทิศ
  arc.bg_enable(true);              // ให้แสดง arc พื้นหลังด้วย : ค่าเริ่มต้น arc พื้นหลัง จะไม่แสดง
  // arc.range(0, 100);             // กำหนด range : ค่าเริ่มต้น range 0-100
  arc = 0;                          // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด

  static SoftTimer timer_3;
  timer_3.setInterval(10, []() {
    static int v = 0;
    static int s = 1;
    if(s ==  1 && v >=  arc.max_value()) {
      s = -1;
    }
    if(s == -1 && v <=  arc.min_value()) {
      s = 1;
    }
    arc   = (v = v + s);
  });
#endif

#if (ARC_EXAMPLE==4)
  // ตย.4 : arc แบบสมมาตร
  arc.mode(ARC_MODE_SYMMETRICAL);   // ARC_MODE_SYMMETRICAL โหมดสมมาตร โดยจะมีจุดกึ่งกลาง range เป็นค่าสมมาตร
  arc.bg_enable(true);              // ให้แสดง arc พื้นหลังด้วย : ค่าเริ่มต้น arc พื้นหลัง จะไม่แสดง
  arc.range(-100, 100);             // กำหนด range
  arc = -10;                        // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด

  static SoftTimer timer_4;
  timer_4.setInterval(10, []() {
    static int v = 0;
    static int s = 1;
    if(s ==  1 && v >=  arc.max_value()) {
      s = -1;
    }
    if(s == -1 && v <=  arc.min_value()) {
      s = 1;
    }
    arc   = (v = v + s);
  });
#endif

}

void loop()
{
  BlynkGO.update();
}

