#include <BlynkGOv5.h>

/* GRID_CELL แบบ 3 x 3 
 *   โดย คอลัมภ์กว้าง 70, 100, 70
 *       แถวสูง 50,30,50
 */

GContainer frame(
  GRID_COL {70, 100, 70}, 
  GRID_ROW {50, 30, 50}
);
  GButton btn[9];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  frame.size(300, 220);

  // frame.grid_cell(
  //   GRID_COL {70, 100, 70}, 
  //   GRID_ROW {50, 30, 50}
  // );
  // frame.grid_align(GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

  for(int i=0; i<9; i++){
    int col = i%3, row = i/3;
    btn[i].parent(frame);
    btn[i].grid_cell(col,row, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);      // btn[i] วางที่พิกัดใน grid_cell ที่ col, row และ ให้ขยายเต็มช่องกริด
    btn[i] = StringX::printf("c%d, r%d", col, row); 
    btn[i].font(prasanmit_20);
    btn[i].onClicked(GWIDGET_CB{
      int ii = (GButton*) widget - &btn[0];
      int col = ii%3; int row = ii/3;
      Serial.printf("[BTN] c%d, r%d\n", col, row);
    });
  }
}

void loop(){
  BlynkGO.update();
}

