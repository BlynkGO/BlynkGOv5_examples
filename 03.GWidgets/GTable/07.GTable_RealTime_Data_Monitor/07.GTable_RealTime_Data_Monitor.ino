#include <BlynkGOv5.h>

#define MAX_TABLE_ROW     (20+1)

#define ROW_HEIGHT        20
#define ROW0_HEIGHT       40

#define TABLE_HEIGHT      (ROW0_HEIGHT + (MAX_TABLE_ROW-1)*ROW_HEIGHT)

GTable table;

uint32_t id = 0;
int8_t cur_row_id = 0;
int8_t prev_row_id = MAX_TABLE_ROW-1;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  table.height(TABLE_HEIGHT);
  table.scrollable(true);
  table.bg_opa(0);
  table.font(prasanmit_18);
  table.text_align(TEXT_ALIGN_CENTER, GPART_ITEMS);
  table.padding(2, GPART_ITEMS);

  table.column(0).width(50);                    /* เปลี่ยนขนาดความกว้าง เฉพาะ column 0 */
  table.column(1).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 1 */
  table.column(2).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 2 */
  table.column(3).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 3 */
  table.column(4).width(180);                   /* เปลี่ยนขนาดความกว้าง เฉพาะ column 4 */
  table.row(0).font(prasanmit_25);
  table.row(0).height(ROW0_HEIGHT);
  table.row(0).color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_PURPLE,2));

  /***************************************/
  /* กำหนดค่าให้ cell ต่างๆ (r แถว, c หลัก) */
  /***************************************/
  table.cell(0,0) = "#";  table.cell(0,1) = "วดป";   table.cell(0,2) = "เวลา"; table.cell(0,3) = "อุณหภูมิ"; table.cell(0,4) = "ความชื้น";

  static SoftTimer timer;
  timer.setInterval(200,[](){
    
    float temp  = random(2000,4500)/100.0;      // ของจริงให้อ่านค่าจาก sensor
    float humid = random(6000,9500)/100.0;      // ของจริงให้อ่านค่าจาก sensor

    //------------------------------------------------------
    cur_row_id = (cur_row_id+1) % MAX_TABLE_ROW;
    if(cur_row_id == 0) cur_row_id = 1;
    id = (id+1)%1000; if(id == 0) id = 1000;

    table.cell(cur_row_id, 0) = String(id);
    table.cell(cur_row_id, 1) = StringX::printf("%02d/%02d/%02d", day(),month(),year());
    table.cell(cur_row_id, 2) = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
    table.cell(cur_row_id, 3) = StringX::printf("%.2f", temp);
    table.cell(cur_row_id, 4) = StringX::printf("%.2f", humid);

    table.row_height(ROW_HEIGHT);               // กำหนดทุก row สูง ROW_HEIGHT
    table.row(0).height(ROW0_HEIGHT);           // กำหนดเฉพาะ row 0 สูง ROW0_HEIGHT
    table.row(prev_row_id).color(TFT_WHITE);
    table.row(cur_row_id).color(TFT_CYAN);
    prev_row_id = cur_row_id;
    //------------------------------------------------------
  });
}

void loop() {
  BlynkGO.update();
}
