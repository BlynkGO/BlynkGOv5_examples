#ifndef __CWC_GGAUGERING_H__
#define __CWC_GGAUGERING_H__

// ════════════════════════════════════════════════════════════════
//  CWC: GGaugeRing — Gauge วงกลมแบบใช้ GScale Section เท่านั้น
//                    (ไม่มี GArc — section หนาๆ ทำหน้าที่เป็นวงแหวนสี)
//                    + GCircle dot — indicator ลูกบอลวิ่งตามค่าบนวงแหวน
//
//  VISUAL:
//  ─────────────────────────────────────────────────────────────
//        ╭──────●───╮
//      ╱  🔵🟢🟡🔴   ╲    ← GScale section หนา + dot (●) ชี้ค่าปัจจุบัน
//     │    31.7       │   ← lb_value (gauge_num_55)
//     │     °C        │   ← lb_unit
//      ╲             ╱
//        ╰─────  ────╯    ← เปิดล่าง สมมาตร (angle_range 270°)
//
//  FONT แนะนำ:
//  ─────────────────────────────────────────────────────────────
//  Oxanium Medium — wide, geometric, เหมาะกับ gauge
//  https://fonts.google.com/specimen/Oxanium
//  → ดาวน์โหลด Oxanium-Medium.ttf แล้วรัน:
//  fontconv Oxanium-Medium.ttf --size 55 --name gauge
//  จากนั้นแก้ lb_value.font() เป็น gauge_num_55
//
//  INTERNAL STRUCTURE:
//  ─────────────────────────────────────────────────────────────
//  this (GRect — transparent container)
//   ├─ scale    (GScale)   — วงแหวนสี section หนา, size PCT(98)
//   ├─ dot      (GCircle)  — indicator ตำแหน่งค่าปัจจุบันบนวงแหวน
//   ├─ lb_title (GLabel)   — หัวข้อ เหนือ lb_value (optional)
//   ├─ lb_value (GLabel)   — ตัวเลขหลัก align_center
//   └─ lb_unit  (GLabel)   — หน่วย ใต้ lb_value
//
//  DOT POSITIONING (สูตรยืนยันแล้วจากการทดสอบจริงบนจอ):
//  ─────────────────────────────────────────────────────────────
//  ใช้ dot.center(Point(radius, angle, angle_offset, GRect::centerpoint(),
//  CLOCKWISE)) ตาม pattern ของ BMI_Gauge indicator ใน examples
//
//  radius ที่ทำให้ dot อยู่กึ่งกลางแถบสี section พอดี:
//    r = scale.width()/2 − (_thick/2)
//
//  VALUE OVER/UNDER RANGE:
//  ─────────────────────────────────────────────────────────────
//  - lb_value (ตัวเลขกลาง) แสดงค่าจริงเสมอ แม้เกิน range (ไม่ clamp)
//  - dot (indicator บนวงแหวน) clamp ไว้ที่ขอบ min/max ของ range เสมอ
//    เพื่อไม่ให้ dot วิ่งหลุดออกนอกแถบสี section
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  GGaugeRing gauge;
//
//  gauge.size(280, 280);
//  gauge.align_center();
//  gauge.title("TEMP");          // optional
//  gauge.unit("°C");
//  gauge.decimal(1);
//  gauge.range(0, 50);
//  gauge.sections(
//    {0, 15, 30, 40, 50},
//    { TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED }
//  );
//  gauge.value(31.7f);           // หรือ gauge = 31.7f;  → dot ขยับตามอัตโนมัติ
//  gauge.value(120.0f);          // เกิน range(0,50) → ตัวเลขแสดง "120.0"
//                                // แต่ dot หยุดที่ปลายสุด (max) ของวงแหวน
//
//  float v = gauge.toFloat();
//
//  SECTION THICKNESS (default = 20px):
//  ─────────────────────────────────────────────────────────────
//  gauge.section_thick(25);      // dot จะปรับ orbit radius ตามอัตโนมัติ
//
//  DOT STYLE:
//  ─────────────────────────────────────────────────────────────
//  gauge.dot_size(16);
//  gauge.dot_color(TFT_WHITE);
//
//  PUBLIC MEMBERS:
//  ─────────────────────────────────────────────────────────────
//  gauge.scale     → GScale   (วงแหวนสี — เข้าถึงตรงๆ ได้)
//  gauge.dot       → GCircle  (indicator ลูกบอล)
//  gauge.lb_value  → GLabel   (ตัวเลขหลัก)
//  gauge.lb_title  → GLabel   (หัวข้อ เหนือตัวเลข)
//  gauge.lb_unit   → GLabel   (หน่วย ใต้ตัวเลข)
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include <vector>

// ── FONT DECLARE (uncomment เมื่อ build font แล้ว) ──────────────
FONT_DECLARE(gauge_num_55);   // Oxanium-Medium size 55

class GGaugeRing : public GRect {
  public:
    GScale  scale;
    GLabel  lb_value;
    GLabel  lb_title;
    GLabel  lb_unit;
    GCircle dot;

    GGaugeRing(GWidget& parent = GScreen)
      : GRect(parent),
        _ranges{ 0, 25, 50, 75, 100 },
        _sect_colors{ TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED }
    {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      // ── container โปร่งใส — ขนาดถูก stretch จาก grid หรือกำหนดเอง ──
      GRect::size(280, 280);
      GRect::min_size(120, 120);
      GRect::bg_opa(0);
      GRect::radius(RADIUS_CIRCLE);
      GRect::border(2);

      // ── GScale (วงแหวนสี section) ──────────────────────────────
      scale.parent(this);
      scale.size(PCT(98), PCT(98)); // 98% ของ แม่
      scale.range(0, 100);
      scale.angle_offset(135);    // เริ่ม ~7 นาฬิกา สมมาตร
      scale.angle_range(270);     // กวาด 270° → จบ ~5 นาฬิกา เปิดล่าง
      scale.mode(SCALE_MODE_ROUND);
      scale.tick_opa(0);          // ซ่อน tick — section ทำหน้าที่วงแหวนทั้งหมด
      scale.showLabel(false);

      _build_sections();

      // ── Labels กลางวง ─────────────────────────────────────────
      lb_title.parent(this);
      lb_title.font(prasanmit_25, TFT_COLOR_HEX(0xAAAAAA));
      lb_title = "";

      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      // ↑ เปลี่ยนเป็น gauge_num_55 เมื่อ build Oxanium-Medium ด้วย fontconv แล้ว
      lb_value = "0";
      lb_value.update_layout();   // force resolve ขนาด ก่อน align ตาม

      lb_unit.parent(this);
      lb_unit.font(prasanmit_25, TFT_COLOR_HEX(0xAAAAAA));
      lb_unit = "";

      // ── GCircle dot (indicator ตำแหน่งค่าปัจจุบัน) ──────────────
      dot.parent(this);
      dot.size(_dot_size, _dot_size);
      dot.border(1, TFT_BLACK);
      dot.color(TFT_WHITE);
      _refresh_dot();   // วางตำแหน่งเริ่มต้นตาม _value (default = 0)

      _refresh_labels();

      GRect::onSizeChanged(GWIDGET_CB{
        if (widget->event_old_size() == widget->size()) return;
        GGaugeRing* g = (GGaugeRing*) widget;
        g->scale.size(PCT(98), PCT(98));
        g->_refresh_labels();
        g->_refresh_dot();
      });
    }

    // ── Public API ─────────────────────────────────────────────
    inline void title(String t)    { create(); lb_title = t;                 }
    inline void unit(String u)     { create(); lb_unit  = u;                 }
    inline void decimal(uint8_t d) { create(); _decimal = min(d,(uint8_t)3); }
    inline uint8_t decimal()       { return _decimal; }
    inline void  operator=(float f){ create(); value(f); }
    inline float toFloat()         { return _value; }

    // ค่าจริงแสดงบน lb_value ตรงๆ แม้เกิน range (ไม่ clamp ตัวเลข)
    // ส่วน dot จะถูก clamp ไว้ที่ขอบ range ใน _refresh_dot()
    void value(float v) {
      create();
      _value   = v;
      lb_value = String(_value, (unsigned int)_decimal);
      _refresh_dot();
    }

    void range(int32_t mn, int32_t mx) {
      create();
      scale.range(mn, mx);
      _range_min = mn;
      _range_max = mx;
      value(_value);
    }

    void sections(std::vector<uint32_t> ranges, std::vector<GColor> colors) {
      _ranges.clear();
      for (auto& r : ranges) _ranges.push_back(r);
      _sect_colors.clear();
      for (auto& c : colors) _sect_colors.push_back(c.c);
      if (GRect::isCreated()) {
        scale.clearSection();
        _build_sections();
      }
    }

    void section_thick(int32_t t) {
      create();
      _thick = t;
      scale.clearSection();
      _build_sections();
      // _refresh_dot();
      this->dot_size(t);
    }

    // ── Dot style API ─────────────────────────────────────────
    void dot_size(int32_t d) {
      create();
      _dot_size = d;
      dot.size(_dot_size, _dot_size);
      _refresh_dot();
    }

    void dot_color(color_t c) {
      create();
      dot.color(c);
    }

    using GObject::align;
    using GObject::align_center;
    using GObject::parent;
    using GObject::width;
    using GObject::height;
    using GObject::min_size;
    using GObject::max_size;

  private:
    std::vector<uint32_t> _ranges;
    std::vector<color_t>  _sect_colors;
    float   _value     = 0;
    uint8_t _decimal   = 1;
    int32_t _thick     = 20;
    int32_t _dot_size  = 16;
    int32_t _range_min = 0;
    int32_t _range_max = 100;

    void _build_sections() {
      int8_t n = min(_ranges.size() - 1, _sect_colors.size());
      for (int i = 0; i < n; i++) {
        GScaleSection* s = scale.addSection();
        s->range(_ranges[i], _ranges[i + 1]);
        s->thickness(_thick, _sect_colors[i]);
      }
    }

    void _refresh_labels() {
      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,    0, 0);
      lb_unit.align( lb_value, ALIGN_BOTTOM, 0, 0);
    }

    // วาง dot บนกึ่งกลางความหนาของ section ring ตามค่า _value ปัจจุบัน
    // ค่าที่ใช้คำนวณตำแหน่ง dot จะถูก clamp ไว้ในช่วง [range_min, range_max]
    // เสมอ — เพื่อไม่ให้ dot วิ่งหลุดออกนอกแถบสี section เมื่อค่าจริงเกิน
    // range (ตัวเลขบน lb_value ยังแสดงค่าจริงตรงๆ ไม่ถูก clamp)
    void _refresh_dot() {
      if (!GRect::isCreated()) return;

      int32_t scale_outer_r = scale.width() / 2;
      if (scale_outer_r <= 0) return;

      // กึ่งกลางความหนาของแถบ section บน scale ring
      // (ค่านี้ยืนยันแล้วจากการทดสอบจริงบนจอ — dot อยู่กึ่งกลางแถบสีพอดี)
      float r = scale_outer_r - (_thick / 2.0);
      if (r < 0) r = 0;

      // clamp ค่าไว้ในช่วง range ก่อนคำนวณมุม — dot จะไปสุดที่ปลาย
      // min/max ของ scale section เท่านั้น ไม่วิ่งเกินออกไป
      float v_clamped = _value;
      if (v_clamped < (float)_range_min) v_clamped = (float)_range_min;
      if (v_clamped > (float)_range_max) v_clamped = (float)_range_max;

      float angle_deg = map_f((v_clamped * 100),
                               _range_min * 100,
                               _range_max * 100,
                               0, scale.angle_range());

      dot.center( Point( r,
                         angle_deg,
                         scale.angle_offset(),
                         GRect::centerpoint(),
                         CLOCKWISE) );
    }
};

#endif // __CWC_GGAUGERING_H__