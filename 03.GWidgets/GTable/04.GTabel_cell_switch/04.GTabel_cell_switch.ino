#include <BlynkGOv5.h>

#define ITEM_CNT 200

GTable table;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  table.height(220);
  table.border(2, TFT_BLUE);

  table.cells(ITEM_CNT, 1);     /* ให้ table มีจำนวน row จองไว้ 200 แถว และ column 1 หลัก */

  table.row_height(40);         /* ให้ทุก row สูง 40 px วางหลัง กำหนดจำนวน cells(...) */
  table.column(0).width(150);   /* column 0 กว้าง 150px */
  table.font(prasanmit_20);     /* ฟอนต์ทั้งตาราง ใช้ prasanmit_20 */

  for(int r = 0; r < ITEM_CNT; r++) {
    table.cell(r,0) = StringX::printf("Item %d", r + 1);
    table.cell(r,0).clickable(false);       /* กำหนด cell ไม่ให้ click ได้  (เพื่อเวลา draw กราฟิกระดับล่าง ไม่ต้อง draw เวลามีการคลิก cell แบบตามปกติ) */
  }

  table.cell(2,0).ON();
  table.cell(3,0).ON();

  /* ทำการ hook การ draw กราฟิกระดับล่าง  โดยมีการวาด switch ขึ้นมาเอง ช่วยให้ประหยัดเม็มได้ กว่าการใช้ วิตเจ็ต GSwitch */
  table.hookDrawTask(true);
  table.onDrawTask(GWIDGET_CB{
    if( DRAW_PART == DRAW_PART_CELL  && DRAW_TYPE == DRAW_TYPE_FILL ){    /* หากการ draw กราฟิกระดับล่าง กำลังวาดในส่วนของ cell และ เป็นประเภท fill พื้นที่สี่เหลี่ยมของตัว cell */
      auto cell = table.draw_cell();              /* cell ที่กำลังถูก draw กราฟิก ระดับล่าง*/
      bool cell_isON = table.cell(cell).isON();   /* สถานะ ของ cell ที่กำลังถูกวาด มี สถานะเป็น ON ใช่ไหม */
      
      /*ทำการ draw กราฟิกระดับล่าง ในส่วนของ พื้นหลัง ของ switch เอง */
      area_t sw_area = { 0, 0, 40, 24 };
      area_align(DRAW_AREA, &sw_area, ALIGN_RIGHT_MID, -15, 0); /* ให้พื้นที่วาด พื้นหลัง ของ switch อยู่ชิดขวา ของพื้นที่ DRAW_AREA พื้นที่ที่กำลังวาด cell ปัจจุบัน */

      draw_rect_dsc_t switch_rect_dsc;
      draw_rect_dsc_init(&switch_rect_dsc);
      switch_rect_dsc.bg_color = TFT_COLOR( cell_isON ?  TFT_PALETTE(TFT_PALETTE_BLUE) : TFT_PALETTE_LIGHTEN(TFT_PALETTE_GRAY, 2) );
      switch_rect_dsc.radius   = RADIUS_CIRCLE;
      draw_rect(DRAW_LAYER, &switch_rect_dsc, &sw_area);

      /* draw กราฟิกระดับล่าง ในส่วน knob ของ switch เอง */
      area_t knob_area = { 0, 0, 18, 18};
      if(cell_isON) { area_align(&sw_area, &knob_area, ALIGN_RIGHT_MID , -3, 0); }  /* ให้พื้นที่วาด knob ของ switch อยู่ชิดขวา ของพื้นที่ switch */
      else          { area_align(&sw_area, &knob_area, ALIGN_LEFT_MID  ,  3, 0); }  /* ให้พื้นที่วาด knob ของ switch อยู่ชิดซ้าย ของพื้นที่ switch */      

      draw_rect_dsc_t knob_rect_dsc;
      draw_rect_dsc_init(&knob_rect_dsc);
      knob_rect_dsc.bg_color = TFT_COLOR(TFT_WHITE);
      knob_rect_dsc.radius   = RADIUS_CIRCLE;
      draw_rect(DRAW_LAYER, &knob_rect_dsc, &knob_area);
    }
  });

  /* หาก cell มีการถูก click มา ให้ เพิ่ม สถานะ ON/OFF สลับสถานะ เข้าไปด้วย เพิ่อใช้สำหรับ เป็นตัวเช็ค ในตอน draw ระดับล่าง */
  table.onValueChanged(GWIDGET_CB{
    auto cell       = table.selected_cell();
    table.cell(cell.row, cell.col).toggle();      /* สลับสถานะ ON-OFF ของ cell */

    String cell_str = table.cell(cell.row, cell.col).toString();
    bool cell_isON  = table.cell(cell.row, cell.col).isON();
    Serial.printf("[cell %d,%d] %s : %s\n", cell.row, cell.col, cell_str.c_str(), cell_isON? "ON" : "OFF");
  });
  
}

void loop(){
  BlynkGO.update();
}

