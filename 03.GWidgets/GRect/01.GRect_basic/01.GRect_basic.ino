#include <BlynkGOv5.h>

GRect rect[9];

void setup(){
  Serial.begin(9600); Serial.println();

  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  GScreen.layout(LAYOUT_GRID,10,10,10,10,20);

  rect[0].size(80,30);
  rect[0].color(TFT_RED);

  rect[1].size(rect[0]);
  rect[1].radius(10);
  rect[1].color(TFT_PALETTE(TFT_PALETTE_BLUE));
  rect[1].shadow(30,5, TFT_BLUE);

  rect[2].size(rect[0]);
  rect[2].radius(10);
  rect[2].border(1, TFT_SILVER);
  rect[2].shadow(8,1, TFT_BLACK);
  rect[2].shadow_offset(5,5);

  rect[3].size(rect[0]);
  rect[3].color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_PURPLE,2));
  rect[3].border(5, TFT_PALETTE_DARKEN(TFT_PALETTE_PURPLE,1));
  rect[3].rounded(true);

  rect[4].size(rect[0]);
  rect[4].color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,2));
  rect[4].border(3, TFT_CYAN);
  rect[4].border_side( (border_side_t) ( BORDER_SIDE_BOTTOM | BORDER_SIDE_TOP ) );     // ตีเส้นขอบเฉพาะด้านล่าง กับ ด้านบน

  rect[5].size(rect[0]);
  rect[5].color(TFT_BLUE, TFT_RED);
  rect[5].grad_dir(GRAD_DIR_HOR);        // ไล่เฉดสีแนวนอน (ซ้าย->ขวา)
  
  rect[6].size(rect[0].width(), rect[0].height()*2);
  rect[6].color(TFT_BLUE, TFT_RED);

  rect[7].size(rect[6]);
  rect[7].color(TFT_BLUE, TFT_RED);
  rect[7].color_stop(100);

  rect[8].size(rect[6]);
  rect[8].color(TFT_BLUE, TFT_RED);
  rect[8].color_stop(100);
  rect[8].grad_stop(180);

  for(int i=0; i<9; i++){
    rect[i].onClicked(GWIDGET_CB{
      int ii = (GRect*)widget - &rect[0];
      if(rect[ii].ignore_layout()){
        rect[ii].ignore_layout(false);
        rect[ii].layer_id(ii);      // ย้ายมาไปเป็น layer ลูกลำดับที่ ii
        rect[ii].size(80,(ii <5)? 30 : 60);
      }else{
        rect[ii].ignore_layout(true);
        rect[ii].toForeground();    // ย้ายมาไว้บนสุด
        rect[ii].size(PCT(80),PCT(80));
      }
    });
  }
}

void loop(){
  BlynkGO.update();
}