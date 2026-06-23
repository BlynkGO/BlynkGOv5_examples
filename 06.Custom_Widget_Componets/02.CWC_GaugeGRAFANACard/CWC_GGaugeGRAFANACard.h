#ifndef __CWC_GGAUGE_GRAFANA_CARD_H__
#define __CWC_GGAUGE_GRAFANA_CARD_H__

#include <BlynkGOv5.h>
#include <vector>
#include "CWC_GGaugeGRAFANA.h"

// ════════════════════════════════════════════════════════════════
//  CWC: GGaugeGRAFANACard — GGaugeGRAFANA ที่มีพื้นหลัง card (GRect) ในตัว
//        ตั้ง title/value/unit/range/sections ได้ตรงที่ตัว card เลย
//        ไม่ต้องเรียกผ่าน .gauge ทุกครั้ง — เหมาะกับวางใน GRID_CELL dashboard
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  GGaugeGRAFANACard card;
//
//  card.size(150, 150);
//  card.title("CPU Load");           // หัวข้อ — แสดงบนสุดของ card (นอก gauge)
//  card.unit("%");                   // หน่วย  — แสดงเหนือตัวเลขกลางวง gauge
//  card.decimal(0);                  // ทศนิยม 0-3 (default = 1)
//  card.range(0, 100);
//  card.sections(
//    {0, 60, 80, 100},
//    { TFT_GREEN, TFT_YELLOW, TFT_RED }
//  );
//  card.value(42.5f);                // หรือ card = 42.5f;
//
//  float v = card.toFloat();
//
//  VISUAL LAYOUT:
//  ─────────────────────────────────────────────────────────────
//  ┌─────────────────────────┐
//  │ lb_title  "CPU Load"    │  ← card.title()  → lb_title (GLabel ของ Card)
//  │   ┌───────────────────┐ │
//  │   │   %               │ │  ← card.unit()   → gauge.lb_title (เหนือตัวเลข)
//  │   │      42           │ │  ← card.value()  → gauge.lb_value (ตัวเลขหลัก)
//  │   │   (lb_unit ว่าง)   │ │  ← gauge.lb_unit ไม่ใช้ใน Card นี้
//  │   └───────────────────┘ │
//  └─────────────────────────┘
//
//  NOTE — label mapping ใน Card นี้ต่างจาก GGaugeGRAFANA แบบ standalone:
//  ─────────────────────────────────────────────────────────────
//  card.title()  → lb_title ของ Card    (หัวข้อบนสุด นอก gauge)
//  card.unit()   → gauge.lb_title        (repurpose: แสดงหน่วยเหนือตัวเลข)
//                  เพราะตำแหน่ง lb_title อยู่เหนือ lb_value พอดีกับ unit สั้น
//  gauge.lb_unit → ไม่ใช้ — ปล่อยว่างไว้ (อยู่ใต้ตัวเลข ไม่เหมาะ visual นี้)
//
//  PUBLIC MEMBERS (เข้าถึงลึกกว่า API ด้านบนได้ตรงๆ):
//  ─────────────────────────────────────────────────────────────
//  card.lb_title         → GLabel  (หัวข้อ card บนสุด)
//  card.gauge            → GGaugeGRAFANA (ตัวมิเตอร์เต็มรูป)
//  card.gauge.arc        → GArc    (วงแหวนค่า)
//  card.gauge.lb_value   → GLabel  (ตัวเลขหลัก)
//  card.gauge.lb_title   → GLabel  (repurposed: แสดงหน่วย เหนือตัวเลข)
//  card.gauge.lb_unit    → GLabel  (ไม่ใช้ใน Card นี้)
// ════════════════════════════════════════════════════════════════
class GGaugeGRAFANACard : public GRect {
  public:
    GGaugeGRAFANACard(GPARENT) : GRect(parent) {}
    GLabel lb_title;
    GGaugeGRAFANA gauge;

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      GRect::size(SIZE_CONTENT, SIZE_CONTENT);
      GRect::min_size(160, 160);
      GRect::radius(10);
      GRect::color(TFT_COLOR_HEX(0x1E2230));
      GRect::GRID_CELL(GRID_COL{GRID_CONTENT}, GRID_ROW{GRID_CONTENT, GRID_CONTENT}, GRID_ALIGN_CENTER, GRID_ALIGN_START);
      GRect::padding(10, 15, 15, 15, 10);

      // ── row 0: หัวข้อ card (นอก gauge) ─────────────────────
      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_title.grid_cell(0, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 0, 0);
      lb_title = "Title";

      // ── row 1: gauge ─────────────────────────────────────────
      // gauge.lb_title → repurpose เป็น unit (เหนือตัวเลข) ผ่าน card.unit()
      // gauge.lb_unit  → ปล่อยว่าง ไม่ใช้ใน layout นี้
      gauge.parent(this);
      gauge.size(PCT(100), PCT(100));
      gauge.title("");   // unit จะถูก set ทีหลังผ่าน card.unit() → gauge.title()
      gauge.unit("");    // ไม่ใช้
      gauge.grid_cell(0, 1, GRID_ALIGN_CENTER, GRID_ALIGN_START);

      GRect::onSizeChanged(GWIDGET_CB{
        if(widget->event_old_size() == widget->size()) return;

        GGaugeGRAFANACard* card = (GGaugeGRAFANACard*) widget;

        int32_t g_w = widget->content_width();
        int32_t g_h = widget->content_height();
        // gauge รับ size เต็ม content แล้วบีบตัวเองเป็นสี่เหลี่ยมจัตุรัสใน onSizeChanged ของมันเอง
        card->gauge.size(g_w, g_h);

        // อัปเดตความสูง row 1 ของ GRID_CELL ให้ตรงกับ gauge จริง (ใช้ min เพราะ gauge บีบเป็น square)
        int32_t g_h_for_layout = min(g_w, g_h) * 30 / 100.0f;
        card->GRID_CELL(GRID_COL{GRID_CONTENT}, GRID_ROW{GRID_CONTENT, g_h_for_layout}, GRID_ALIGN_CENTER, GRID_ALIGN_START);
      });
    }

    // ── API ──────────────────────────────────────────────────────
    inline void title(String t)    { create(); lb_title     = t; }  // → card.lb_title
    inline void unit(String u)     { create(); gauge.title(u);   }  // → gauge.lb_title (เหนือตัวเลข)
    inline void decimal(uint8_t d) { create(); gauge.decimal(d); }
    inline uint8_t decimal()       { create(); return gauge.decimal(); }

    inline void  value(float v)     { create(); gauge.value(v); }
    inline void  operator=(float f) { create(); gauge.value(f); }
    inline float toFloat()          { create(); return gauge.toFloat(); }

    inline void range(int32_t mn, int32_t mx) { create(); gauge.range(mn, mx); }

    inline void sections(std::vector<uint32_t> ranges, std::vector<GColor> colors) {
      create();
      gauge.sections(ranges, colors);
    }

    using GObject::align;
    using GObject::parent;
    using GObject::width;
    using GObject::height;
    using GObject::min_size;
    using GObject::max_size;
};

#endif // __CWC_GGAUGE_GRAFANA_CARD_H__