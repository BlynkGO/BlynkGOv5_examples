#include <BlynkGOv5.h>

#if defined(LCD_WIDTH) && defined (LCD_HEIGHT)
  #define CANVAS_WIDTH    LCD_WIDTH
  #define CANVAS_HEIGHT   LCD_HEIGHT
#else
  #define CANVAS_WIDTH    150
  #define CANVAS_HEIGHT   150
#endif

GCanvas canvas;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  DRAW_BUF_DEFINE(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, COLOR_FORMAT_ARGB8888);
  canvas.draw_buf(draw_buf); 

  GVectorPath vpath;
  Point pts[] = {{10, 10}, {130, 130}, {10, 130}};
  vpath.move_to(pts[0]);
  vpath.line_to(pts[1]);
  vpath.line_to(pts[2]);
  vpath.close();

  layer_t layer;
  canvas.init_layer(layer);
    GVector vector(layer);
    vector.add_path(vpath, TFT_RED);
    vector.draw();
  canvas.finish_layer(layer);
}

void loop(){
  BlynkGO.update();
}
