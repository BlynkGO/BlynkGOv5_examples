#include <BlynkGOv5.h>

IMAGE_DECLARE(img_wave);
IMAGE_DECLARE(img_wave_bg);

#define MY_BLUE  TFT_COLOR_HEX(0x4477ff)

GRect frame_circle;
  GRect water(frame_circle);
    GRect indicator(water);
      GImage wave_bg(img_wave_bg, indicator);
      GImage wave(img_wave,indicator);
      GRect  water_part(indicator);
  
GAnimation anim_wave_bg, anim_wave;


void setup() {
  BlynkGO.begin();
  
  BlynkGO.fillScreen(TFT_WHITE); 

  frame_circle.size(300, 300);
  frame_circle.padding(10);
  frame_circle.bg_opa(0);
  frame_circle.border(10, MY_BLUE);
  frame_circle.radius(RADIUS_CIRCLE);
    water.size(frame_circle.content_width(),frame_circle.content_height());
    water.bg_opa(0);
    water.radius(RADIUS_CIRCLE);
    water.clip_corner(true);
      indicator.size(PCT(100), PCT(55));
      indicator.GRID_CELL( 
        GRID_COL{GRID_FR(1)}, 
        GRID_ROW{GRID_CONTENT, GRID_FR(1)}
      );
      indicator.align(ALIGN_BOTTOM);
      indicator.bg_opa(0);
      indicator.padding(0);

        wave_bg.grid_cell(0,0, GRID_ALIGN_START, GRID_ALIGN_END);
        wave_bg.recolor(TFT_COLOR_HEX(0x9cb6f6));
        wave_bg.image_align(IMAGE_ALIGN_TILE); // ให้วาดต่อเนื่องต่อๆ กันเป็น tile
        anim_wave_bg.init([](GWIDGET, int32_t v){ wave_bg.offset_x(v); },0, -img_wave_bg.header.w, 10000, true); anim_wave_bg.start();

        wave.grid_cell(0,0, GRID_ALIGN_START, GRID_ALIGN_END);
        wave.width(PCT(100));
        wave.recolor(MY_BLUE);
        wave.image_align(IMAGE_ALIGN_TILE); // ให้วาดต่อเนื่องต่อๆ กันเป็น tile
        anim_wave.init([](GWIDGET, int32_t v){ wave.offset_x(v); },0, -img_wave.header.w, 2000, true); anim_wave.start();
      
        water_part.grid_cell(0,1);
        water_part.color(MY_BLUE);

}

void loop(){
  BlynkGO.update();
}

