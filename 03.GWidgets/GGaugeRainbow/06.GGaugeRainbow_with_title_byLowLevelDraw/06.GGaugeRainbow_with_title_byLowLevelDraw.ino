/*
 * ตัวอย่างสร้าง อนุพันธุ์ class จากวิตเจ็ต GGaugeRainbow
 * ที่มีการเพิ่ม title เข้าไป แบบ ไม่ต้องเปลืองเม็มในการสร้าง วิตเจ็ต GLabel เพิ่ม
 * (เขียนยากกว่า แต่ประหยัดเม็มขึ้น)
 */


#include <BlynkGOv5.h>

class GMyGaugeRainbow : public GGaugeRainbow
{
  public:
    GMyGaugeRainbow(GWidget&parent=GScreen) : GGaugeRainbow(parent) {}
    String _title = "Title";
    void create(){
      if(this->isCreated()) return;
      GGaugeRainbow::create();
      GGaugeRainbow::size(200,200);
      GGaugeRainbow::padding(45,20,20,20);
      GGaugeRainbow::border(1, TFT_SILVER);
      GGaugeRainbow::radius(20);
      GGaugeRainbow::font(prasanmit_25, TFT_SILVER);    // font, สีฟอนต์ ของ title ใช้ GPART_MAIN
      GGaugeRainbow::onDrawMainEnd(GWIDGET_CB{
        GMyGaugeRainbow* _p_gauge =(GMyGaugeRainbow*) widget;
        if(_p_gauge->_title != "") {
          draw_label_dsc_t label_dsc;
          draw_label_dsc_init(&label_dsc);
          label_dsc.text  = _p_gauge->_title.c_str();
          label_dsc.font  = _p_gauge->font();
          label_dsc.color = TFT_COLOR(_p_gauge->font_color());
          area_t label_area;
          text_get_area(&label_area, label_dsc.text, label_dsc.font);
          area_align(_p_gauge->coords(), &label_area, ALIGN_TOP_MID, 0, 5);
          draw_label(DRAW_LAYER, &label_dsc, &label_area);
        }
      });
    }
    inline void title(String t)     { create(); this->_title = t; this->invalidate(); }
    inline String title()           { return this->_title; }

    using GGaugeRainbow::operator=;
};

GMyGaugeRainbow gauge;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  gauge.title("อุณหภูมิ");
  gauge = 89.34;

}

void loop()
{
  BlynkGO.update();
}
