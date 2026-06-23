#ifndef __CWC_GGAUGE_DIAL_H__
#define __CWC_GGAUGE_DIAL_H__

// ════════════════════════════════════════════════════════════════════
//  CWC: GGaugeDial — Grafana-style gauge เต็มวง — เริ่ม 12 นาฬิกา
//
//  [FIX] refresh_size() rebuild GScale + section thickness
//        ให้ scale ตามขนาดจริงทุกครั้งที่ size เปลี่ยน
//
//  QUICK START:
//  gauge.size(200, 200);
//  gauge.align_center();
//  gauge.title("อุณหภูมิ");
//  gauge.unit("°C");
//  gauge.decimal(1);
//  gauge.range(0, 100);
//  gauge.sections({0,30,60,80,100},{TFT_BLUE,TFT_GREEN,TFT_YELLOW,TFT_RED});
//  gauge.value(42.5f);
//
//  PUBLIC MEMBERS:
//  gauge.arc      → GArc
//  gauge.lb_value → GLabel
//  gauge.lb_title → GLabel
//  gauge.lb_unit  → GLabel
// ════════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>

class GGaugeDial : public GScale {
  public:
    GGaugeDial(GWidget& parent = GScreen)
      : GScale(parent),
        _ranges{0, 40, 60, 80, 100},
        _sect_colors{ TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED }
    {}

    void create() {
      if (GScale::isCreated()) return;
      GScale::create();
      GScale::size(200, 200);
      GScale::range(0, 100);
      GScale::angle_offset(270);
      GScale::angle_range(360);
      GScale::mode(SCALE_MODE_ROUND);
      GScale::tick_opa(0);
      GScale::showLabel(false);

      _build_sections_scaled(5);

      arc.parent(this);
      arc.size(PCT(90), PCT(90));
      arc.thickness(0.1 * GScale::width(), TFT_GREEN);
      arc.angle_offset(270);
      arc.angle_range(360);
      arc.bg_enable(true);
      arc.bg_color(TFT_COLOR_HEX(0x2d2f34));
      arc.range(0, 100);
      arc.clickable(false);

      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value = 0;

      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_SILVER);
      lb_title = "Title";

      lb_unit.parent(this);
      lb_unit.font(prasanmit_20, TFT_DARKGRAY);
      lb_unit = "";

      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,    0, 0);
      lb_unit.align( lb_value, ALIGN_BOTTOM, 0, 0);

      this->onSizeChanged(GWIDGET_CB {
        GGaugeDial* g = (GGaugeDial*) widget;
        if(g->event_old_size() != g->size()) g->refresh_size();
      });

      this->refresh_size();
    }

    inline void title(String t)    { create(); lb_title = t;                        refresh_size(); }
    inline void unit(String u)     { create(); lb_unit  = u;                        refresh_size(); }
    inline void decimal(uint8_t d) { create(); _decimal = min(d, (uint8_t)3);                      }
    inline uint8_t decimal()       { return _decimal; }
    inline void operator=(float f) { create(); this->value(f); }

    void value(float v) {
      create();
      _value   = v;
      lb_value = String(_value, (unsigned int)_decimal);
      arc      = (int32_t)v;
      refresh_size();
    }

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
        refresh_size();   // refresh_size จะ clearSection + rebuild เอง
      }
    }

    void refresh_size() {
      if (!this->isCreated()) return;

      // ── arc: 90% ของ parent, thickness 10% ───────────────────────
      arc.size(PCT(90), PCT(90));
      float thick = 0.1f * this->width();
      arc.thickness((int32_t)thick, TFT_GREEN);

      // ── section indicator: scale 2.5% ของ width (min 2px) ────────
      int32_t sect_thick = (int32_t)(0.025f * this->width());
      if (sect_thick < 2) sect_thick = 2;
      GScale::clearSection();
      _build_sections_scaled(sect_thick);

      // ── labels ───────────────────────────────────────────────────
      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,    0, 0);
      lb_unit.align( lb_value, ALIGN_BOTTOM, 0, 0);

      // ── arc color ตาม value ───────────────────────────────────────
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

    void _build_sections_scaled(int32_t sect_thick) {
      int8_t n = min(_ranges.size() - 1, _sect_colors.size());
      for (int i = 0; i < n; i++) {
        GScaleSection* section = GScale::addSection();
        section->range(_ranges[i], _ranges[i + 1]);
        section->thickness(sect_thick, _sect_colors[i]);
      }
    }
};

#endif // __CWC_GGAUGE_DIAL_H__