#ifndef __CWC_DONUTCARD_H__
#define __CWC_DONUTCARD_H__

#include <BlynkGOv5.h>
#include <vector>
#include "CWC_DonutChart.h"

// ════════════════════════════════════════════════════════════════
//  CWC: DonutCard — DonutChart ที่มีพื้นหลัง card (GRect) ในตัว
//        ตั้งค่า title/value/colors/thick/gap ได้ตรงที่ card เลย
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  DonutCard dcard;
//
//  dcard.size(200, 200);           // ไม่ใส่ก็ได้ — default SIZE_CONTENT
//  dcard.title("ENERGY");
//  dcard.thick(24);
//  dcard.gap(2.0f);
//  dcard.unit("%");
//  dcard.showValues(true);
//  dcard.value({40, 25, 20, 15});
//  dcard.colors({ TFT_RED, TFT_BLUE, TFT_GREEN, TFT_PURPLE });
//
//  PUBLIC MEMBERS (direct access):
//  ─────────────────────────────────────────────────────────────
//  dcard.donut   → DonutChart (เข้าถึง member ลึกกว่า API ด้านบน)
// ════════════════════════════════════════════════════════════════
class DonutCard : public GRect {
  public:
    DonutCard(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      GRect::size(SIZE_CONTENT, SIZE_CONTENT);
      GRect::min_size(120, 120);
      GRect::radius(10);
      GRect::color(TFT_COLOR_HEX(0x1E2230));
      GRect::border(0);
      GRect::padding(10);

      donut.parent(this);
      donut.size(PCT(100), PCT(100));
    }

    // ── Proxy API → donut ─────────────────────────────────────
    inline void title(String t)     { create(); donut.title(t);        }
    inline void unit(String u)      { create(); donut.unit(u);         }
    inline void thick(int32_t t)    { create(); donut.thick(t);        }
    inline void gap(float g)        { create(); donut.gap(g);          }
    inline void showValues(bool en) { create(); donut.showValues(en);  }

    inline void value(std::initializer_list<float> vals) { create(); donut.value(vals); }
    inline void value(const std::vector<float>& vals)    { create(); donut.value(vals); }

    inline void colors(std::initializer_list<color_t> cols) { create(); donut.colors(cols); }
    inline void colors(const std::vector<color_t>& cols)    { create(); donut.colors(cols); }

    DonutChart donut;

    using GObject::align;
    using GObject::parent;
    using GObject::width;
    using GObject::height;
    using GObject::min_size;
    using GObject::max_size;
};

#endif // __CWC_DONUTCARD_H__
