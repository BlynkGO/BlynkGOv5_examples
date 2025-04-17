#include <BlynkGOv5.h>

GContainer frame_setting(
  GRID_COL {100, 350}, 
  GRID_ROW {50, 20, 65, 65, 65}
);
  GLabel lb_title(frame_setting);
  GLine  ln_title(frame_setting);
  GLabel lb_info[3] = { "ชื่อ", "นามสกุล", "รหัส"};
  GTextArea ta_value[3];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  frame_setting.padding(10,20,30,30,5);
  frame_setting.bg_opa(0); 
  frame_setting.font_color(TFT_WHITE);
  frame_setting.border(1, TFT_WHITE);

    lb_title = "ตั้งค่า";
    lb_title.grid_cell( 0, 0, 2, 1);            /* ไว้ที่ cell (0,0) โดยขยาย span แนวนอน 2 ช่อง แนวตั้ง 1 ช่อง*/
    lb_title.text_align(TEXT_ALIGN_CENTER);     /* ให้ข้อความจัดอยู่กลาง */

    ln_title.setPoints(Point(0,0), frame_setting.content_width());
    ln_title.color(TFT_SILVER);
    ln_title.grid_cell( 0, 1, 2, 1,             /* ไว้ที่ cell (0,1) โดยขยาย span แนวนอน 2 ช่อง แนวตั้ง 1 ช่อง*/
                        GRID_ALIGN_START, GRID_ALIGN_CENTER, 0, -5);  /* ให้วางวิตเจ็ตเส้น แนวนอนชิดเริ่มต้น, แนวตั้งอยู่กลาง และ ปรับระยะเลื่อนขึ้น 5px */ 
  
    for(int r=0; r<3; r++){
      lb_info[r] .parent(frame_setting);
      lb_info[r] .grid_cell(0, r+2, GRID_ALIGN_START, GRID_ALIGN_CENTER); /* ไว้ที่ cell (0, r+2) ให้ แนวนอน ชิดเริ่มต้น แนวตั้งอยู่กลาง */
      ta_value[r].parent(frame_setting);
      ta_value[r].grid_cell(1, r+2, GRID_ALIGN_START, GRID_ALIGN_CENTER); /* ไว้ที่ cell (1, r+2) ให้ แนวนอน ชิดเริ่มต้น แนวตั้งอยู่กลาง */
      ta_value[r].font(prasanmit_25);
      if(r == 2) ta_value[r].password_mode(true);
    }

}

void loop(){
  BlynkGO.update();
}

