#include <BlynkGOv5.h>

#ifndef TFT_WIDTH
#define TFT_WIDTH   800
#endif 

#ifndef TFT_HEIGHT
#define TFT_HEIGHT  480
#endif

#if TFT_WIDTH == 800 && TFT_HEIGHT == 480
/* GRID_CELL แบบ 4 x 3
 *   โดย คอลัมภ์กว้าง  150px, GRID_FR(1), 150px
 *       แถวสูง      60px, GRID_FR(1), 60px
 */
GContainer dashboard_frame(
  GRID_COL {150, GRID_FR(1), 150},
  GRID_ROW {60, GRID_FR(1), 60},
  OPA_0
);
#elif TFT_WIDTH == 320 && TFT_HEIGHT == 480
/* GRID_CELL แบบ 2 x 4
 *   โดย คอลัมภ์กว้าง  150px, GRID_FR(1), 150px
 *       แถวสูง      60px, GRID_FR(1), 60px
 */
GContainer dashboard_frame(
  GRID_COL {GRID_FR(1), GRID_FR(1)},
  GRID_ROW {50, GRID_FR(1), GRID_FR(1), 50},
  OPA_0
);
#endif


GRect rect_header(dashboard_frame);
GRect rect_main(dashboard_frame);
GRect rect_left(dashboard_frame);
GRect rect_right(dashboard_frame);
GRect rect_footer(dashboard_frame);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin(); BlynkGO.fillScreen(TFT_BLACK);

  dashboard_frame.size(GScreen); 
  dashboard_frame.padding(0);

#if TFT_WIDTH == 800 && TFT_HEIGHT == 480
  rect_header.grid_cell(0, 0);
  rect_header.grid_cell_span(3, 1);
  rect_header.color(TFT_COLOR_HEX(0xf97173));

  rect_left.grid_cell(0, 1);
  rect_left.color(TFT_COLOR_HEX(0xf5d460));

  rect_main.grid_cell(1, 1);

  rect_right.grid_cell(2, 1);
  rect_right.color(TFT_COLOR_HEX(0xc6ed78));

  rect_footer.grid_cell(0, 2);
  rect_footer.grid_cell_span(3, 1);
  rect_footer.color(TFT_COLOR_HEX(0x71c1f2));

#elif TFT_WIDTH == 320 && TFT_HEIGHT == 480
  rect_header.grid_cell(0, 0);
  rect_header.grid_cell_span(2, 1);
  rect_header.color(TFT_COLOR_HEX(0xf97173));

  rect_main.grid_cell(0, 1);
  rect_main.grid_cell_span(2, 1);

  rect_left.grid_cell(0, 2);
  rect_left.color(TFT_COLOR_HEX(0xf5d460));

  rect_right.grid_cell(1, 2);
  rect_right.color(TFT_COLOR_HEX(0xc6ed78));

  rect_footer.grid_cell(0, 3);
  rect_footer.grid_cell_span(2, 1);
  rect_footer.color(TFT_COLOR_HEX(0x71c1f2));
#endif

}

void loop()
{
  BlynkGO.update();
}

