/* ตัวอย่าง จัดเรียง ปุ่มกด 6 ปุ่ม อัตโนมัติ แถวละ 2 ปุ่ม x 3 แถว  
   ด้วย GContainer แบบ GRID_CELL */

#include <BlynkGOv5.h>

// กำหนด GContainer เป็นแบบ GRID_CELL แบบ 2 column x 3 row
// โดยมีการกำหนด ความกว้าง column 150px และ ความสูง row 50px
GContainer cont_btn( GRID_COL {120, 120}, GRID_ROW {50,50,50} );
  GButton btn[6];
    String btn_str[6] = { 
      "ปุ่ม 1", "ปุ่ม 2",
      "ปุ่ม 3", "ปุ่ม 4",
      "ปุ่ม 5", "ปุ่ม 6",
    };
  
void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  cont_btn.size(320,240);
  cont_btn.padding(0);                                                    /* "ช่องไฟ" ของวิตเจ็ต ไม่ต้องมี */
  cont_btn.grid_align(GRID_ALIGN_SPACE_EVENLY, GRID_ALIGN_SPACE_EVENLY);  /* "ช่องว่าง" ระหว่างกริด และ "ช่องว่าง" ระหว่างกริดกับช่องไฟรอบนอก เฉลี่ยเท่าๆกัน */
  for(int i=0; i< 6; i++) {
    int col = i%2;  int row = i/2;
    btn[i].parent(cont_btn);
    btn[i].grid_cell(col, row, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);   /* ปุ่มวางไว้ที่ช่อง กริด (col, row) โดยขยายขนาด ปุ่ม เต็มช่องกริดทั้งด้านกว้างและด้านสูง */
    btn[i] = btn_str[i];
    btn[i].onClicked(GWIDGET_CB{
      int ii = (GButton*) widget - &btn[0];
      Serial.printf("[GButtun] %d : clicked\n", ii+1);
    });
  }
}

void loop() {
  BlynkGO.update();
}

