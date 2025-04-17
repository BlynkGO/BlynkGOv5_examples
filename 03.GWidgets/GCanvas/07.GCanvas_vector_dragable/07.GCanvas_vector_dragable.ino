#include <BlynkGOv5.h>

#if defined(LCD_WIDTH) && defined (LCD_HEIGHT)
  #define CANVAS_WIDTH    LCD_WIDTH
  #define CANVAS_HEIGHT   LCD_HEIGHT
#else
  #define CANVAS_WIDTH    150
  #define CANVAS_HEIGHT   150
#endif

GCanvas canvas;

GCircle cir[5];

void canvas_draw_vector(){
  GVectorPath vpath;
  for(int i=0;i<5; i++){
    (i==0)? vpath.move_to(cir[0].center()) : 
            vpath.line_to(cir[i].center()) ;
  }
  vpath.close();

  layer_t layer;
  canvas.init_layer(layer);
    GVector vector(layer);
    vector.add_path(vpath, TFT_RED);
    vector.draw();
  canvas.finish_layer(layer);
  canvas.invalidate();
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  DRAW_BUF_DEFINE(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, COLOR_FORMAT_ARGB8888);
  canvas.draw_buf(draw_buf); 

  for(int i=0; i<5; i++){
    cir[i].radius(10);
    cir[i].color( random(0xFFFF));
    cir[i].center( random(CANVAS_WIDTH), random(CANVAS_HEIGHT));
    cir[i].drag_n_drop(true);
  }

  for(int i=0;i<5;i++){
    cir[i].onPressing(GWIDGET_CB{ canvas_draw_vector(); });
  }
  canvas_draw_vector();

}

void loop(){
  BlynkGO.update();
}
