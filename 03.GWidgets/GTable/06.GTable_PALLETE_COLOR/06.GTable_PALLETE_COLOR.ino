#include <BlynkGOv5.h>

GTable table;
struct mycolor {
  palette_t palette_color;
  String   palette_str;
};
GRect  box_palette_selected;
GLabel lb_palette_cmd;

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
  { TFT_PALETTE_BLUE_GRAY, "TFT_PALETTE_BLUE_GRAY" },
  { TFT_PALETTE_GRAY, "TFT_PALETTE_GRAY" }
};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.window_title("BlynkGOv5 : สี Palette");

  table.cells(COLOR_NUM+1,7);
  table.align(ALIGN_TOP,0,10);
  table.column_width(100);
  table.row_height(23);
  table.font(prasanmit_15);

  table.column(3).width(200);
  table.row(0).height(22);
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

  table.onCellClicked(GWIDGET_CB{
    auto   cell     = table.selected_cell();        // cell ที่ผู้ใช้กดเลือกเข้ามา
    if(cell.row == 0 ) return;

    const char* palette_color_txt =   table.cell((table_cell_dsc_t){cell.row, 3}).c_str();
    String palette_cmd;
    if( cell.col < 3 )      palette_cmd = StringX::printf("TFT_PALETTE_LIGHTEN(%s, %d)", palette_color_txt, 3-cell.col);
    else if( cell.col == 3) palette_cmd = StringX::printf("TFT_PALETTE(%s)", palette_color_txt );
    else if( cell.col <= 6) palette_cmd = StringX::printf("TFT_PALETTE_DARKEN(%s, %d)" , palette_color_txt, cell.col-3);
    Serial.println(palette_cmd);
    lb_palette_cmd = palette_cmd;
#ifdef _WIN32
    Windows_CopyTextToClipboard(palette_cmd.c_str());   // copy ไว้ที่ clipboard
#endif

    if(cell.col <3){
      int lighten_i = 3-cell.col;
      box_palette_selected.color(TFT_PALETTE_LIGHTEN(mycolor[cell.row-1].palette_color,lighten_i));
    }else
    if(cell.col==3){
      box_palette_selected.color(TFT_PALETTE(mycolor[cell.row-1].palette_color));
    }else{
      int darken_i = cell.col-3;
      box_palette_selected.color(TFT_PALETTE_DARKEN(mycolor[cell.row-1].palette_color,darken_i));
    }
  });

  table.align(ALIGN_TOP,0,2);
  box_palette_selected.size(30,10);
  box_palette_selected.color(TFT_BLACK);
  box_palette_selected.align(ALIGN_BOTTOM_LEFT,10,-5);
  lb_palette_cmd.align(ALIGN_BOTTOM_LEFT,50,0);
  lb_palette_cmd.font(prasanmit_18);

#ifdef _WIN32
  lb_mouse.show(false);
#endif

}

void loop(){
  BlynkGO.update();
}