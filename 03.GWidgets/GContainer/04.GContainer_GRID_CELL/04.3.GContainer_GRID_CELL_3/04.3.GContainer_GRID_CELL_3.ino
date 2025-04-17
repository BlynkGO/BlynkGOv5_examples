#include <BlynkGOv5.h>

// กำหนด GContainer เป็นแบบ GRID_CELL แบบ 3 column x 3 row
// โดยมีการกำหนด ความกว้าง column 70px และ ความสูง row 50px
GContainer cont(GRID_COL {70, 70, 70}, GRID_ROW {50, 50, 50}); 
  GRect rect[9];
    GLabel label[9];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.size(300,220);
  cont.grid_align(GRID_ALIGN_SPACE_BETWEEN, GRID_ALIGN_END);
  // cont.grid_align(GRID_ALIGN_SPACE_EVENLY, GRID_ALIGN_END);
  // cont.grid_align(GRID_ALIGN_SPACE_AROUND, GRID_ALIGN_END);
  // cont.grid_align(GRID_ALIGN_CENTER , GRID_ALIGN_CENTER);
  // cont.grid_align(GRID_ALIGN_CENTER , GRID_ALIGN_START);
  for(int row=0; row<3; row++){
    for(int col=0; col<3; col++){
      int i= row*3 + col;
      rect[i].parent(cont);
      rect[i].grid_cell(col,row);
      rect[i].border(2, TFT_SILVER);
      rect[i].radius(5);
        label[i].parent(rect[i]);
        label[i] = StringX::printf("c%d, r%d", col, row); 
    }
  }
}

void loop(){
  BlynkGO.update();
}

