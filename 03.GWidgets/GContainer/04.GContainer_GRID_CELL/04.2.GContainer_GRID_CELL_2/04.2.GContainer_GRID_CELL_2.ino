#include <BlynkGOv5.h>

GContainer cont (
  GRID_COL {70, GRID_FR(1), GRID_FR(2)},   // column 0: กว้าง 70px, colomn 1 : กว้าง 1 FR หน่วยจากระยะที่เหลือ, colomn 2 : กว้าง 2 FR หน่วยจากระยะที่เหลือ
  GRID_ROW {45, GRID_FR(1), 45}            // row    0: สูง 45px  , row    1 : สูง 1 FR หน่วยจากระยะที่เหลือ  , row   2 : สูง 45px
); 

  GRect rect[9];
    GLabel label[9];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.size(300,220);         // container นั้นๆ กำหนดขนาด แบบ fix คงที่ แล้ว ขนาด cell สามารถกำหนดขนาดด้วย GRID_FR(..) ได้ 
  for(int i=0; i<9; i++){
    int col = i%3, row = i/3;
    rect[i].parent(cont);
    rect[i].grid_cell(col,row);
    rect[i].border(2, TFT_SILVER);
    rect[i].radius(5);
    rect[i].font(prasanmit_20);
      label[i].parent(rect[i]);
      label[i] = StringX::printf("c%d, r%d", col, row); 
  }
}

void loop(){
  BlynkGO.update();
}
