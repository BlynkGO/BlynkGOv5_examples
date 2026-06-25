#ifndef __CWC_STATSTRIPCARD_H__
#define __CWC_STATSTRIPCARD_H__

#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
//  CWC: StatStripCard — card แสดงค่าเดี่ยว มีแถบสีด้านข้าง
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  StatStripCard card;
//
//  card.align_center();
//  card.icon(SYMBOL_THERMOSTAT);
//  card.icon_font(my_icons_24, TFT_RED);   // เปลี่ยน icon font + สี
//  card.icon_font(my_icons_24);            // หรือเปลี่ยนแค่ font คงสีเดิม
//  card.title("อุณหภูมิ");
//  card.value(28.5f, 1);                   // หรือ card = 28.5f;
//  card.unit("°C");
//  card.stripColor(TFT_RED);
//  card.stripSide(BORDER_SIDE_TOP);
//
//  float v = card.toFloat();
//
//  SIZE OVERRIDE (optional — card defaults to content-driven SIZE_CONTENT):
//  ─────────────────────────────────────────────────────────────
//  card.min_size(160, 70);   // กันเล็กเกินไป
//  card.max_size(320, 140);  // กันใหญ่เกินไป
//
//  INTERNAL GRID LAYOUT:
//  ─────────────────────────────────────────────────────────────
//  col: [GRID_CONTENT, GRID_FR(1)]   row: [GRID_CONTENT, GRID_FR(1)]
//  ┌─────────┬──────────────────────┐
//  │  icon   │       title          │  row 0
//  ├─────────┴──────────────────────┤
//  │  value          unit           │  row 1 (span 2 col)
//  └─────────────────────────────────┘
//
//  PUBLIC MEMBERS:
//  ─────────────────────────────────────────────────────────────
//  card.lb_icon   → GLabel (icon — font() เปลี่ยนได้ตรงๆ หรือผ่าน icon_font())
//  card.lb_title  → GLabel (หัวข้อ)
//  card.lb_value  → GLabel (ค่าตัวเลข ใหญ่)
//  card.lb_unit   → GLabel (หน่วย เลือก font แยกได้)
// ════════════════════════════════════════════════════════════════
class StatStripCard : public GRect {
  public:
    StatStripCard(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      // outer size: parent-filling — size() below is a fallback only,
      // overridden automatically when placed in a STRETCH grid_cell()
      GRect::size(220, 90);
      GRect::min_size(160, 70);   // กันไม่ให้เล็กจน icon+title ล้น ไม่ว่าจะมาจาก
                                  // grid stretch หรือ caller เรียก size() เอง

      GRect::radius(10);
      GRect::color(TFT_COLOR_HEX(0x1E2230));
      GRect::border(4, _strip_color);
      GRect::border_side(BORDER_SIDE_LEFT);

      // ── GRID ภายใน: 2 col x 2 row ──────────────────────────
      this->GRID_CELL(
        GRID_COL{ GRID_CONTENT, GRID_FR(1) },
        GRID_ROW{ GRID_CONTENT, GRID_FR(1) }
      , GRID_ALIGN_START, GRID_ALIGN_CENTER);
      this->padding(10, 15, 16, 12, 4);

      // ── (0,0): icon ─────────────────────────────────────────
      lb_icon.parent(this);
      lb_icon.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_icon.grid_cell(0, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);
      lb_icon = SYMBOL_BELL;

      // ── (1,0): title — same font as icon, no baseline calc needed ──
      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_title.grid_cell(1, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 6, 0);
      lb_title = "";

      // ── (0,1)-(1,1): value + unit span 2 col, centered ─────
      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value.grid_cell(0, 1, 2, 1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_value = "0";

      lb_unit.parent(this);
      lb_unit.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_unit.ignore_layout(true);   // free-position relative to lb_value
      lb_unit = "";

      _reflesh_layout();

      GRect::onSizeChanged(GWIDGET_CB{
        if(widget->event_old_size() != widget->size()){
          Serial.println("Size Changed");
          ((StatStripCard*) widget)->_reflesh_layout();
        }
      });
    }

    inline void icon(const char* s)  { create(); lb_icon  = s; }
    inline void title(const char* t) { create(); lb_title = t; }
    inline void unit(const char* u)  { create(); lb_unit  = u; }

    inline void icon_font(const font_t& f, color_t c) { create(); lb_icon.font(f, c); }
    inline void icon_font(const font_t& f)             { create(); lb_icon.font(f); }

    inline void value(float v, int32_t dec = 1) {
      create();
      _value   = v;
      lb_value = String(v, dec);
      _reflesh_layout();
    }
    inline void  operator=(float f) { value(f); }
    inline float toFloat()          { return _value; }

    inline void stripColor(GColor c) {
      create();
      _strip_color = c;
      GRect::border(4, _strip_color);
    }

    inline void stripWidth(int32_t px) { create(); GRect::border(px, _strip_color); }
    inline void stripSide(border_side_t side) { create(); GRect::border_side(side); }

    GLabel lb_icon;
    GLabel lb_title;
    GLabel lb_value;
    GLabel lb_unit;

    using GObject::align;
    using GObject::parent;
    using GObject::width;
    using GObject::height;
    using GObject::min_size;
    using GObject::max_size;

  private:
    color_t _strip_color = TFT_COLOR_HEX(0x3B82F6);
    float   _value        = 0;
    void _reflesh_layout(){
      // bl_offset: BebasNeueBold_num_45 vs prasanmit_20
      const font_t* fv = lb_value.font();
      const font_t* fu = lb_unit.font();
      int32_t dist_v = (fv->line_height - fv->base_line) - fv->line_height / 2;
      int32_t dist_u = (fu->line_height - fu->base_line) - fu->line_height / 2;
      int32_t bl_offset = dist_v - dist_u;
      lb_unit.align(lb_value, ALIGN_RIGHT, 6, bl_offset);
    }
};

#endif // __CWC_STATSTRIPCARD_H__