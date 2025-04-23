#include <BlynkGOv5.h>

FONT_DECLARE(BebasNeueBold_num_45);

GButton btn;
  GLabel lb_extra(btn);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  btn = "อุณหภูมิ";
  btn.font(prasanmit_20);
  btn.GRID_CELL( GRID_COL { 100 }, GRID_ROW { GRID_CONTENT, GRID_CONTENT });
    lb_extra = String(29.1, 1);
    lb_extra.font(BebasNeueBold_num_45);
    lb_extra.grid_cell(0,1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER,0,-5);
}

void loop()
{
  BlynkGO.update();
}
