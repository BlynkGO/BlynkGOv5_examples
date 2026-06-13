#ifndef CWC_SIDEBAR_H
#define CWC_SIDEBAR_H

// ════════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน src/ ก่อน build
// ════════════════════════════════════════════════════════════════════
//
// [ICON]
//   สร้าง src/iconlist.txt ใส่เนื้อหาดังนี้:
//   ──────────────────────────────────────────────
//   dashboard      E871
//   thermostat     E846
//   air            EFD8
//   water_drop     E798
//   settings       E8B8
//   ──────────────────────────────────────────────
//   iconconv iconlist.txt --size 28 --name nav_icons
//   → สร้าง nav_icons_28.c และ nav_icons_List.h ใน src/
//
// [FONT]  prasanmit_15 — มีใน BlynkGO library ใช้ได้เลย
// [IMAGE] — ไม่มี
//
// ════════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>

#include "nav_icons_List.h"
FONT_DECLARE(nav_icons_28);
FONT_DECLARE(prasanmit_15);
FONT_DECLARE(prasanmit_30);

// ════════════════════════════════════════════════════════════════════
// CWC_Sidebar.h
// ════════════════════════════════════════════════════════════════════
//
// DESCRIPTION:
//   แถบเมนูนำทางแนวตั้งฝั่งซ้ายมือ มี GButton ธรรมดา สูงสุด 8 ปุ่ม
//   กำหนดรายการปุ่มจากภายนอกผ่าน addItem() ก่อนเรียก create()
//   เรียงแนวตั้งด้วย LAYOUT_COL_M
//
// PUBLIC API:
//   void addItem(const char* symbol, const char* label)
//        — เพิ่มปุ่มเมนู เรียกได้สูงสุด SIDEBAR_MAX_ITEMS ครั้ง
//          *** ต้องเรียกก่อน create() หรือก่อนนำ widget วางบน parent ***
//
//   GButton btn[SIDEBAR_MAX_ITEMS]
//        — เข้าถึงปุ่มโดยตรงเพื่อลงทะเบียน onClicked จาก main.cpp:
//          sidebar.btn[0].onClicked(GWIDGET_CB { ... });
//          หรืออ่าน index ใน callback:
//          int idx = (GButton*)widget - &sidebar.btn[0];
//
// USAGE EXAMPLE:
//
// #include "CWC_Sidebar.h"
//
// Sidebar sidebar;
//
// void setup() {
//   BlynkGO.begin();

//   sidebar.addItem(SYMBOL_DASHBOARD, "Dashboard");
//   sidebar.addItem(SYMBOL_THERMOSTAT, "Temp");
//   sidebar.addItem(SYMBOL_AIR,        "Air");
//   sidebar.addItem(SYMBOL_WATER_DROP, "Water");
//   sidebar.addItem(SYMBOL_SETTINGS,   "Settings");

//   sidebar.align(ALIGN_LEFT);

//   // ลงทะเบียน event จาก main.cpp
//   for (int i = 0; i < sidebar.item_count(); i++) {
//     sidebar.btn[i].onClicked(GWIDGET_CB {
//       int idx = (GButton*)widget - &sidebar.btn[0];
//       Serial.println("Tab: " + String(idx));
//     });
//   }
// }
// ════════════════════════════════════════════════════════════════════

#define SIDEBAR_MAX_ITEMS  8

class Sidebar : public GRect {
  public:

    Sidebar(GPARENT) : GRect(parent) {}

    // ── addItem() : เรียกก่อน create() ──────────────────────────
    void addItem(const char* symbol, const char* label) {
      if (_count >= SIDEBAR_MAX_ITEMS) return;
      _symbols[_count] = symbol;
      _labels[_count]  = label;
      _count++;
    }

    // ── create() ─────────────────────────────────────────────────
    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      GRect::size(SIZE_CONTENT, PCT(100));
      GRect::color(TFT_COLOR_HEX(0x2C3E50));
      GRect::radius(0);
      GRect::shadow(0);
      GRect::layout(LAYOUT_COL_M, 15, 0, 8, 8, 4);

      // ── Logo ──────────────────────────────────────────────────
      lb_logo.parent(this);
      lb_logo.font(prasanmit_30, TFT_COLOR_HEX(0xECF0F1));
      lb_logo = "HOME";
      lb_logo.height(36);

      // ── สร้างปุ่มตามจำนวน item ที่ addItem() ไว้ ──────────────
      for (int i = 0; i < _count; i++) {
        btn[i].parent(this);
        btn[i].width(120);
        btn[i].radius(12);
        btn[i].shadow(0);
        btn[i].border(0);
        btn[i].color(TFT_COLOR_HEX(0x3D5166));

        btn[i].setSymbol(_symbols[i]);
        btn[i].symbol_font(nav_icons_28, TFT_WHITE);

        btn[i] = _labels[i];
        btn[i].font(prasanmit_15, TFT_COLOR_HEX(0xBDC3C7));
      }

      refresh_layout();

      this->onSizeChanged(GWIDGET_CB {
        Sidebar* sb = (Sidebar*) widget;
        sb->_targetW = sb->width();
        sb->_targetH = sb->height();
        sb->refresh_layout();
      });
    }

    void refresh_layout() {
      if (!GRect::isCreated()) return;
    }

  public:
    GLabel  lb_logo;
    GButton btn[SIDEBAR_MAX_ITEMS];
    int     item_count() { return _count; }  // อ่านจำนวนปุ่มที่ add ไว้

  private:
    const char* _symbols[SIDEBAR_MAX_ITEMS] = {};
    const char* _labels [SIDEBAR_MAX_ITEMS] = {};
    int         _count   = 0;
    int32_t     _targetW = 0;
    int32_t     _targetH = 0;
};

#endif // CWC_SIDEBAR_H