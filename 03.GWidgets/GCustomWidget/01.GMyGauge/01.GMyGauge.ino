/*
 * ตัวอย่างสร้าง class วิตเจ็ต แตกอนุพันธุ์ จาก GRect และมีการ drawMain เพิ่ม layer ใหม่อีกอีกชั้น 
 *  
 */

#include <BlynkGOv5.h>

class GMyGauge : public GRect
{
  public:
    GMyGauge(GWidget &parent = GScreen) : GRect(parent) {}

    float _val=0.0f, _val_min=0.0f, _val_max =100.0f;
    uint8_t _arc_max_num = 120;
    double  _angle_offset = 150;
    double  _angle_total = 240;
    double  _angle_unit = _angle_total/_arc_max_num;
    int32_t _thick = 20;
    color_t _color[3] = { TFT_BLUE, TFT_RED, TFT_COLOR_HEX(0x1e1e1e) };
    uint8_t _decimal = 2;

    void create(){
      if(this->isCreated()) return;
      GRect::create();
      GRect::size(200,200);
      GRect::bg_opa(0);
      GRect::font(prasanmit_40, TFT_WHITE);
      this->onDrawMain(GWIDGET_CB{
        GMyGauge* _p_mygauge = (GMyGauge*) widget;

        /*Calculate the main area*/
        int32_t bg_left   = _p_mygauge->pad_left();
        int32_t bg_right  = _p_mygauge->pad_right();
        int32_t bg_top    = _p_mygauge->pad_top();
        int32_t bg_bottom = _p_mygauge->pad_bottom();

        auto bg_area      = _p_mygauge->coords();
        /*************************/
        /* สร้าง layer ใหม่  */
        area_t gauge_area;
        area_copy(&gauge_area, bg_area);
        gauge_area.x1 += bg_left;
        gauge_area.x2 -= bg_right;
        gauge_area.y1 += bg_top;
        gauge_area.y2 -= bg_bottom;
        layer_t * layer_gauge = draw_layer_create(DRAW_LAYER, (color_format_t) COLOR_FORMAT_ARGB8888, &gauge_area);
        /*************************/

        draw_arc_dsc_t arc_dsc;
        draw_arc_dsc_init(&arc_dsc);

        point_t center;
        auto _r =  min (  _p_mygauge->raw_width()  - bg_left - bg_right,
                          _p_mygauge->raw_height() - bg_top  - bg_bottom) / 2;

        center.x = bg_area->x1 + _p_mygauge->raw_width()/2;
        center.y = bg_area->y1 + _r + bg_top;

        int32_t thick = ( _p_mygauge->_thick )?  _p_mygauge->_thick : map_f( 2*_r ,0, 240, 0, 30); 

        arc_dsc.center      = center;
        arc_dsc.radius      = _r;
        arc_dsc.width       = thick;
        arc_dsc.rounded     = false;

        color_hsv_t hsv[2];
        hsv[0] = TFT_COLOR_TO_HSV( _p_mygauge->_color[0]);
        hsv[1] = TFT_COLOR_TO_HSV( _p_mygauge->_color[1] );

        arc_dsc.start_angle = _p_mygauge->_angle_offset;
        arc_dsc.end_angle   = arc_dsc.start_angle + _p_mygauge->_angle_total;
        arc_dsc.color       = TFT_COLOR(_p_mygauge->_color[2]) ; 
        draw_arc(layer_gauge, &arc_dsc);

        int8_t cur_arc_id = constrain(map( _p_mygauge->_val, _p_mygauge->_val_min, _p_mygauge->_val_max, 
                                      0,_p_mygauge->_arc_max_num-1), 0, _p_mygauge->_arc_max_num-1);
        // for(int i=0; i< _p_mygauge->_arc_max_num;i++){
        for(int i=0; i< cur_arc_id;i++){
          arc_dsc.start_angle = _p_mygauge->_angle_offset + _p_mygauge->_angle_unit * i - 0.01;
          arc_dsc.end_angle   = arc_dsc.start_angle + _p_mygauge->_angle_unit +0.02;
          arc_dsc.color       = (i<=cur_arc_id)? TFT_COLOR(TFT_COLOR_HSV(
                                                          map_f( i,0, _p_mygauge->_arc_max_num-1, hsv[0].h, hsv[1].h) , 
                                                          map_f( i,0, _p_mygauge->_arc_max_num-1, hsv[0].s, hsv[1].s),
                                                          map_f( i,0, _p_mygauge->_arc_max_num-1, hsv[0].v, hsv[1].v) ))
                                                : TFT_COLOR(_p_mygauge->_color[2]) ; 
          if(_p_mygauge->_val == _p_mygauge->_val_min && i==0) {
            arc_dsc.color = TFT_COLOR( _p_mygauge->_color[0]) ; 
          }
          /*************************/
          draw_arc(layer_gauge, &arc_dsc);
          /*************************/
        }

        draw_label_dsc_t label_dsc;
        draw_label_dsc_init(&label_dsc);
        static String txt;
        txt = String(_p_mygauge->_val, (unsigned long) _p_mygauge->_decimal );
        label_dsc.text  = txt.c_str();
        label_dsc.font  = _p_mygauge->font(GPART_INDICATOR);
        label_dsc.color = TFT_COLOR(_p_mygauge->font_color(GPART_INDICATOR));
        label_dsc.ofs_y = -10;

        area_t label_area;
        text_get_area(&label_area, label_dsc.text, label_dsc.font);
        area_align(&gauge_area, &label_area, ALIGN_CENTER);
        draw_label(layer_gauge, &label_dsc, &label_area);

        /*************************/
        /* วาด layer ใหม่ บน layer หลัก */
        /*************************/
        draw_image_dsc_t layer_draw_dsc;
        draw_image_dsc_init(&layer_draw_dsc);
        layer_draw_dsc.src = layer_gauge;
        render_layer(DRAW_LAYER, &layer_draw_dsc, &gauge_area);
        /*************************/
      });
    }

    inline void thick(int32_t t)            { create(); this->_thick = t; this->invalidate(); }
    inline void value(float v)              { create(); this->_val = constrain(v, this->_val_min,this->_val_max); this->invalidate(); }
    inline void value_min(float v)          { create(); this->_val_min = min( v, this->_val_max); this->_val_max = max(v, this->_val_max); this->value(this->_val); }
    inline void value_max(float v)          { create(); this->_val_max = max( v, this->_val_min); this->_val_max = min(v, this->_val_min); this->value(this->_val); }
    inline void decimal(uint8_t decimal)    { create(); this->_decimal = decimal; this->invalidate(); }
    inline void color(color_t color1, color_t color2)     { create(); this->_color[0] = color1, this->_color[1] = color2; this->invalidate();   }

    inline float value()                    { return this->_val;  }
};

GMyGauge gauge;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  gauge.align(ALIGN_CENTER);
  gauge.value(92.4);
  gauge.decimal(1);
  gauge.color(TFT_VIOLET, TFT_ORANGE);

  static SoftTimer timer;
  timer.setInterval(10, []() {
    static float step = 0.1;
    
    // ปรับทิศทางของ step เมื่อค่าเกจถึงขอบเขต
    if (gauge.value() >= 100.0) {
      step = -0.1;
    } else if (gauge.value() <= 0.0) {
      step = 0.1;
    }

    // อัปเดตค่าเกจตาม step
    gauge.value(gauge.value() + step);
  });

}

void loop()
{
  BlynkGO.update();
}
