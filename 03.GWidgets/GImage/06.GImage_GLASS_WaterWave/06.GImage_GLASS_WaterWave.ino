#include <BlynkGOv5.h>

IMAGE_DECLARE(img_wave);

GRect  glass;
  GRect water(glass);
    GRect indicator(water);
      GImage wave(img_wave,indicator);
      GRect  water_part(indicator);
  
GAnimation anim_wave;

GSlider slider;
GLabel  lb_level;

void setup() {
  BlynkGO.begin();
  
  BlynkGO.fillScreen(TFT_COLOR_HEX(0x0A1931)); 

  glass.size(SIZE_CONTENT, 300);
  glass.padding(5);
  glass.bg_opa(0);
  glass.border(3, TFT_WHITE);
  glass.radius(10);
    water.size(SIZE_CONTENT,glass.content_height());
    water.bg_opa(0);
    water.radius(7);
    water.clip_corner(true);
      indicator.size(SIZE_CONTENT, PCT(100));
      indicator.GRID_CELL( 
        GRID_COL{GRID_CONTENT}, 
        GRID_ROW{GRID_CONTENT, GRID_FR(1)}
      );
      indicator.bg_opa(0);
      indicator.padding(0);

        wave.grid_cell(0,0);
        wave.recolor(TFT_CYAN);
        wave.image_align(IMAGE_ALIGN_TILE); // ให้วาดต่อเนื่องต่อๆ กันเป็น tile
        anim_wave.init([](GWIDGET, int32_t v){ wave.offset_x(v); },0, wave.width(), 2000, true); anim_wave.start();
      
        water_part.grid_cell(0,1);
        water_part.color(TFT_CYAN);

  slider.range(0, 100);
  slider.align(ALIGN_BOTTOM, 0, -20);
  slider.onValueChanged(GWIDGET_CB{
    int level = slider.toInt();
    int translate_y = glass.content_height() - map(level, 0, 100, 0, glass.content_height());
    indicator.translate_y(translate_y);
    lb_level = level;
  });

  lb_level.align(glass, ALIGN_RIGHT, 50);
  lb_level.font(BebasNeueBold_num_45, TFT_WHITE);

  slider = 50;
  slider.event_send(EVENT_VALUE_CHANGED);
}

void loop(){
  BlynkGO.update();
}

