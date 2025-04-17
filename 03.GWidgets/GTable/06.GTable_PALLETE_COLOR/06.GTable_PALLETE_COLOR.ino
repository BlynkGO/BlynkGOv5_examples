#include <BlynkGOv5.h>

GTable table;
struct mycolor {
  palette_t palette_color;
  String   palette_str;
};

#define COLOR_NUM   TFT_PALETTE_LAST

mycolor mycolor[] = {
  { TFT_PALETTE_RED, "TFT_PALETTE_RED"},
  { TFT_PALETTE_PINK, "TFT_PALETTE_PINK" },
  { TFT_PALETTE_PURPLE, "TFT_PALETTE_PURPLE" },
  { TFT_PALETTE_DEEP_PURPLE, "TFT_PALETTE_DEEP_PURPLE" },
  { TFT_PALETTE_INDIGO, "TFT_PALETTE_INDIGO" },
  { TFT_PALETTE_BLUE, "TFT_PALETTE_BLUE" },
  { TFT_PALETTE_LIGHT_BLUE, "TFT_PALETTE_LIGHT_BLUE" },
  { TFT_PALETTE_CYAN, "TFT_PALETTE_CYAN" },
  { TFT_PALETTE_TEAL, "TFT_PALETTE_TEAL" },
  { TFT_PALETTE_GREEN, "TFT_PALETTE_GREEN" },
  { TFT_PALETTE_LIGHT_GREEN, "TFT_PALETTE_LIGHT_GREEN" },
  { TFT_PALETTE_LIME, "TFT_PALETTE_LIME" },
  { TFT_PALETTE_YELLOW, "TFT_PALETTE_YELLOW" },
  { TFT_PALETTE_AMBER, "TFT_PALETTE_AMBER" },
  { TFT_PALETTE_ORANGE, "TFT_PALETTE_ORANGE" },
  { TFT_PALETTE_DEEP_ORANGE, "TFT_PALETTE_DEEP_ORANGE" },
  { TFT_PALETTE_BROWN, "TFT_PALETTE_BROWN" },
  { TFT_PALETTE_BLUE_GREY, "TFT_PALETTE_BLUE_GREY" },
  { TFT_PALETTE_GREY, "TFT_PALETTE_GREY" },
  { TFT_PALETTE_GRAY, "TFT_PALETTE_GRAY" }
};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  table.cells(COLOR_NUM+1,7);
  table.align(ALIGN_TOP,0,10);
  table.column_width(100);
  table.row_height(23);
  table.font(prasanmit_15);

  table.column(3).width(200);
  table.row(0).height(30);
  table.row(0).font(prasanmit_20);
  table.text_align(TEXT_ALIGN_CENTER);

  for(int r=0; r< COLOR_NUM+1; r++){
    if(r==0){
      table.merge_cell_right(0, 0, true);           /* ผสานรวม cell (0,0) กับ cell ด้านขวาเข้าด้วยกัน */ 
      table.merge_cell_right(0, 1, true);           /* ผสานรวม cell (0,1) กับ cell ด้านขวาเข้าด้วยกัน */
      table.cell(0,0) = "TFT_PALETTE_LIGHTEN";
      /* หลังผสานเซล์ จะได้ cell (0,0) ถึง (0,2) รวมกันเป็น cell เดียวกัน */

      table.cell(0,3) = "สีแบบ PALETTE";

      table.merge_cell_right(0, 4, true);           /* ผสานรวม cell (0,4) กับ cell ด้านขวาเข้าด้วยกัน */ 
      table.merge_cell_right(0, 5, true);           /* ผสานรวม cell (0,5) กับ cell ด้านขวาเข้าด้วยกัน */
      /* หลังผสานเซล์ จะได้ cell (0,4) ถึง (0,6) รวมกันเป็น cell เดียวกัน */

      table.cell(0,4) = "TFT_PALETTE_DARKEN";
      continue;
    }
    for(int c=0; c< 7; c++){
      if(c<3){
        int lighten_i = 3-c;
        table.cell(r,c).color(TFT_PALETTE_LIGHTEN(mycolor[r-1].palette_color,lighten_i));
        table.cell(r,c) = String(lighten_i);
      }else
      if(c==3){
        table.cell(r,c).color(TFT_PALETTE(mycolor[r-1].palette_color));
        table.cell(r,c) = mycolor[r-1].palette_str;
      }else{
        int darken_i = c-3;
        table.cell(r,c).color(TFT_PALETTE_DARKEN(mycolor[r-1].palette_color,darken_i));
        table.cell(r,c) = String(darken_i);
      }
    }
  }
}

void loop(){
  BlynkGO.update();
}