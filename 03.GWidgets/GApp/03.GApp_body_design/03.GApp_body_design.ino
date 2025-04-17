#include <BlynkGOv5.h>

GApp app;
  GContainer body(app);
    GRect rect_main(body);
    GRect rect_left(body);
    GRect rect_right(body);
    GRect rect_footer(body);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  app.size(GScreen);
  body = app.body();
  body.GRID_CELL(    // ในส่วน body ของ app กำหนด layout ให้เป็นแบบ GRID_CELL   แบบ 3 x 2
        GRID_COL {GRID_FR(1), GRID_FR(3), GRID_FR(1)},   // ความกว้างของ column ของ GRID_CELL
        GRID_ROW {GRID_FR(1), 40 }                       // ความสูง ของ row ของ GRID_CELL
      );

  rect_left.grid_cell(0, 0);
  rect_left.grid_cell_align(GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  rect_left.color(TFT_COLOR_HEX(0xf5d460));

  rect_main.grid_cell(1, 0);
  rect_main.grid_cell_align(GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

  rect_right.grid_cell(2, 0);
  rect_right.grid_cell_align(GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  rect_right.color(TFT_COLOR_HEX(0xc6ed78));

  rect_footer.grid_cell(0, 1);
  rect_footer.grid_cell_span(3, 1);
  rect_footer.grid_cell_align(GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  rect_footer.color(TFT_COLOR_HEX(0x71c1f2));

}

void loop()
{
  BlynkGO.update();
}

