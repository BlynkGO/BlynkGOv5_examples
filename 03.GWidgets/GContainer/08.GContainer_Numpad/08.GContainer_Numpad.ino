#include <BlynkGOv5.h>

GContainer cont_keypad{
  GRID_COL { 60, 60, 60, 60},
  GRID_ROW { 80, 60, 60, 60, 60, 60},
};
  GTextArea ta_numpad(cont_keypad);
  GButton btn[17];
    enum {
      NUMPAD_0        = 0,
      NUMPAD_9        = 9,
      NUMPAD_DOT      = 10,
      NUMPAD_PLUS     = 11,
      NUMPAD_MINUS    = 12,
      NUMPAD_MULTIPLE = 13,
      NUMPAD_DIVISOR  = 14,
      NUMPAD_CLEAR    = 15,
      NUMPAD_ENTER    = 16
    };
    const char* btn_str[17] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
                                ".", "+", "-", "*", "/", 
                                "num\nclear", "enter"};


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_keypad.padding(5, 15, 15, 15, 5);  // กำหนดช่องไฟ
  cont_keypad.color(TFT_COLOR_HSV(0,0,20));
  cont_keypad.align(ALIGN_LEFT, 20);
    ta_numpad.default_keyboard(false);    // ไม่ใช้แป้นพิมพ์หลัก
    ta_numpad.clickable(false);           // กด click สัมผัสไม่ได้
    ta_numpad.scrollbar(SCROLLBAR_AUTO, true, false); // ให้มี scrollbar เฉพาะแนวนอน
    ta_numpad.font(prasanmit_40);
    ta_numpad.height(60);
    ta_numpad.padding_top(-1);            // เนื่องจากปรับขนาดความสูง น้อยกว่า ความสูง font ทำให้ข้อความในช่อง textarea ไม่ได้อยู่ตรงกับ ทำการปรับระยะช่องไฟบนให้น้อยลง
    ta_numpad.padding_bottom(0);          // ทำการปรับระยะช่องไฟล่าง เสริม

    for(int i=0; i<17; i++){
      btn[i].parent(cont_keypad);
      btn[i] = btn_str[i];

      btn[i].color(TFT_COLOR_HSV(0,0,95));
      btn[i].font_color(TFT_BLACK);
      btn[i].radius(5);

      if(i>=NUMPAD_0 && i <= NUMPAD_9)  btn[i].font(prasanmit_40);  // 0-9
      if(i>=NUMPAD_CLEAR) btn[i].font(prasanmit_20);                // num clear, enter
      if(i==NUMPAD_CLEAR) btn[i].text_line_space(-10);              // num clear ให้ข้อความขึ้นบรรทัดใหม่มีระยะห่างระหว่างบรรทัดลดลง 10px

      btn[i].onClicked(GWIDGET_CB{
        int ii = (GButton*)widget - &btn[0];
        if(ii <= NUMPAD_9){
          ta_numpad.add(btn_str[ii]);
        }else
        if(ii == NUMPAD_DOT) { // "."
          if( ta_numpad.toString().length() == 0) return;
          if( ta_numpad.toString().indexOf(".") <= 0) {  // ไม่มี จุดมาก่อน
            ta_numpad.add(".");
          }
        }else
        if(ii == NUMPAD_CLEAR) {
          ta_numpad = "";
        }else
        if(ii == NUMPAD_ENTER) {
          /* หากมีการกด enter จะให้ทำอะไร วางตรงนี้ */
        }
        
      });
    }

    /* จัดตำแหน่งแต่ละวิตเจ็ต ด้วย grid_cell(..) ย้ายตำแหน่ง  */
    ta_numpad.grid_cell(0, 0, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    btn[0].grid_cell(0, 5, 2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[1].grid_cell(0, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[2].grid_cell(1, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[3].grid_cell(2, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[4].grid_cell(0, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[5].grid_cell(1, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[6].grid_cell(2, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[7].grid_cell(0, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[8].grid_cell(1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[9].grid_cell(2, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    btn[10].grid_cell(2, 5, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[11].grid_cell(3, 2, 1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[12].grid_cell(3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[13].grid_cell(2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[14].grid_cell(1, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    btn[15].grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[16].grid_cell(3, 4, 1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    /* จัดตำแหน่งแต่ละวิตเจ็ต ด้วย grid_cell(..) จัดวางอีกแบบ */
    // ta_numpad.grid_cell(0, 0, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    // btn[0].grid_cell(1, 5, 2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[1].grid_cell(0, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[2].grid_cell(1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[3].grid_cell(2, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[4].grid_cell(0, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[5].grid_cell(1, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[6].grid_cell(2, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[7].grid_cell(0, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[8].grid_cell(1, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[9].grid_cell(2, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    // btn[10].grid_cell(0, 5, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[11].grid_cell(3, 2, 1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[12].grid_cell(3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[13].grid_cell(2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[14].grid_cell(1, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    // btn[15].grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    // btn[16].grid_cell(3, 4, 1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
}

void loop(){
  BlynkGO.update();
}

