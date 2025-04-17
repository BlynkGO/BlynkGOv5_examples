#include <BlynkGOv5.h>

GButtonMatrix btnm;
  const char* btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                            "6", "7", "8", "9", "0", "\n",
                            "Action1", "Action2", ""          // ให้ปิด btnm_map ตัวสุดท้ายด้วย "" เสมอ
                           };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  //-----------------------------------
  // คำสั่ง ตำแหน่ง / ขนาด
  //------------------------
  btnm.width(PCT(90));                                        // กว้าง 90% ของหน้าจอ

  //-----------------------------------
  // คำสั่งกำหนดปุ่มต่างๆภายใน btnm 
  // และ ดักเหตุการณ์ เมื่อมีการเปลี่ยนแปลงค่า
  //-------------------------
  btnm.map(btnm_map);
  btnm.button_width(10, 2);                                   // ปุ่ม id = 10 มีขนาดกว้าง 2 เท่าจาก กว้างของปุ่มปกติ ในแถวปุ่มเดียวกันนั้น
  btnm.button_ctrl(10, BUTTONMATRIX_CTRL_CHECKABLE);          // ปุ่ม id = 10 ให้ เป็นปุ่มแบบ checkable ได้  (กดติด กดปล่อย - toggle)
  btnm.button_ctrl(11, BUTTONMATRIX_CTRL_CHECKED);            // ปุ่ม id = 11 ให้ เป็นปุ่ม มีสถานะ ถูก checked อยู่
  btnm.onValueChanged(GWIDGET_CB{
    uint32_t  selected_id     = btnm.selected_id();
    String    selected_button = btnm.selected_button();
    Serial.printf("[%d] %s\n", selected_id, selected_button.c_str());
  });

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  btnm.color(TFT_PALETTE(TFT_PALETTE_CYAN));                  // สีพื้นหลัง ของ btnm
  btnm.padding_row(5);                                        // ระยะช่องไฟระหว่างแถว ของปุ่มภายใน btnm
  btnm.padding_column(10);                                    // ระยะช่องไฟระหว่างคอลัมน์ ของปุ่มภายใน btnm
  btnm.font(prasanmit_25);                                    // ฟอนต์ของ ปุ่มภายใน btmn
  // btnm.button_radius(RADIUS_CIRCLE);                       // รัศมีของปุ่มภายใน btmn ให้เป็นแบบวงกลม
  // btnm.button_border(2, TFT_BLUE);                         // ขอบของปุ่มภายใน btmn
}

void loop(){
  BlynkGO.update();
}
