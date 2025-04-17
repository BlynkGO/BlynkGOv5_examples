#include <BlynkGOv5.h>

GDropdownList ddlist;

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน",
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

GList list;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  //-----------------------------------
  // คำสั่ง ขนาด และตำแหน่ง
  //-------------------------
  ddlist.size(150,40);
  ddlist.align(ALIGN_TOP,0,20);

  //-----------------------------------
  // คำสั่งเฉพาะตัวของวิตเจ็ต GDropdownList
  //-------------------------
  ddlist.options( th_month, 12 );

  //-----------------------------------
  // คำสั่งดักเหตุการณ์ : เมื่อมีการเปลี่ยนแปลงค่า
  //-------------------------
  ddlist.onValueChanged(GWIDGET_CB{
    int selected_id     = ddlist.selected_id();
    String selected_str = ddlist.selected_str();
    Serial.println(selected_str);
  });

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  // ddlist.symbol(NULL);                   // ไม่แสดงสัญลักษณ์ลูกศร
  // ddlist.base_dir(DIR_LEFT);             // symbol อยู่ด้านซ้าย
  // ddlist.text_align(TEXT_ALIGN_LEFT);    // ข้อความให้อยู่ชิดซ้าย
  ddlist.font(prasanmit_25, TFT_COLOR_RGB(204,204,204));
  ddlist.color(TFT_COLOR_RGB(24,24,24));
  ddlist.border(0);
  ddlist.list_border(1,TFT_SILVER);
  // ddlist.list_padding_right(80);         // ขยายช่องไฟของ list ด้านขวาไปอีก 80px
  ddlist.selected_color( TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,3));   // สีของ item ที่ถูกเลือก ใน list
  // ddlist.selected_font_color(TFT_CYAN);  // สีฟอนต์ของ item ที่เลือกถูกเลือกใน list
  
}

void loop(){
  BlynkGO.update();
}
