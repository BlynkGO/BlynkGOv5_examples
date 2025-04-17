#include <BlynkGOv5.h>

FONT_DECLARE(material_icon_30);
#define SYMBOL_BACKSPACE    "\ue14a"

GContainer cont_keypad{
  GRID_COL { 15, 15, 15, 15, 15, 15, 15, 15, 15},
  GRID_ROW { 25, 35, 45, 45, 45, 45, 40},
};
  GLabel    lb_title("กำหนดราคาสินค้า", cont_keypad);     /* ข้อความไตเติ้ล */
  GTextArea ta_numpad(cont_keypad);                     /* ช่องกรอกข้อความ */
  GButton   btn[12]; 
    const char* btn_str[12] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", SYMBOL_BACKSPACE };
    enum { NUMPAD_0 = 0, NUMPAD_9 = 9, NUMPAD_DOT = 10, NUMPAD_BACKSPACE = 11 };
  GButton btn_ctrl[2];
    const char* btn_ctrl_str[2] = { "เคลียร์", "ตกลง"};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_keypad.size(240,320);
  cont_keypad.padding(5, 15, 15, 15, 5);  // กำหนดช่องไฟ
  cont_keypad.color(TFT_PALETTE(TFT_PALETTE_PURPLE));
  cont_keypad.align(ALIGN_LEFT, 20);
  cont_keypad.grid_align(GRID_ALIGN_CENTER, GRID_ALIGN_START);                                                                                                 

    /* จัดตำแหน่งแต่ละวิตเจ็ต ด้วย grid_cell(..) */
     /* lb_title ไว้ที่ cell (0,0) ขยายแนวนอนไป 9 ช่อง แนวตั้ง 1 ช่อง */
    lb_title.grid_cell (0, 0, 9, 1); 

    /* ta_numpad ไว้ที่ cell (0,1) ขยายแนวนอนไป 9 ช่อง แนวตั้ง 1 ช่อง และขนาดให้เต็มช่อง cell */
    ta_numpad.grid_cell(0, 1, 9, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    btn[0].grid_cell(3, 5, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[1].grid_cell(0, 2, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[2].grid_cell(3, 2, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[3].grid_cell(6, 2, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[4].grid_cell(0, 3, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[5].grid_cell(3, 3, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[6].grid_cell(6, 3, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[7].grid_cell(0, 4, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[8].grid_cell(3, 4, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[9].grid_cell(6, 4, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[10].grid_cell(0, 5, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    btn[11].grid_cell(6, 5, 3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

    btn_ctrl[0].grid_cell(0, 6, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_END);
    btn_ctrl[1].grid_cell(5, 6, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_END);


    lb_title.color(TFT_WHITE);
    lb_title.text_align(TEXT_ALIGN_CENTER);
    lb_title.font(prasanmit_25);
    ta_numpad.default_keyboard(false);      // ไม่ใช้แป้นพิมพ์หลัก
    ta_numpad.clickable(false);             // กด click สัมผัสไม่ได้
    ta_numpad.scrollbar(SCROLLBAR_AUTO, true, false); // ให้มี scrollbar เฉพาะแนวนอน
    ta_numpad.font(prasanmit_40);
    ta_numpad.padding_top(-12);             // เนื่องจากปรับขนาดความสูง น้อยกว่า ความสูง font ทำให้ข้อความในช่อง textarea ไม่ได้อยู่ตรงกับ ทำการปรับระยะช่องไฟบนให้น้อยลง
    ta_numpad.padding_bottom(-10);          // ทำการปรับระยะช่องไฟล่าง เสริม
    ta_numpad.padding_left(30);
    ta_numpad.hookDrawTask(true);
    ta_numpad.onDrawTask(GWIDGET_CB{
      if(DRAW_PART==DRAW_PART_MAIN && DRAW_TYPE_FILL){
        draw_label_dsc_t label_dsc;
        draw_label_dsc_init(&label_dsc);
        label_dsc.text  = "฿";
        label_dsc.font  = ta_numpad.font();
        label_dsc.color = TFT_COLOR(TFT_BLUE);
        area_t label_area;
        text_get_area(&label_area, &label_dsc);
        area_align(DRAW_AREA, &label_area, ALIGN_LEFT_MID, 8, 0);
        draw_label(DRAW_LAYER,&label_dsc, &label_area);
      }
    });

    for(int i=0; i<12; i++){
      btn[i].parent(cont_keypad);
      btn[i] = btn_str[i];

      btn[i].color(TFT_COLOR_HSV(0,0,95));
      btn[i].font_color(TFT_BLACK);
      btn[i].radius(5);

      if(i>=NUMPAD_0 && i <= NUMPAD_9 || i == NUMPAD_DOT)  btn[i].font(prasanmit_40);  // 0-9
      if(i== NUMPAD_BACKSPACE) btn[i].font(material_icon_30);

      btn[i].onClicked(GWIDGET_CB{
        int ii = (GButton*)widget - &btn[0];
        if(ii <= NUMPAD_9){
          int id_dot = ta_numpad.toString().indexOf(".");
          if( id_dot > 0) {
            if(ta_numpad.length() <= id_dot+2) {
              ta_numpad.add(btn_str[ii]);
            }
          }else{
            ta_numpad.add(btn_str[ii]);
          }          
        }else
        if(ii == NUMPAD_DOT) { // "."
          if( ta_numpad.toString().length() == 0) return;
          if( ta_numpad.toString().indexOf(".") <= 0) {  // ไม่มี จุดมาก่อน
            ta_numpad.add(".");
          }
        }else
        if(ii == NUMPAD_BACKSPACE) {
          ta_numpad.backspace();
        }
      });

      if( i < 2) {
        btn_ctrl[i].parent(cont_keypad);
        btn_ctrl[i] = btn_ctrl_str[i];
        btn_ctrl[i].height(40);
        btn_ctrl[i].radius(RADIUS_CIRCLE);
        btn_ctrl[i].font(prasanmit_20);
        btn_ctrl[i].onClicked(GWIDGET_CB{
          int ii=(GButton*)widget -&btn_ctrl[0];
          if(ii == 0) ta_numpad = "";
        });
      }
    }
}

void loop(){
  BlynkGO.update();
}
