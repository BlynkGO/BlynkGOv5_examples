#include <BlynkGOv5.h>

GTable table;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  table.height(220);

  /**************************************************/
  /* ปรับเปลี่ยน ความกว้างของ column หรือ ความสูงของ row */
  /**************************************************/
  table.column(0).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 0 */
  table.row(4).height(table.row(1).height());   /* เปลี่ยนขนาดความสูง   เฉพาะ row 4 ให้เท่ากับความสูงของ row 1 */

  /********************************************************/
  /* ปรับเปลี่ยน design ตาราง, column, row, cell ได้ตามต้องการ */
  /********************************************************/
  table.row(0).font(prasanmit_40, TFT_CYAN);
  table.row(0).color(TFT_VIOLET);
  table.row(0).align(TEXT_ALIGN_CENTER,0,-10);
  table.column(1).font(prasanmit_30, TFT_RED);  
  table.column(1).align(TEXT_ALIGN_RIGHT);

  // table.font(prasanmit_20, TFT_BLACK);           /* กำหนด font แบบทั้งตาราง */
  // table.color(TFT_YELLOW);                       /* กำหนด สี ทั้งตาราง */
  // table.pad_left(5, GPART_CELL);                 /* กำหนดช่องไฟเฉพาะด้านซ้าย ของ cell */
  // table.padding(5, 5, 10, 10, 10, GPART_CELL);   /* กำหนดช่องไฟ บน ล่าง ซ้าย ขวา ระหว่าง ของ cell */

  table.cell(2,0).color(TFT_ORANGE);                /* ให้อยู่ตรงกลางของ cell เฉพาะ cell ที่ระบุ */
  // table.cell(3,1).align(TEXT_ALIGN_CENTER);      /* ให้อยู่ตรงกลางของ cell เฉพาะ cell ที่ระบุ */
  // table.cell(4,0).font(prasanmit_20, TFT_CYAN);  /* กำหนด font          เฉพาะ cell ที่ระบุ */
  // table.cell(4,1).font_color(TFT_RED);           /* กำหนด สีข้อความ       เฉพาะ cell ที่ระบุ */
  // table.cell(4,0).color(TFT_ORANGE);             /* เปลี่ยนสีพื้น            เฉพาะ cell ที่ระบุ */
  // table.cell(4,0).text_offset(0, 10);            /* ปรับตำแหน่ง text ให้ลงมา 10px เฉพาะ cell ที่ระบุ */

  /***********************/
  /* กำหนดค่าให้ cell ต่างๆ */
  /***********************/
  table.cell(0,0) = "ชื่อ";                       table.cell(0,1) = "ราคา";
  table.cell(1,0) = "แอปเปิ้ล";                   table.cell(1,1) = 15;
  table.cell(2,0) = "กล้วย";                     table.cell(2,1) = FLOAT(16.1, 1);     /* ใส่ค่าให้ cell ด้วย เลขแบบทศนิยม 1 ตำแหน่ง*/
  table.cell(3,0) = "มะนาว";                    table.cell(3,1) = FLOAT(17.25, 2);    /* ใส่ค่าให้ cell ด้วย เลขแบบทศนิยม 2 ตำแหน่ง*/
  table.cell(4,0) = "มะม่วงไม่รู้หาว\nมะนาวไม่รู้โห่";   table.cell(4,1) = 270;
  table.cell(5,0) = "แตงโม";                    table.cell(5,1) = 90;
  table.cell(6,0) = "มะม่วง";                    table.cell(6,1) = 60;
  table.cell(7,0) = "มะพร้าว";                   table.cell(7,1) = 58;

  /******************************************** */
  /* เมื่อ user มีการกดgเลือกมาที่ cell ใดๆ ของ ตาราง */
  /******************************************** */
  table.onCellClicked(GWIDGET_CB{
    auto   cell     = table.selected_cell();        // cell ที่ผู้ใช้กดเลือกเข้ามา
    String cell_str = table.cell(cell).toString();  // ข้อความของ cell
    Serial.printf("[cell %d,%d] %s\n", cell.row, cell.col, cell_str.c_str());
  });

}

void loop(){
  BlynkGO.update();
}

