#ifndef __CWC_STOCKCARD_H__
#define __CWC_STOCKCARD_H__

#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
//  CWC: StockCard — card แสดงค่า + mini trend chart
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  StockCard card;
//
//  card.size(220, 160);
//  card.align_center();
//  card.title("AAPL");
//  card.unit("USD");
//  card.color(TFT_CYAN);
//  card.decimal(1);
//  card.value(151.2f);
//
//  PUBLIC MEMBERS:
//  ─────────────────────────────────────────────────────────────
//  card.lb_title  → GLabel (หัวข้อ)
//  card.lb_value  → GLabel (ค่าตัวเลข)
//  card.lb_unit   → GLabel (หน่วย)
//  card.chart     → GChart (mini trend)
//  card.series    → chart_series_t*
// ════════════════════════════════════════════════════════════════

class StockCard : public GRect {
  public:
    StockCard(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      GRect::radius(12);
      GRect::border(0);
      GRect::bg_color(TFT_COLOR_HEX(0x1E2230));

      // ── GRID ภายใน: 3 row — title / value / chart ──────────
      this->GRID_CELL(
        GRID_COL{ GRID_FR(1) },
        GRID_ROW{ GRID_CONTENT, GRID_FR(1), 50 }
      );
      this->padding(12, 4, 14, 12, 4);

      // ── row 0: title ────────────────────────────────────────
      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_title.grid_cell(0, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);
      lb_title = "";

      // ── row 1: value (ซ้าย) + unit (ขวาล่าง) ───────────────
      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value.grid_cell(0, 1, GRID_ALIGN_START, GRID_ALIGN_CENTER);
      lb_value = "0";

      lb_unit.parent(this);
      lb_unit.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_unit.grid_cell(0, 1, GRID_ALIGN_END, GRID_ALIGN_END);
      lb_unit = "";

      // ── row 2: mini chart ────────────────────────────────────
      chart.parent(this);
      chart.grid_cell(0, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      chart.chart_type(CHART_TYPE_LINE);
      chart.point_count(_POINTS);
      chart.range(0, 100);
      chart.bg_opa(0);
      chart.border(0);
      chart.line_width(2, GPART_ITEMS);
      chart.line_opa(220, GPART_ITEMS);
      chart.draw_grad(true);
      chart.div_line_count(0, 0);

      series = chart.createSerie(TFT_CYAN);
    }

    inline void title(const char* t) { create(); lb_title = t; }
    inline void unit(const char* u)  { create(); lb_unit  = u; }
    inline void color(color_t c)     { create(); chart.series_color(series, c); }
    inline void decimal(uint8_t d)   { create(); _decimal = min(d, (uint8_t)3); }
    inline uint8_t decimal()         { return _decimal; }

    void value(float v) {
      create();
      _value   = v;
      lb_value = String(v, (unsigned int)_decimal);
      chart.addPoint(series, (int32_t)v);
      _update_range();
    }

    inline void operator=(float f) { value(f); }

    GLabel          lb_title;
    GLabel          lb_value;
    GLabel          lb_unit;
    GChart          chart;
    chart_series_t* series = nullptr;

    using GObject::size;
    using GObject::align;
    using GObject::parent;
    using GObject::width;
    using GObject::height;

  private:
    static const int _POINTS  = 20;
    float   _value   = 0;
    uint8_t _decimal = 1;

    void _update_range() {
      int32_t* arr = chart.y_array(series);
      if (!arr) return;

      int32_t mn = INT32_MAX, mx = INT32_MIN;
      for (int i = 0; i < _POINTS; i++) {
        if (arr[i] == LV_CHART_POINT_NONE) continue;
        if (arr[i] < mn) mn = arr[i];
        if (arr[i] > mx) mx = arr[i];
      }
      if (mn == INT32_MAX) return;

      int32_t pad = max((int32_t)1, (mx - mn) / 5);
      chart.range(mn - pad, mx + pad);
    }
};

#endif // __CWC_STOCKCARD_H__
