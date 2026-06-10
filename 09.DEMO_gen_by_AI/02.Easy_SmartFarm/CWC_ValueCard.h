#ifndef __CWC_VALUECARD_H__
#define __CWC_VALUECARD_H__

#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
//  CWC: ValueCard — บล็อกการ์ดแสดงหัวข้อและค่าปัจจุบันแบบเรียบง่าย (ไม่มีกราฟ)
// ════════════════════════════════════════════════════════════════
//  QUICK START / วิธีใช้งาน:
//  ─────────────────────────────────────────────────────────────
//  1. ประกาศตัวแปรตัววัตถุ (สไตล์ Lazy Code ไม่ต้องใช้ pointer/new)
//     ValueCard pm_card;
//
//  2. ตั้งค่าในฟังก์ชัน setup()
//     pm_card.parent(rect_panel);  // กำหนด parent คอนเทนเนอร์หลัก
//     pm_card.grid_cell(0, 0);     // จัดวางตำแหน่งในระบบตาราง
//     pm_card.title("PM2.5");
//     pm_card.value("24 ug/m³");
//     pm_card.valueColor(TFT_GREEN); // เปลี่ยนสีข้อความตัวเลข (เลือกใส่หรือไม่ใส่ก็ได้)
//
//  PUBLIC MEMBERS (คอมโพเนนต์ภายในที่สามารถเข้าถึงเพื่อตกแต่งเพิ่มได้):
//  ─────────────────────────────────────────────────────────────
//  pm_card.lb_title  → GLabel ตัวอักษรหัวข้อ (font: prasanmit_25)
//  pm_card.lb_value  → GLabel ตัวอักษรแสดงค่า (font: prasanmit_35)
// ════════════════════════════════════════════════════════════════

#define VALUECARD_COLOR_BG     TFT_COLOR_HEX(0x01466e)
#define VALUECARD_COLOR_VALUE  TFT_COLOR_HEX(0x4DD9FF)

class ValueCard : public GRect {
  public:
    ValueCard(GPARENT) : GRect(parent) {}

    GLabel lb_title;
    GLabel lb_value;

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      
      // กำหนดสไตล์การ์ดพื้นหลังพื้นฐาน
      GRect::height(SIZE_CONTENT);
      GRect::color(VALUECARD_COLOR_BG);
      GRect::bg_opa(210);
      GRect::radius(8);
      GRect::border(0);
      GRect::padding(14, 14, 16, 16, 6);
      GRect::GRID_CELL( GRID_COL { GRID_FR(1)}, GRID_ROW { GRID_CONTENT, GRID_CONTENT });

      // ข้อความหัวข้อด้านบน
      lb_title.parent(this);
      lb_title.font(prasanmit_25, TFT_WHITE);
      lb_title = "Title";
      lb_title.grid_cell(0, 0);

      // ข้อความแสดงค่าด้านล่าง
      lb_value.parent(this);
      lb_value.font(prasanmit_35, VALUECARD_COLOR_VALUE);
      lb_value = "---";
      lb_value.grid_cell(0, 1);
    }

    // ── Public API สำหรับเรียกสั่งการจากภายนอก ──

    // กำหนดชื่อหัวข้อ
    void title(const char* t) {
      create();
      lb_title = t;
    }

    // กำหนดค่าตัวเลขหรือข้อความที่ต้องการแสดงผล
    void value(const char* v) {
      create();
      lb_value = v;
    }

    // กำหนดสีของข้อความแสดงค่าโดยเฉพาะ
    void valueColor(color_t color) {
      create();
      lb_value.color(color);
    }
};

#endif // __CWC_VALUECARD_H__