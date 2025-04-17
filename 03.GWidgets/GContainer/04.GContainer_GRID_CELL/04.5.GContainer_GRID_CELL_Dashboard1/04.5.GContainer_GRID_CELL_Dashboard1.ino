
#include <BlynkGOv5.h>

/* GRID_CELL แบบ 5 x 3 
 *   โดย คอลัมภ์กว้าง  100, 100, 100, 100, 100
 *       แถวสูง      100, 100, 100
 */
GContainer frame(
  GRID_COL {100, 100, 100, 100, 100}, 
  GRID_ROW {100, 100, 100},
  OPA_0
);

GRect rect[6];
  GLabel label[6];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  for(int i=0; i< 6; i++){
    rect[i].parent(frame);
    rect[i].radius(10);

    label[i].parent(rect[i]);
    label[i] = String("id : ") + String(i);
  }

  rect[0].grid_cell(0, 0);          // วางที่พิกัด 0,0
  rect[0].grid_cell_span(2 , 2);    // ขยาย กว้างไป 2 ช่อง สูง 2 ช่อง

  rect[1].grid_cell(2, 0);          // วางที่พิกัด 2,0
  rect[1].grid_cell_span(3 , 1);    // ขยาย กว้างไป 3 ช่อง สูง 1 ช่อง

  rect[2].grid_cell(0, 2);          // วางที่พิกัด 0,2
  rect[2].grid_cell_span(2 , 1);    // ขยาย กว้างไป 2 ช่อง สูง 1 ช่อง

  rect[3].grid_cell(2, 1);          // วางที่พิกัด 2,1
  rect[3].grid_cell_span(2 , 2);    // ขยาย กว้างไป 2 ช่อง สูง 2 ช่อง

  rect[4].grid_cell(4, 1);          // วางที่พิกัด 4,1

  rect[5].grid_cell(4, 2);          // วางที่พิกัด 4,2
}

void loop()
{
  BlynkGO.update();
}
