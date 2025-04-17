/**************************************************************************************
 * 
 * GContainer เป็นวิตเจ็ต สำหรับรวม กลุ่มวิตเจ็ตอื่นๆเข้าด้วยกัน
 *   โดยจะมี API สำคัญ หลัก อยู่ 2 กลุ่ม API
 *   คือ
 *     
 *   1. API คำสั่ง layout(..)
 *      สำหรับจัดวางตำแหน่ง เลเยอร์ลูกๆ อัตโนมัติ ด้วย layout ที่กำหนด
 *      
 *      LAYOUT_OFF    : ไม่มีการกำหนด layout, เลเยอร์ลูกๆ ให้มีการกำหนดจัดตำแหน่งเอง
 *      LAYOUT_CENTER : จัด layout แบบตรงกลาง
 *      LAYOUT_COL_L  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว column ชิดซ้ายตรงกัน
 *      LAYOUT_COL_M  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว column ชิดแนวกลางตรงกัน
 *      LAYOUT_COL_R  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว column ชิดขวาตรงกัน
 *      LAYOUT_ROW_T  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว row (แถว) ให้ชิดด้านบนแถวตรงกัน
 *      LAYOUT_ROW_M  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว row (แถว) ให้ชิดกลางแถวตรงกัน
 *      LAYOUT_ROW_B  : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว row (แถว) ให้ชิดด้านล่างแถวตรงกัน
 *      LAYOUT_PRETTY : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนว แบบสวยงามอัตโนมัติ
 *      LAYOUT_GRID   : ให้เลเยอร์ลูกๆ จัดเรียงแบบแนวตาราง grid อัตโนมัติ
 *      
 *   2. API คำสั่งกลุ่มกำหนดระยะช่องไฟ padding(..), padding_XXX(...)
 *      และการกำหนด layout(..) จะมีการใช้ ระยะช่องไฟ มาเกี่ยวข้องด้วยเสมอ
 *      โดยสามารถกำหนดระยะช่องไฟได้จาก API ในกลุ่มนี้
 *      
 *      padding_top(..)    : ใช้กำหนดระยะช่องไฟด้านบน
 *      padding_bottom(..) : ใช้กำหนดระยะช่องไฟด้านล่าง
 *      padding_left(..)   : ใช้กำหนดระยะช่องไฟด้านซ้าย
 *      padding_right(..)  : ใช้กำหนดระยะช่องไฟด้านขวา
 *      padding_inner(..)  : ใช้กำหนดระยะช่องไฟภายในระหว่าง วิตเจ็ตลูกๆด้วยกัน
 *      padding_row(..)    : ใช้กำหนดระยะช่องไฟภายใน เฉพาะ ด้านแถว ระหว่างวิตเจ็ตลูกๆด้วยกัน
 *      padding_col(..)    : ใช้กำหนดระยะช่องไฟภายใน เฉพาะ ด้านคอลัมภ์ ระหว่างวิตเจ็ตลูกๆด้วยกัน
 *      padding(..)        : ใช้กำหนดระยะช่องไฟทุกด้าน ด้วยค่านี้ค่าเดียวให้ตรงกัน
 * 
 **************************************************************************************/


#include <BlynkGOv5.h>

GContainer cont(LAYOUT_COL_L,10,10,15,25,0);  // container ที่ให้ลูกๆ เรียงตัวอัตโนมัติตาม layout แบบ แนวคอลัมภ์ ซ้ายตรงกัน โดยมีช่องไฟ บน,ล่าง,ซ้าย,ขวา,ภายใน ตามกำหนด
  GLabel label1(cont);
  GLabel label2(cont);
  GLabel label3(cont);

GContainer cont2(LAYOUT_COL_R,10,10,15,25,5);  // container ที่ให้ลูกๆ เรียงตัวอัตโนมัติตาม layout แบบ แนวคอลัมภ์ ขวาตรงกัน
  GRect  rect1(cont2);
  GLabel label4(cont2);
  GRect  rect2(cont2);
  GRect  rect3(cont2);

GContainer cont3(LAYOUT_ROW_M,10,10,10,10,10);  // container ที่ให้ลูกๆ เรียงตัวอัตโนมัติตาม layout แบบ แนวแถว ตรงกลางตรงกัน
  GButton btn[4];

GRect screen[4];
  GLabel lb_screen[4];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  cont.align(ALIGN_TOP_LEFT,20,20);
    label1 = "Hello";
    label2 = "World";
    label3 = "BlynkGO";

  cont2.align(ALIGN_TOP_RIGHT,-20,20);
  cont2.color(TFT_PALETTE(TFT_PALETTE_ORANGE));
    rect1.size(60,25);  rect1.color(random(0xFFFF));
    label4 = "Krapao";
    rect2.size(80,15);  rect2.color(random(0xFFFF));
    rect3.size(40,20);  rect3.color(random(0xFFFF));

  cont3.align(ALIGN_BOTTOM,0,-10);
  cont3.border(1,TFT_BLACK);
    for(int i=0; i<4; i++){
      btn[i].parent(cont3);
      btn[i] = String("ปุ่ม ") + String(i+1);
      btn[i].font(prasanmit_20);
      btn[i].height(30);
      btn[i].onClicked(GWIDGET_CB{
        int i= (GButton*)widget - &btn[0];
        Serial.println( String("ปุ่ม ") + String(i+1)+ " กด");
        screen[i].show(true);
      });
    }
  
  for(int i=0; i<4; i++){
    screen[i].hidden(true);
    screen[i].size(PCT(80), PCT(80)); // ขนาด 80% ของหน้าจอ
    screen[i].border(1,TFT_BLACK);
    screen[i].color(random(0xFFFF));
    screen[i].corner_radius(10);
    screen[i].onClicked(GWIDGET_CB{
      int i= (GRect*)widget - &screen[0];
      screen[i].show(false);
    });
      lb_screen[i].parent(screen[i]);
      lb_screen[i] = "Screen " + String(i+1);
  }
}

void loop(){
  BlynkGO.update();
}

