#include <BlynkGOv5.h>

const char* row_0_text1[] = { "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "T1", "T2", "T3", "T4", "T5", "T6"};
const char* row_0_text2[] = { "C10", "C11", "C12", "C13", "C14", "C15", "C16", "C17", "C18", "BTemp", "BHum", "T I/O", "T Min", "T Avg", "T Max"};
const char* col_0_text[]  = { "1A", "1B", "2A", "2B", "3A", "3B", "4A", "4B", "5A", "6B" };

GTable table;
GLabel label;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  table.cells(11,16);                       /* table มี row 11 แถว มี column 16 คอลัมภ์ */
  table.align(ALIGN_BOTTOM); 

  /**************************************************/
  /* ปรับเปลี่ยน ความกว้างของ column หรือ ความสูงของ row */
  /**************************************************/
  table.column_width(800/table.columns());  /* ความกว้างทุก column */
  table.row_height(30);                     /* ความสูงทุก row */
  table.row(0).height(40);                  /* ความสูงเฉพาะ row 0 */

  /********************************************************/
  /* ปรับเปลี่ยน design ตาราง, column, row, cell ได้ตามต้องการ */
  /********************************************************/
  table.font(prasanmit_18, TFT_BLACK);

  table.pad_right(0, DRAW_PART_CELL);       /* ช่องไฟด้านขวาของ cell เป็น 0 */
  table.pad_left(0, DRAW_PART_CELL);        /* ช่องไฟด้านซ้ายของ cell เป็น 0 */

  for(int r=0; r< table.rows(); r++) {
    table.row(r).align((r==0)? TEXT_ALIGN_RIGHT :TEXT_ALIGN_CENTER );  /* เฉพาะ แถวแรก ข้อความชิดขวา ที่เหลือ ไว้ตรงกลาง cell */
  }

  table.row(0).color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,3));
  table.row(0).font_color(TFT_WHITE);
  table.row(0).text_offset(-5,5);

  table.column(0).color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,3));
  table.column(0).font_color(TFT_WHITE);

  table.cell(0,0).color(TFT_WHITE);
  table.cell(0,0).font(prasanmit_40, TFT_BLACK);
  table.cell(0,0).align(TEXT_ALIGN_CENTER);
  table.cell(0,0).text_offset(0,-15);

  /**************************************/
  /* hook ขณะมีการ draw กราฟิกระดับล่าง     */
  /**************************************/
  table.hookDrawTask(true);
  table.onDrawTask(GWIDGET_CB{
    auto r = table.draw_row();    /* cell ณ แถว r ที่กำลัง draw ระดับล่าง */
    auto c = table.draw_col();    /* cell ณ คอลัมภ์ c ที่กำลัง draw ระดับล่าง */

    if(DRAW_PART== DRAW_PART_CELL && DRAW_TYPE == DRAW_TYPE_FILL) { /* เมื่อขณะ draw ในส่วน cell ของ table และ กำลัง draw ประเภท fill ช่อง cell */
      if( r==0 && c >= 1) {         /* เฉพาะ แถว แรก และ column 1 เป็นต้นไป */
        /* draw กราฟิกระดับล่าง ส่วนข้อความเสริม จากปกติ */
        draw_label_dsc_t label_dsc;
          draw_label_dsc_init(&label_dsc);
          label_dsc.text  = row_0_text1[c-1];
          label_dsc.font  = table.font();
          label_dsc.color = TFT_COLOR(TFT_YELLOW);
          label_dsc.ofs_x = 5;
          label_dsc.ofs_y = -8;
        area_t label_area;
          text_get_area(&label_area, label_dsc.text, label_dsc.font);
          area_align(DRAW_AREA, &label_area, ALIGN_LEFT_MID );
        draw_label(DRAW_LAYER, &label_dsc, &label_area);    /* draw ข้อความ ระดับล่าง เสริมให้ cell ในแถวแรก แบบนี้ไม่เปลืองเมมในการจอง วิตเจ็ต */
      }
      
      if( r > 0 && c > 0) {
        auto fill_dsc = table.draw_fill_dsc();    /* ดึง fill_dsc ที่กำลังใช้ในขณะกำลัง draw กราฟิกระดับล่าง */
        if ((r - 1) / 2 % 2 ) {                   /* เฉพาะ cell ที่มี row คือ 3,4, 7,8 */
          if(table.cell(r,c).isPressed()){        /* cell ที่กำลัง draw กราฟิกระดับล่าง มีสถานะถูกกด หรือไม่ */
            /* เปลี่ยนสีพื้นของ cell ด้วย สี pallete โทนเหลืองอ่อน ที่ผสมสีดำให้ดูเข้มขึ้น */
            fill_dsc->color = TFT_COLOR( TFT_COLOR_MIX( TFT_PALETTE_LIGHTEN(TFT_PALETTE_YELLOW,2), TFT_BLACK, 200) );  
          }else{
            /* สี pallete โทนเหลืองอ่อน */
            fill_dsc->color = TFT_COLOR(TFT_PALETTE_LIGHTEN(TFT_PALETTE_YELLOW,2)); 
          }
        }
      }
    }
  });

  table.onDrawMainEnd(GWIDGET_CB{

  });

  /***********************/
  /* กำหนดค่าให้ cell ต่างๆ */
  /***********************/
  table.cell(0,0)  = SYMBOL_HOME;
  for(int c=1; c < table.columns(); c++){
    table.cell(0, c) = row_0_text2[c-1];
  }
  for(int r=1; r < table.rows(); r++){
    table.cell(r,0) = col_0_text[r-1];
  }

  /* เติม data ในช่องต่างๆ ได้ตามต้องการ */
  table.cell(1,1) = 0;   table.cell(1,2) = 1;   table.cell(1,3) = 2;   table.cell(1,4) = 3;   table.cell(1,5) = 4;
  table.cell(1,6) = 5;   table.cell(1,7) = 6;   table.cell(1,8) = 7;   table.cell(1,9) = 8;   table.cell(1,10) = 9;
  table.cell(1,11) = 10; table.cell(1,12) = 11; table.cell(1,13) = 12; table.cell(1,14) = 13; table.cell(1,15) = 14;

  table.cell(2,1) = 15;  table.cell(2,2) = 16;  table.cell(2,3) = 17;  table.cell(2,4) = 18;  table.cell(2,5) = 19;
  table.cell(2,6) = 20;  table.cell(2,7) = 21;  table.cell(2,8) = 22;  table.cell(2,9) = 23;  table.cell(2,10) = 24;
  table.cell(2,11) = 25; table.cell(2,12) = 26; table.cell(2,13) = 27; table.cell(2,14) = 28; table.cell(2,15) = 29;

  table.cell(3,1) = 30;  table.cell(3,2) = 31;  table.cell(3,3) = 32;  table.cell(3,4) = 33;  table.cell(3,5) = 34;
  table.cell(3,6) = 35;  table.cell(3,7) = 36;  table.cell(3,8) = 37;  table.cell(3,9) = 38;  table.cell(3,10) = 39;
  table.cell(3,11) = 40; table.cell(3,12) = 41; table.cell(3,13) = 42; table.cell(3,14) = 43; table.cell(3,15) = 44;

  table.cell(4,1) = 45;  table.cell(4,2) = 46;  table.cell(4,3) = 47;  table.cell(4,4) = 48;  table.cell(4,5) = 49;
  table.cell(4,6) = 50;  table.cell(4,7) = 51;  table.cell(4,8) = 52;  table.cell(4,9) = 53;  table.cell(4,10) = 54;
  table.cell(4,11) = 55; table.cell(4,12) = 56; table.cell(4,13) = 57; table.cell(4,14) = 58; table.cell(4,15) = 59;

  table.cell(5,1) = 60;  table.cell(5,2) = 61;  table.cell(5,3) = 62;  table.cell(5,4) = 63;  table.cell(5,5) = 64;
  table.cell(5,6) = 65;  table.cell(5,7) = 66;  table.cell(5,8) = 67;  table.cell(5,9) = 68;  table.cell(5,10) = 69;
  table.cell(5,11) = 70; table.cell(5,12) = 71; table.cell(5,13) = 72; table.cell(5,14) = 73; table.cell(5,15) = 74;

  table.cell(6,1) = 75;  table.cell(6,2) = 76;  table.cell(6,3) = 77;  table.cell(6,4) = 78;  table.cell(6,5) = 79;
  table.cell(6,6) = 80;  table.cell(6,7) = 81;  table.cell(6,8) = 82;  table.cell(6,9) = 83;  table.cell(6,10) = 84;
  table.cell(6,11) = 85; table.cell(6,12) = 86; table.cell(6,13) = 87; table.cell(6,14) = 88; table.cell(6,15) = 89;

  table.cell(7,1) = 90;  table.cell(7,2) = 91;  table.cell(7,3) = 92;  table.cell(7,4) = 93;  table.cell(7,5) = 94;
  table.cell(7,6) = 95;  table.cell(7,7) = 96;  table.cell(7,8) = 97;  table.cell(7,9) = 98;  table.cell(7,10) = 99;
  table.cell(7,11) = 100; table.cell(7,12) = 101; table.cell(7,13) = 102; table.cell(7,14) = 103; table.cell(7,15) = 104;

  table.cell(8,1) = 105; table.cell(8,2) = 106; table.cell(8,3) = 107; table.cell(8,4) = 108; table.cell(8,5) = 109;
  table.cell(8,6) = 110; table.cell(8,7) = 111; table.cell(8,8) = 112; table.cell(8,9) = 113; table.cell(8,10) = 114;
  table.cell(8,11) = 115; table.cell(8,12) = 116; table.cell(8,13) = 117; table.cell(8,14) = 118; table.cell(8,15) = 119;

  table.cell(9,1) = 120; table.cell(9,2) = 121; table.cell(9,3) = 122; table.cell(9,4) = 123; table.cell(9,5) = 124;
  table.cell(9,6) = 125; table.cell(9,7) = 126; table.cell(9,8) = 127; table.cell(9,9) = 128; table.cell(9,10) = 129;
  table.cell(9,11) = 130; table.cell(9,12) = 131; table.cell(9,13) = 132; table.cell(9,14) = 133; table.cell(9,15) = 134;

  table.cell(10,1) = 135; table.cell(10,2) = 136; table.cell(10,3) = 137; table.cell(10,4) = 138; table.cell(10,5) = 139;
  table.cell(10,6) = 140; table.cell(10,7) = 141; table.cell(10,8) = 142; table.cell(10,9) = 143; table.cell(10,10) = 144;
  table.cell(10,11) = 145; table.cell(10,12) = 146; table.cell(10,13) = 147; table.cell(10,14) = 148; table.cell(10,15) = 149;

  /****************************************** */
  /* เมื่อ user มีการกดเลือกมาที่ cell ใดๆ ของ ตาราง */
  /****************************************** */
  table.onCellClicked(GWIDGET_CB{
    auto cell = table.selected_cell();
    auto cell_str = table.cell(cell).toString();

    if( cell.row ==0 && cell.col > 1) {
      label = String( row_0_text1[cell.col-1]) + " " + cell_str;
    }else{
      label = cell_str;
    }
  });

  /******************/
  /* ออกแบบวิตเจ็ตอื่นๆ */
  /******************/
  label.font(prasanmit_40);
  label.align(table, ALIGN_TOP,0, -20);
}

void loop()
{
  BlynkGO.update();
}
