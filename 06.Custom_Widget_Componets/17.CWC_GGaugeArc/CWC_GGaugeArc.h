#ifndef __CWC_GGAUGEARC_H__
#define __CWC_GGAUGEARC_H__

// ════════════════════════════════════════════════════════════════
//  CWC: GGaugeArc — Gauge วงกลมแบบใช้ GArc หลายตัวซ้อนกัน (zone arcs)
//                   + GCircle dot — indicator ลูกบอลวิ่งตามค่าบนวงแหวน
//
//  ต่างจาก GGaugeRing (ที่ใช้ GScale + Section) — ตัวนี้ใช้ GArc
//  หลายตัวสร้าง "โซนสี" เอง (cyan/green/amber/red) ตาม pattern จาก
//  example "05.GArc_4arcs.ino" ใน 02_BlynkGOv5_examples_All.txt
//
//  อ้างอิงจาก HTML mockup: dashboard_mockup.html (hero gauge ซ้ายบน)
//
//  VISUAL:
//  ─────────────────────────────────────────────────────────────
//        ╭──────●───╮
//      ╱  🔵🟢🟡🔴   ╲    ← 4× GArc โซนสี (เรียงต่อกัน) + dot (●)
//     │  อุณหภูมิ      │   ← lb_room (label เล็กบนสุด)
//     │    24.5       │   ← lb_value (gauge_num_55)
//     │     °C        │   ← lb_unit
//      ╲             ╱
//        ╰─────  ────╯    ← เปิดล่าง สมมาตร (angle_range 270°)
//
//  FONT แนะนำ:
//  ─────────────────────────────────────────────────────────────
//  Oxanium Medium — wide, geometric, เหมาะกับ gauge (ตรงกับ mockup)
//  https://fonts.google.com/specimen/Oxanium
//  → ดาวน์โหลด Oxanium-Medium.ttf แล้วรัน:
//  fontconv Oxanium-Medium.ttf --size 46 --name gauge
//  จากนั้นแก้ lb_value.font() เป็น gauge_num_46
//
//  INTERNAL STRUCTURE:
//  ─────────────────────────────────────────────────────────────
//  this (GRect — transparent container)
//   ├─ bg_track (GArc)     — track พื้นหลังเข้ม เต็มความยาว 270°
//   ├─ zone[0..3] (GArc)   — 4 โซนสี เรียงต่อกันตาม _ranges
//   ├─ dot       (GCircle) — indicator ตำแหน่งค่าปัจจุบัน
//   ├─ lb_room   (GLabel)  — ชื่อห้อง/ป้ายเล็กบนสุด (optional)
//   ├─ lb_value  (GLabel)  — ตัวเลขหลัก align_center
//   └─ lb_unit   (GLabel)  — หน่วย ใต้ lb_value
//
//  ARC ANGLE MATH (pattern จาก GArc_4arcs.ino):
//  ─────────────────────────────────────────────────────────────
//  ทุก arc ใช้ angle_offset เดียวกัน (135° — เริ่ม ~7 นาฬิกา สมมาตร)
//  แล้วใช้ set_angle(start, end) วาดเฉพาะช่วงของตัวเอง โดย start/end
//  คำนวณจาก map(_ranges[i], range_min, range_max, 0, 270)
//
//  DOT POSITIONING:
//  ─────────────────────────────────────────────────────────────
//  ใช้ pattern เดียวกับ GGaugeRing (ยืนยันแล้วว่าใช้งานได้จริงบน
//  hardware ไม่ crash):
//    dot.center(Point(r, angle_deg, angle_offset, GRect::centerpoint(), CLOCKWISE))
//  โดย r = (วงนอกของ zone arc) − (thickness/2)
//  ค่าที่ใช้คำนวณตำแหน่ง dot จะถูก clamp ไว้ในช่วง range เสมอ (ไม่ให้
//  วิ่งหลุดออกนอกแถบสี แม้ value() เกิน range) ส่วนตัวเลขบน lb_value
//  ยังแสดงค่าจริงตรงๆ ไม่ถูก clamp
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  GGaugeArc gauge;
//
//  gauge.size(196, 196);
//  gauge.align_center();
//  gauge.room("Living Room");     // optional, ใต้ขอบบน
//  gauge.unit("°C");
//  gauge.decimal(1);
//  gauge.range(0, 50);
//  gauge.sections(
//    {0, 15, 30, 40, 50},
//    { TFT_COLOR_HEX(0x22D3EE),   // cyan
//      TFT_COLOR_HEX(0x22C55E),   // green
//      TFT_COLOR_HEX(0xF59E0B),   // amber
//      TFT_COLOR_HEX(0xEF4444) }  // red
//  );
//  gauge.value(24.5f);            // หรือ gauge = 24.5f;
//
//  ARC THICKNESS (default = 14px ตาม mockup):
//  ─────────────────────────────────────────────────────────────
//  gauge.thickness(16);
//
//  ANGLE CONFIG (default: offset=135°, range=270° — เปิดล่าง สมมาตร):
//  ─────────────────────────────────────────────────────────────
//  gauge.angle_offset(180);   // เริ่มจากซ้ายสุด (9 นาฬิกา)
//  gauge.angle_range(360);    // กวาดเต็มวง (ไม่มีช่องเปิด)
//
//  DOT STYLE:
//  ─────────────────────────────────────────────────────────────
//  gauge.dot_size(14);
//  gauge.dot_color(TFT_WHITE);
//
//  PUBLIC MEMBERS:
//  ─────────────────────────────────────────────────────────────
//  gauge.bg_track  → GArc     (track พื้นหลัง)
//  gauge.zone[4]   → GArc[]   (โซนสีทั้ง 4 — เข้าถึงตรงๆ ได้)
//  gauge.dot       → GCircle  (indicator ลูกบอล)
//  gauge.lb_value  → GLabel   (ตัวเลขหลัก)
//  gauge.lb_room   → GLabel   (ป้ายชื่อห้อง/หัวข้อ)
//  gauge.lb_unit   → GLabel   (หน่วย ใต้ตัวเลข)
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include <vector>

// ── FONT DECLARE (uncomment เมื่อ build font แล้ว) ──────────────
FONT_DECLARE(gauge_num_46);   // Oxanium-Medium size 46

class GGaugeArc : public GRect {
  public:
    GArc    bg_track;
    GArc    zone[4];
    GCircle dot;
    GLabel  lb_room;
    GLabel  lb_value;
    GLabel  lb_unit;

    GGaugeArc(GWidget& parent = GScreen)
      : GRect(parent),
        _ranges{ 0, 15, 30, 40, 50 },
        _zone_colors{ TFT_COLOR_HEX(0x22D3EE), TFT_COLOR_HEX(0x22C55E),
                       TFT_COLOR_HEX(0xF59E0B), TFT_COLOR_HEX(0xEF4444) }
    {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      // ── container โปร่งใส ──────────────────────────────────────
      GRect::size(196, 196);
      GRect::min_size(100, 100);
      GRect::bg_opa(0);
      GRect::radius(RADIUS_CIRCLE);
      GRect::border(0);

      // ── bg_track (พื้นหลังเข้ม เต็มความยาว 270°) ────────────────
      bg_track.parent(this);
      bg_track.size(PCT(96), PCT(96));
      bg_track.angle_offset(_angle_offset);
      bg_track.set_angle(0, _angle_range);
      bg_track.thickness(_thick, TFT_COLOR_HEX(0x1F262D));
      bg_track.rounded(true);
      bg_track.bg_enable(false);
      bg_track.clickable(false);

      // ── zone[0..3] (โซนสีเรียงต่อกัน) ───────────────────────────
      for (int i = 0; i < 4; i++) {
        zone[i].parent(this);
        zone[i].size(PCT(96), PCT(96));
        zone[i].angle_offset(_angle_offset);
        zone[i].thickness(_thick, _zone_colors[i]);
        zone[i].rounded(true);
        zone[i].bg_enable(false);
        zone[i].clickable(false);
      }
      _build_zone_angles();

      // ── Labels กลางวง ─────────────────────────────────────────
      lb_room.parent(this);
      lb_room.font(prasanmit_18, TFT_COLOR_HEX(0x8A929C));
      lb_room.text_align(TEXT_ALIGN_CENTER);
      lb_room = "";

      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      // ↑ เปลี่ยนเป็น gauge_num_46 เมื่อ build Oxanium-Medium ด้วย fontconv แล้ว
      lb_value = "0";
      lb_value.update_layout();   // force resolve ขนาด ก่อน align ตาม
      lb_value.align_center();

      lb_unit.parent(this);
      lb_unit.font(prasanmit_18, TFT_COLOR_HEX(0x8A929C));
      lb_unit = "";

      // ── GCircle dot (indicator ตำแหน่งค่าปัจจุบัน) ──────────────
      dot.parent(this);
      dot.size(_dot_size, _dot_size);
      dot.border(2, TFT_COLOR_HEX(0x0B0D10));
      dot.color(TFT_WHITE);
      dot.clickable(false);

      _refresh_labels();
      _refresh_dot();

      GRect::onSizeChanged(GWIDGET_CB{
        if (widget->event_old_size() == widget->size()) return;
        GGaugeArc* g = (GGaugeArc*) widget;
        for (int i = 0; i < 4; i++) g->zone[i].size(PCT(96), PCT(96));
        g->bg_track.size(PCT(96), PCT(96));
        g->_refresh_labels();
        g->_refresh_dot();
      });
    }

    // ── Public API ─────────────────────────────────────────────
    inline void room(String t)     { create(); lb_room = t;                  }
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
      _range_min = mn;
      _range_max = mx;
      _build_zone_angles();
      value(_value);
    }

    void sections(std::vector<uint32_t> ranges, std::vector<GColor> colors) {
      _ranges.clear();
      for (auto& r : ranges) _ranges.push_back(r);
      _zone_colors.clear();
      for (auto& c : colors) _zone_colors.push_back(c.c);
      if (GRect::isCreated()) {
        for (int i = 0; i < 4 && i < (int)_zone_colors.size(); i++) {
          zone[i].color(_zone_colors[i]);
        }
        _build_zone_angles();
      }
    }

    void thickness(int32_t t) {
      create();
      _thick = t;
      bg_track.thickness(_thick, bg_track.color());
      for (int i = 0; i < 4; i++) zone[i].thickness(_thick, zone[i].color());
      _refresh_dot();
    }

    // ── Angle config API ─────────────────────────────────────
    // ปรับมุมเริ่มต้น (องศา, 0° = แกน +x, ตามเข็มนาฬิกา) — ค่า default
    // คือ 135° (เริ่มประมาณ "7 นาฬิกา" สมมาตรซ้าย-ขวา)
    void angle_offset(int32_t deg) {
      create();
      _angle_offset = deg;
      bg_track.angle_offset(_angle_offset);
      for (int i = 0; i < 4; i++) zone[i].angle_offset(_angle_offset);
      bg_track.set_angle(0, _angle_range);
      _build_zone_angles();
      _refresh_dot();
    }
    inline int32_t angle_offset() { return _angle_offset; }

    // ปรับความยาวการกวาดของวงแหวน (องศา) — ค่า default คือ 270°
    // (เปิดล่าง สมมาตร) ลองใส่ 360 เพื่อให้เป็นวงกลมเต็มรอบ
    void angle_range(int32_t deg) {
      create();
      _angle_range = deg;
      bg_track.set_angle(0, _angle_range);
      _build_zone_angles();
      _refresh_dot();
    }
    inline int32_t angle_range() { return _angle_range; }

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
    std::vector<color_t>  _zone_colors;
    float   _value       = 0;
    uint8_t _decimal     = 1;
    int32_t _thick       = 14;
    int32_t _dot_size    = 14;
    int32_t _range_min   = 0;
    int32_t _range_max   = 50;
    int32_t _angle_offset = 135;   // เริ่ม ~7 นาฬิกา สมมาตร
    int32_t _angle_range  = 270;   // กวาด 270° เปิดล่าง

    // วาด zone[i] ให้ครอบคลุมช่วง _ranges[i] .. _ranges[i+1] บนวงแหวน
    // โดย map ค่าจริงเป็นมุม 0-270° (สัมพัทธ์กับ angle_offset เดียวกันหมด)
    void _build_zone_angles() {
      if (!GRect::isCreated()) return;
      int n = min((int)_ranges.size() - 1, 4);
      for (int i = 0; i < n; i++) {
        int32_t a_start = map(_ranges[i],     _range_min, _range_max, 0, _angle_range);
        int32_t a_end   = map(_ranges[i + 1], _range_min, _range_max, 0, _angle_range);
        zone[i].set_angle(a_start, a_end);
      }
    }

    void _refresh_labels() {
      lb_value.align_center();
      lb_room.align(lb_value, ALIGN_TOP,    0, -6);
      lb_unit.align(lb_value, ALIGN_BOTTOM, 0, 2);
    }

    // วาง dot บนกึ่งกลางความหนาของ zone arc ตามค่า _value ปัจจุบัน
    // สูตร r ใช้ pattern เดียวกับ GGaugeRing ที่ยืนยันแล้วว่าตรงจริงบนจอ:
    //   r = (วงนอกของ zone arc) - (thickness/2)
    // ค่าที่ใช้คำนวณมุมจะถูก clamp ไว้ในช่วง range เสมอ — dot จะไปสุดที่
    // ปลาย min/max ของวงแหวนเท่านั้น ไม่วิ่งเกินออกไปแม้ _value เกิน range
    void _refresh_dot() {
      if (!GRect::isCreated()) return;

      int32_t outer_r = zone[0].width() / 2;
      if (outer_r <= 0) return;

      int32_t r = outer_r - (_thick / 2);
      if (r < 0) r = 0;

      float v_clamped = _value;
      if (v_clamped < (float)_range_min) v_clamped = (float)_range_min;
      if (v_clamped > (float)_range_max) v_clamped = (float)_range_max;

      int32_t angle_deg = map((int32_t)(v_clamped * 100),
                               _range_min * 100,
                               _range_max * 100,
                               0, _angle_range);

      dot.center( Point(r,
                         angle_deg,
                         _angle_offset,
                         GRect::centerpoint(),
                         CLOCKWISE) );
    }
};

#endif // __CWC_GGAUGEARC_H__