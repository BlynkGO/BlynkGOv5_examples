#ifndef __CWC_GGAUGE_GRAFANA_H__
#define __CWC_GGUAGE_GRAFANA_H__

// ════════════════════════════════════════════════════════════════
//  CWC: GGaugeGRAFANA — Grafana-style gauge
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  GGaugeGRAFANA gauge;
//
//  gauge.size(200, 200);
//  gauge.align_center();
//  gauge.title("อุณหภูมิ");
//  gauge.unit("°C");
//  gauge.decimal(1);           // ทศนิยม 0-3 (default = 1)
//  gauge.range(0, 100);
//  gauge.sections(
//    {0, 30, 60, 80, 100},
//    { TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_RED }
//  );
//  gauge.value(42.5f);         // หรือ gauge = 42.5f;
//
//  SECTIONS COLOR — รับได้ 3 แบบ มิกซ์กันได้เลย:
//  ─────────────────────────────────────────────────────────────
//  gauge.sections(
//    {0, 25, 50, 75, 100},
//    {
//      TFT_COLOR_HSV(220,100,100),  // color_t  : HSV
//      TFT_GREEN,                   // color_t  : named color
//      "#FFAA00",                   // "#RRGGBB": hex string
//      TFT_RED                      // color_t  : named color
//    }
//  );
//
//  PUBLIC MEMBERS (direct access):
//  ─────────────────────────────────────────────────────────────
//  gauge.arc       → GArc   (วงแหวนค่า)
//  gauge.lb_value  → GLabel (ตัวเลข)
//  gauge.lb_title  → GLabel (หัวข้อ)
//  gauge.lb_unit   → GLabel (หน่วย)
//
// ════════════════════════════════════════════════════════════════



#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
// BlynkGOv5 รุ่นก่อน 950_097 ให้เปิด comment นี้ด้วย
// ════════════════════════════════════════════════════════════════
// ── GColor: รับได้ทั้ง color_t, "#RRGGBB", String ──────────
// struct GColor {
//   color_t c;
//   GColor(color_t     v) : c(v)                        {}
//   GColor(const char* s) : c(TFT_COLOR_HEX(String(s))) {}
//   GColor(String      s) : c(TFT_COLOR_HEX(s))          {}
//   operator color_t() const { return c; }
// };
// ════════════════════════════════════════════════════════════════


class GGaugeGRAFANA : public GScale {
  public:
    GGaugeGRAFANA(GWidget& parent = GScreen)
      : GScale(parent),
        _ranges{0, 40, 60, 80, 100},
        _sect_colors{ TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED }
    {}

    void create() {
      if (GScale::isCreated()) return;
      GScale::create();
      GScale::size(200, 200);
      GScale::min_size(150, 150);
      GScale::range(0, 100);
      GScale::angle_offset(170);
      GScale::angle_range(200);
      GScale::mode(SCALE_MODE_ROUND);
      GScale::tick_opa(0);
      GScale::showLabel(false);

      _build_sections();

      arc.parent(this);
      arc.size(PCT(90), PCT(90));
      arc.thickness(0.1 * GScale::width(), TFT_GREEN);
      arc.angle_offset(GScale::angle_offset());
      arc.angle_range(GScale::angle_range());
      arc.bg_enable(true);
      arc.bg_color(TFT_COLOR_HEX(0x2d2f34));
      arc.range(0, 100);
      arc.clickable(false);
      arc.invalidate();

        lb_value.parent(this);
        lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
        lb_value = 0;

        lb_title.parent(this);
        lb_title.font(prasanmit_20, TFT_SILVER);
        lb_title = "Title";

        lb_unit.parent(this);
        lb_unit.font(prasanmit_20, TFT_DARKGRAY);
        lb_unit = "";

        lb_title.align(lb_value, ALIGN_TOP);
        lb_unit.align(lb_value, ALIGN_BOTTOM);

      refresh_size();
      GScale::onSizeChanged(GWIDGET_CB {
        GGaugeGRAFANA* g = (GGaugeGRAFANA*) widget;
        if(g->width() != g->height()) {
          int32_t min_w_h = min(g->width(), g->height());
          g->size(min_w_h, min_w_h);
        }
        if(g->event_old_size() != g->size() ) {
          g->refresh_size();
        }
      });
    }

    inline void title(String t)    { create(); lb_title = t;                  }
    inline void unit(String u)     { create(); lb_unit  = u;                  }
    inline void decimal(uint8_t d) { create(); _decimal = min(d, (uint8_t)3); }
    inline uint8_t decimal()       { return _decimal; }
    inline void operator=(float f) { create(); this->value(f); }

    void value(float v) {
      create();
      _value   = v;
      lb_value = String(_value, (unsigned int)_decimal);
      arc      = (int32_t)v;
      refresh_size();  // ต้องมีจะได้ปรับสี arc ด้วย
    }

    float value(){
      create();
      return lb_value.toFloat();
    }

    inline float toFloat()    { return value(); }

    void range(int32_t mn, int32_t mx) {
      create();
      GScale::range(mn, mx);
      arc.range(mn, mx);
      this->value(_value);
    }

    void sections(std::vector<uint32_t> ranges, std::vector<GColor> colors) {
      _ranges.clear();
      for (auto& r : ranges) _ranges.push_back(r);
      _sect_colors.clear();
      for (auto& g : colors) _sect_colors.push_back(g.c);

      if (GScale::isCreated()) {
        GScale::clearSection();
        _build_sections();
        refresh_size();
      }
    
    }

    void refresh_size() {
      if (!this->isCreated()) return;
      arc.size(PCT(90), PCT(90));
      arc.thickness(0.1 * this->width(), TFT_GREEN);
      int8_t n = min(_ranges.size() - 1, _sect_colors.size());
      auto _arc_v = constrain((int32_t)_value, 0, arc.max_value());
      for (int i = 1; i <= n; i++) {
        if (_arc_v <= _ranges[i]) { arc.color(_sect_colors[i-1]); break; }
      }
    }

    GArc   arc;
    GLabel lb_value;
    GLabel lb_title;
    GLabel lb_unit;

    using GObject::size;
    using GObject::parent;
    using GObject::align;
    using GObject::width;
    using GObject::height;

  private:
    std::vector<uint32_t> _ranges;
    std::vector<color_t>  _sect_colors;
    float   _value   = 0;
    uint8_t _decimal = 1;

    void _build_sections() {
      int8_t n = min(_ranges.size() - 1, _sect_colors.size());
      for (int i = 0; i < n; i++) {
        GScaleSection* section = GScale::addSection();
        section->range(_ranges[i], _ranges[i + 1]);
        section->thickness(5, _sect_colors[i]);
      }
    }
};


#endif // __CWC_GGUAGE_GRAFANA_H__