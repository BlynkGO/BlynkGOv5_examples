#include <BlynkGOv5.h>

IMAGE_DECLARE(img_apple);
IMAGE_DECLARE(img_banana);
IMAGE_DECLARE(img_coconut);
IMAGE_DECLARE(img_grape);
IMAGE_DECLARE(img_lemon);
IMAGE_DECLARE(img_mango);
IMAGE_DECLARE(img_water_melon);

GTable table;
  const img_t* img_fruit[]  = { NULL, &img_apple, &img_banana, &img_lemon, &img_grape, &img_water_melon, &img_mango, &img_coconut};

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  table.height(220);

  /**************************************************/
  /* ปรับเปลี่ยน ความกว้างของ column หรือ ความสูงของ row */
  /**************************************************/
  table.column(0).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 0 */

  /***********************/
  /* กำหนดค่าให้ cell ต่างๆ */
  /***********************/
  table.cell(0,0) = "ชื่อ";                       table.cell(0,1) = "ราคา";
  table.cell(1,0) = "แอปเปิ้ล";                   table.cell(1,1) = 15;
  table.cell(2,0) = "กล้วย";                     table.cell(2,1) = FLOAT(16.1, 1);     /* ใส่ค่าให้ cell ด้วย เลขแบบทศนิยม 1 ตำแหน่ง*/
  table.cell(3,0) = "มะนาว";                    table.cell(3,1) = FLOAT(17.25, 2);    /* ใส่ค่าให้ cell ด้วย เลขแบบทศนิยม 2 ตำแหน่ง*/
  table.cell(4,0) = "องุ่น";                      table.cell(4,1) = 270;
  table.cell(5,0) = "แตงโม";                    table.cell(5,1) = 90;
  table.cell(6,0) = "มะม่วง";                    table.cell(6,1) = 60;
  table.cell(7,0) = "มะพร้าว";                   table.cell(7,1) = 58;

  /**************************************/
  /* ปรับเปลี่ยน design ตาราง ได้ตามต้องการ  */
  /**************************************/
  table.row(0).font_color(TFT_CYAN);
  table.row(0).color(TFT_VIOLET);
  table.row(0).clickable(false);
  table.row(0).align(TEXT_ALIGN_CENTER, 0, 0);
  table.column(1).align(TEXT_ALIGN_RIGHT);

  for(int r=1; r <= 7 ; r++){
    table.cell(r, 0).icon_src(img_fruit[r]);        /* กำหนดเพิ่ม รูป icon ให้ cell ที่ระบุ หน้าข้อความของ cell นั้น */
  }

  /**************************************/
  /* hook ขณะมีการ draw กราฟิกระดับล่าง     */
  /**************************************/
  table.hookDrawTask(true);
  table.onDrawTask(GWIDGET_CB{
    if(DRAW_PART == DRAW_PART_CELL) {               /* ขณะ กราฟิกระดับล่าง กำลัง draw ในส่วนของ CELL ของ GTable */
      uint32_t r = table.draw_row();                /* cell ที่กำลังถูก draw ระดับล่าง เป็น cell แถวไหน */
      uint32_t c = table.draw_col();                /* cell ที่กำลังถูก draw ระดับล่าง เป็น cell คอลัมภ์ไหน */
      if( DRAW_TYPE == DRAW_TYPE_LABEL ){
        if( r > 0 && c == 1) {
          auto label_dsc =  table.draw_label_dsc();

          float price = table.cell(r,c).toFloat();                  /* ดึงค่า float ออกจาก cell ที่ต้องการ */

          static String price_str;
          price_str = String("฿ ") + String(price, 2);              /* เพิ่มเครื่องหมาย ฿ และจัดแสดงแบบ 2 ตำแหน่ง */

          label_dsc->text = price_str.c_str();
          if(price >= 60 ) label_dsc->color = TFT_COLOR(TFT_RED);   /* เปลี่ยนสี เมื่อราคา มากกว่า 60 ขึ้นไป */
        }
      }else
      if( DRAW_TYPE == DRAW_TYPE_FILL ){
        auto fill_dsc = table.draw_fill_dsc();
        if( r > 0 && r%2 == 0) {                                    /* หากเป็น cell ใน แถวเลขคู่ โดย ไม่รวมแถวแรก */
          fill_dsc->color  = TFT_COLOR( TFT_COLOR_HEX(0xf0f0f0));   /* เปลียนสี พื้นหลัง cell */
        }
      }
    }
  });

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
