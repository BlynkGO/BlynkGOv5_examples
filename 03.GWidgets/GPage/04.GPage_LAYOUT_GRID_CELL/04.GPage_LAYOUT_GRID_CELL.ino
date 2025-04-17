#include <BlynkGOv5.h>

GPage page (
  GRID_CELL_COL {70, GRID_FR(1), GRID_FR(2)},               // col 3 คอลัมน์ โดย column 0: กว้าง 70px, colomn 1 : กว้าง 1 หน่วยจากระยะที่เหลือ, colomn 2 : กว้าง 2 หน่วยจากระยะที่เหลือ  
  GRID_CELL_ROW {60, 60, 60, 60, 60, 60, 60, 60, 60, 60}    // row 10 แถว   โดย row มีความ สูง 60px
); 

  GRect rect[30];
    GLabel label[30];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  page.size(300,220);
  for(int i=0; i<30; i++){
    int col = i%3, row = i/3;   // 3 คือจำนวน column ในแต่ละแถว
    rect[i].parent(page);
    rect[i].grid_cell(col,row);
    rect[i].border(2, TFT_SILVER);
    rect[i].radius(5);
      label[i].parent(rect[i]);
      label[i] = StringX::printf("c%d, r%d", col, row); 
  }
}

void loop(){
  BlynkGO.update();
}
