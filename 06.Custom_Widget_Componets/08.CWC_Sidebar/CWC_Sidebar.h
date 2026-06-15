#ifndef CWC_SIDEBAR_H
#define CWC_SIDEBAR_H

// ════════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน src/ ก่อน build
// ════════════════════════════════════════════════════════════════════
//
// [ICON]
//   สร้าง src/iconlist.txt ใส่เนื้อหาดังนี้:
//   ──────────────────────────────────────────────
//   menu           E5D2
//   dashboard      E871
//   thermostat     E846
//   air            EFD8
//   water_drop     E798
//   settings       E8B8
//   ──────────────────────────────────────────────
//   iconconv iconlist.txt --size 28 --name nav_icons
//   → สร้าง nav_icons_28.c และ nav_icons_List.h ใน src/
//
// [FONT]  prasanmit_15, prasanmit_30 — มีใน BlynkGO library ใช้ได้เลย
// [IMAGE] — ไม่มี
//
// ════════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include <vector>

#include "nav_icons_List.h"
FONT_DECLARE(nav_icons_28);
FONT_DECLARE(prasanmit_15);
FONT_DECLARE(prasanmit_30);

// ════════════════════════════════════════════════════════════════════
// CWC_Sidebar.h
// ════════════════════════════════════════════════════════════════════
//
// DESCRIPTION:
//   แถบเมนูนำทางแนวตั้งฝั่งซ้ายมือ มี GButton dynamic ไม่จำกัดจำนวน
//
//   Web-style hamburger behavior:
//   - Expanded : btn_hamburger อยู่บน Sidebar (ignore_layout)
//                กดแล้ว Sidebar slide ซ่อนไปซ้าย
//   - Collapsed: btn_hamburger ย้าย parent → GScreen + top(true)
//                ลอยบนสุด กดได้เสมอแม้ Sidebar หายไป
//                กดแล้ว Sidebar slide กลับออกมา
//
//   Auto-collapse:
//   - หลัง expand() → slide ออก 300ms แล้วรอ _auto_ms → collapse()
//   - กดเมนูใดๆ → collapse() ทันที ไม่รอ
//   - กด hamburger → collapse() ทันที ไม่รอ
//   - _auto_ms = 0 → disable auto-collapse
//
// PUBLIC API:
//   void addItem(const char* symbol, const char* label)
//   void    title(String t)
//   void    auto_collapse_time(uint32_t ms)   // 0 = disable
//   int32_t selected_id()
//   String  selected_text()
//   int     item_count()
//   bool    is_expanded()
//   void    toggle() / expand() / collapse()
//   GButton btn_hamburger
//   std::vector<GButton*> btn
//
// USAGE EXAMPLE:
//
//   #include "CWC_Sidebar.h"
//
//   Sidebar sidebar;
//
//   void setup() {
//     BlynkGO.begin();
//
//     sidebar.auto_collapse_time(3000);   // optional, default = 3000ms
//
//     sidebar.addItem(SYMBOL_DASHBOARD,  "Dashboard");
//     sidebar.addItem(SYMBOL_THERMOSTAT, "Temp");
//     sidebar.addItem(SYMBOL_AIR,        "Air");
//     sidebar.addItem(SYMBOL_WATER_DROP, "Water");
//     sidebar.addItem(SYMBOL_SETTINGS,   "Settings");
//
//     sidebar.onValueChanged(GWIDGET_CB {
//       Serial.println("Tab: " + String(sidebar.selected_id())
//                      + " = " + sidebar.selected_text());
//     });
//   }
// ════════════════════════════════════════════════════════════════════

class Sidebar : public GRect {
  public:

    Sidebar(GPARENT) : GRect(parent) {}

    ~Sidebar() {
      for (GButton* b : btn) delete b;
    }

    // ── create() ─────────────────────────────────────────────────
    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      GRect::size(140, PCT(100));
      GRect::color(TFT_COLOR_HEX(0x2C3E50));
      GRect::radius(0);
      GRect::shadow(0);
      GRect::layout(LAYOUT_COL_M, 8, 0, 8, 8, 4);
      GRect::align(ALIGN_LEFT);
      GRect::scrollbar(SCROLLBAR_AUTO, false, true);
      GRect::top(true);

      // ── Logo ────────────────────────────────────────────────────
      lb_logo.parent(this);
      lb_logo.font(prasanmit_30, TFT_COLOR_HEX(0xECF0F1));
      lb_logo = "HOME";
      lb_logo.height(36);
      lb_logo.translate_x(-10);

      // ── Hamburger button ────────────────────────────────────────
      btn_hamburger.parent(this);
      btn_hamburger.ignore_layout(true);
      btn_hamburger.radius(8);
      btn_hamburger.shadow(0);
      btn_hamburger.border(0);
      btn_hamburger.color(TFT_COLOR_HEX(0x3D5166));
      btn_hamburger.setSymbol(SYMBOL_MENU);
      btn_hamburger.symbol_font(nav_icons_28, TFT_WHITE);
      btn_hamburger.text("");
      btn_hamburger.padding(5,5,5,5,0);
      btn_hamburger.bg_opa(0);
      btn_hamburger.align(ALIGN_TOP_RIGHT, GRect::padding_right()-5, -GRect::padding_top()+5);
      btn_hamburger.floating(true);

      btn_hamburger.onClicked(GWIDGET_CB {
        Sidebar* _sb = (Sidebar*) widget->user_data();
        _sb->toggle();
      }, this);
    }

    // ── addItem() ────────────────────────────────────────────────
    void addItem(const char* symbol, const char* label) {
      create();

      GButton* b = new GButton(this);
      b->create();
      b->width(PCT(100));
      b->radius(12);
      b->shadow(0);
      b->border(0);
      b->color(TFT_COLOR_HEX(0x3D5166));

      b->setSymbol(symbol);
      b->symbol_font(nav_icons_28, TFT_WHITE);

      b->text(label);
      b->font(prasanmit_15, TFT_COLOR_HEX(0xBDC3C7));

      b->onClicked(GWIDGET_CB {
        Sidebar* _sb = (Sidebar*) widget->parent();

        for (int j = 0; j < (int)_sb->btn.size(); j++) {
          if (_sb->btn[j] == (GButton*)widget) {
            _sb->_selected_id = j;
            break;
          }
        }

        _sb->collapse(0);
        _sb->event_send(EVENT_VALUE_CHANGED);
      });

      btn.push_back(b);
    }

    // ── toggle / expand / collapse ────────────────────────────────
    void toggle(uint32_t delay_ms = 0) {
      create();
      if (_expanded) collapse(delay_ms);
      else           expand(delay_ms);
    }

    void expand(uint32_t delay_ms = 0) {
      create();
      _expanded = true;
      this->top(true);
      this->toForeground();

      btn_hamburger.parent(this);
      btn_hamburger.top(true);
      btn_hamburger.toForeground();
      btn_hamburger.ignore_layout(true);
      btn_hamburger.align(ALIGN_TOP_RIGHT, GRect::padding_right()-5, -GRect::padding_top()+5);

      // anim 1: slide ออก 300ms
      _anim.del();
      _anim.init(*this, ANIM_X, GRect::posX(), 0, 300, ANIM_PATH_EASE_OUT);
      _anim.delay(delay_ms);
      _anim.completed_cb([](GAnimation* a) {
        Sidebar* _sb = (Sidebar*) a->user_data();
        if (!_sb->_expanded) return;
        if (_sb->_auto_ms == 0) return;   // disable

        // anim 2: รอ _auto_ms แล้ว collapse (สร้างใหม่หลัง slide เสร็จ)
        _sb->_anim.del();
        _sb->_anim.init(*_sb, ANIM_X, 0, 0, 1, ANIM_PATH_LINEAR);  // อยู่นิ่ง
        _sb->_anim.delay(_sb->_auto_ms);
        _sb->_anim.completed_cb([](GAnimation* a2) {
          Sidebar* _sb = (Sidebar*) a2->user_data();
          if (_sb->_expanded) _sb->collapse(0);
        });
        _sb->_anim.user_data(_sb);
        _sb->_anim.start();
      });
      _anim.user_data(this);
      _anim.start();
    }

    void collapse(uint32_t delay_ms = 0) {
      create();
      _expanded = false;
      this->top(true);
      this->toForeground();
      
      _anim.del();   // ยกเลิก countdown ที่ค้างอยู่ (ถ้ามี)

      // slide ซ่อนไปซ้าย
      _anim.init(*this, ANIM_X, GRect::posX(), -GRect::width(), 300, ANIM_PATH_EASE_OUT);
      _anim.delay(delay_ms);
      _anim.completed_cb([](GAnimation* a) {
        Sidebar* _sb = (Sidebar*) a->user_data();
        GWidget* _parent = _sb->parent();
        _sb->btn_hamburger.parent(_parent);
        _sb->btn_hamburger.top(true);
        _sb->btn_hamburger.toForeground();
        _sb->btn_hamburger.ignore_layout(true);
        _sb->btn_hamburger.align(ALIGN_TOP_LEFT, -_parent->pad_left()+5, -_parent->pad_top());
      });
      _anim.user_data(this);
      _anim.start();
    }

    // ── setters ──────────────────────────────────────────────────
    inline void title(String t)                  { create(); lb_logo = t; }
    inline void auto_collapse_time(uint32_t ms)  { _auto_ms = ms; }

    // ── getters ──────────────────────────────────────────────────
    inline int32_t selected_id()   { create(); return _selected_id; }
    inline String  selected_text() {
      create();
      if (_selected_id < 0 || _selected_id >= (int)btn.size()) return "";
      return btn[_selected_id]->text();
    }
    inline int  item_count()  { create(); return (int)btn.size(); }
    inline bool is_expanded() { return _expanded; }

  public:
    GButton               btn_hamburger;
    GLabel                lb_logo;
    std::vector<GButton*> btn;

  private:
    int        _selected_id = -1;
    bool       _expanded    = true;
    uint32_t   _auto_ms     = 3000;   // auto-collapse delay (ms), 0 = disable
    GAnimation _anim;
};

#endif // CWC_SIDEBAR_H