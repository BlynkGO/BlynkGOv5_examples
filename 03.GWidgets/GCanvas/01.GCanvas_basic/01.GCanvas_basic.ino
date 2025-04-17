#include <BlynkGOv5.h>

#define CANVAS_WIDTH  200
#define CANVAS_HEIGHT  150

IMAGE_DECLARE(img_sun);

GCanvas canvas;
  color_t canvas_buffer[CANVAS_WIDTH*CANVAS_HEIGHT];

void setup() {
  Serial.begin(9600); 
  Serial.println();
  BlynkGO.begin();

  canvas.buffer(&canvas_buffer, CANVAS_WIDTH, CANVAS_HEIGHT);
  canvas.fill_bg(TFT_COLOR_HSV(0,0,95));
  
  GLayer layer;
  canvas.init_layer(layer);
  
  draw_rect_dsc_t rect_dsc;
  draw_rect_dsc_init(&rect_dsc);

    rect_dsc.radius = 10;
    rect_dsc.bg_opa = 255;
    rect_dsc.bg_grad.dir = GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].color = TFT_COLOR(TFT_RED);
    rect_dsc.bg_grad.stops[0].opa = OPA_100;
    rect_dsc.bg_grad.stops[1].color = TFT_COLOR(TFT_BLUE);
    rect_dsc.bg_grad.stops[1].opa = OPA_50;
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = OPA_90;
    rect_dsc.border_color = TFT_COLOR(TFT_WHITE);
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_offset_x = 5;
    rect_dsc.shadow_offset_y = 5;

    layer.drawRect(rect_dsc, 70, 50, ALIGN_TOP_LEFT, 20, 30);

    layer.drawLabel("Some text on text canvas", prasanmit_20, TFT_ORANGE, ALIGN_BOTTOM);

    layer.drawImage(img_sun, 90, ALIGN_LEFT);
    
  canvas.finish_layer(layer);

}


void loop() {
  BlynkGO.update();
}


void loop() {
  BlynkGO.update();
}
