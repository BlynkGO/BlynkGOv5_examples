#include <BlynkGOv5.h>

IMAGE_DECLARE(img_humid);
IMAGE_DECLARE(img_temp);

GTable table;
GButton btn;
GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // table.size(320,180);
  table.height(300);
  table.cells(22,5);                            /* ให้ตารางมี 22 แถว 5 คอลัมภ์ */
  table.align(ALIGN_TOP,0,10);
  
  btn = "Clear Table";
  btn.height(40);
  btn.align(ALIGN_BOTTOM,0,-15);
  btn.onClicked(GWIDGET_CB{
    table.rows(3);                              /* วิธีเคลียร์ตาราง โดยการกำหนดให้เหลือ 3 แถว */
  });

  label.align(table, ALIGN_BOTTOM_LEFT,0,20);

  /**************************************************/
  /* ปรับเปลี่ยน ความกว้างของ column หรือ ความสูงของ row */
  /**************************************************/
  table.column_width(200);                      /* กำหนดความกว้าง column 200 px แบบทั้งตาราง */
  table.row_height(30);                         /* กำหนดความสูง row 30 px แบบทั้งตาราง */

  table.column(0).width(60);                    /* เปลี่ยนความกว้าง เฉพาะ column 0 */
  table.column(1).width(100);                   /* เปลี่ยนความกว้าง เฉพาะ column 1 */

  table.row(0).height(80);                      /* เปลี่ยนความสูง เฉพาะ row 0 */
  table.row(1).height(0);                       /* ยกเลิกความสูง เฉพาะ row 1 ที่เคยกำหนดเอง ใช้คำนวณความสูงอัตโนมัติ */

  /********************************************************/
  /* ปรับเปลี่ยน design ตาราง, column, row, cell ได้ตามต้องการ */
  /********************************************************/
  
  table.font(prasanmit_20);

  table.column(0).align(TEXT_ALIGN_CENTER); table.column(0).color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_GRAY,2));
  table.column(0).clickable(false);
  table.column(1).align(TEXT_ALIGN_CENTER);
  table.column(2).align(TEXT_ALIGN_RIGHT); 
  table.column(2).color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BLUE,2));
  table.column(3).align(TEXT_ALIGN_RIGHT);  
  table.column(3).color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,2));

  table.row(0).align(TEXT_ALIGN_CENTER);
  table.row(0).font(prasanmit_25);
  table.row(0).clickable(true);
  table.row(1).font(prasanmit_25, TFT_WHITE);
  table.row(1).color(TFT_PALETTE_DARKEN(TFT_PALETTE_DEEP_PURPLE,2));
  table.row(1).align(TEXT_ALIGN_CENTER);   
  table.row(1).clickable(false);
  
  table.cell(0,2).font(prasanmit_40);
  table.cell(0,2).align(TEXT_ALIGN_CENTER);
  table.cell(0,2).text_offset(0,-10);  
  table.cell(0,2).color(TFT_PALETTE_DARKEN(TFT_PALETTE_CYAN,2));

  /***********************/
  /* กำหนดค่าให้ cell ต่างๆ */
  /***********************/
  table.cell(0,0) = "BlynkGO";
  table.merge_cell_right(0, 0, true);           /* ผสานรวม cell (0,0) กับ cell ด้านขวาเข้าด้วยกัน */ 

  table.cell(0,2) = "DHT";  
  table.merge_cell_right(0, 2, true);           /* ผสานรวม cell (0,2) กับ cell ด้านขวาเข้าด้วยกัน */ 
  table.merge_cell_right(0, 3, true);           /* ผสานรวม cell (0,3) กับ cell ด้านขวาเข้าด้วยกัน */ 

  table.cell(1,0) = "เวลา";
  table.merge_cell_right(1, 0, true);           /* ผสานรวม cell (1,0) กับ cell ด้านขวาเข้าด้วยกัน */ 

  table.cell(1,2).icon_src(img_temp);           /* เพิ่ม icon หน้าข้อความ ของ cell(1,2) */
  table.cell(1,2) = "อุณหภูมิ";

  table.cell(1,3).icon_src(img_humid);
  table.cell(1,3) = "ความชื้น";

  table.cell(1,4) = "หมายเหตุ";

  for(int i=0 ; i<20; i++){
    int r = 2+i;

    table.cell(r,0) = i+1;

    String clock =  StringX::printf("%02d:%02d", 15, i);   
    table.cell(r,1) = clock;

    float temp  = random(2000, 5000)/100.0;     /* สุ่มตัวเลข 20.00-50.00  ของจริงให้อ่านจาก sensor */
    float humid = random(6000, 9000)/100.0;     /* สุ่มตัวเลข 60.00-90.00  ของจริงให้อ่านจาก sensor */

    table.cell(r,2) = StringX::printf("%.2f", temp);
    table.cell(r,3) = StringX::printf("%.2f", humid);
  }

  /****************************************** */
  /* เมื่อ user มีการกดเลือกมาที่ cell ใดๆ ของ ตาราง */
  /****************************************** */
  table.onCellClicked(GWIDGET_CB{
    auto   cell     = table.selected_cell();      /* cell ที่ผู้ใช้กดเลือกเข้ามา */
    String cell_str = table.cell(cell).toString();/* ข้อความของ cell */
    Serial.printf("[cell %d,%d] %s\n", cell.row, cell.col, cell_str.c_str());
    label = cell_str;
  });
}

void loop(){
  BlynkGO.update();
}

