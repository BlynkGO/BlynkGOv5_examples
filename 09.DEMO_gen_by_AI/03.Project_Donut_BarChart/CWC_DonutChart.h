#ifndef __CWC_DONUTCHART_H__
#define __CWC_DONUTCHART_H__

#include <BlynkGOv5.h>
#include <vector>

// ════════════════════════════════════════════════════════════════
//   CWC: DonutChart — วาดกราฟวงกลม/โดนัทแบ่ง Segment ด้วย SoftArc
//
//   QUICK START:
//   ─────────────────────────────────────────────────────────────
//   DonutChart donut;
//
//   donut.size(300, 300);
//   donut.title("FARM");
//   donut.thick(35);
//   donut.gap(2.0f);
//   donut.value({10, 30, 50, 80}); // ใส่ค่ากี่ Segment ก็ได้
//
//   PUBLIC METHODS:
//   ─────────────────────────────────────────────────────────────
//   donut.value({v1, v2, ...})   → กำหนดชุดข้อมูลแสดงผล (Array/Vector)
//   donut.colors({c1, c2, ...})  → กำหนดสีประจำแต่ละ Segment (Optional)
//   donut.thick(thickness)       → กำหนดความหนาของวงแหวนโดนัท
//   donut.gap(degree)            → กำหนดช่องว่างระหว่างข้อต่อวงแหวน (องศา)
//   donut.title("Text")          → ข้อความแสดงกึ่งกลางวง (Content Center)
//   donut.showValues(true/false) → เปิด/ปิด ตัวเลขระบุค่าบนวงแหวน
//   donut.unit("%")              → กำหนดหน่วยของขุดข้อมูลแสดงผล
// ════════════════════════════════════════════════════════════════
class DonutChart : public GRect {
  public:

    DonutChart(GPARENT) : GRect(parent) {}

    // ── State ──────────────────────────────────────────────────
    std::vector<float>   _vals;
    std::vector<color_t> _colors;
    float    _gap           = 3.0f;
    float    _angle_offset  = 270.0f;
    float    _angle_total   = 360.0f;
    String   _title         = "";
    String   _unit          = "";
    bool     _show_values   = true;

    // ── Default palette ────────────────────────────────────────
    static const color_t PALETTE[];
    static const uint8_t PALETTE_SIZE;

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      GRect::size(200,200);
      GRect::min_size(80,80);

      GRect::bg_opa(0);
      GRect::border(0);
      GRect::overflow_visible(true);
      GRect::clickable(false);
      GRect::font(prasanmit_20, TFT_BLACK, GPART_INDICATOR);
      GRect::text_color(TFT_WHITE, GPART_MAIN);
      
      this->thick(35);
      this->gap(1.0f);

      if (_vals.empty()) {
        _vals = {25, 25, 25, 25};
      }

      GRect::onDrawMain(GWIDGET_CB {
        DonutChart* _p = (DonutChart*) widget;

        // 1. ดึงพิกัดขอบเขตสัมบูรณ์นอกสุดและระยะ Padding ของชิ้นงาน
        auto bg_area = widget->coords();
        int32_t pad_t = widget->padding_top();
        int32_t pad_b = widget->padding_bottom();
        int32_t pad_l = widget->padding_left();
        int32_t pad_r = widget->padding_right();

        // 2. คำนวณหาตำแหน่งขอบเขตของเนื้อที่ด้านในจริง (Content Box)
        int32_t content_x1 = bg_area->x1 + pad_l;
        int32_t content_y1 = bg_area->y1 + pad_t;
        int32_t content_x2 = bg_area->x2 - pad_r;
        int32_t content_y2 = bg_area->y2 - pad_b;

        int32_t c_width  = content_x2 - content_x1 + 1;
        int32_t c_height = content_y2 - content_y1 + 1;

        // 3. กำหนดจุดศูนย์กลางร่วมภายในพื้นที่ Content จริง (สำหรับ softdrawArc)
        point_t c;
        c.x = content_x1 + (c_width / 2);
        c.y = content_y1 + (c_height / 2);

        // รัศมีคิดจากความกว้าง/สูงของพื้นที่ Content จริง
        int32_t r = min(c_width, c_height) / 2 - 2;

        // 4. หาจุดศูนย์กลางของกล่องสี่เหลี่ยมด้านนอก (Widget Absolute Center)
        int32_t widget_cx = bg_area->x1 + (bg_area->x2 - bg_area->x1 + 1) / 2;
        int32_t widget_cy = bg_area->y1 + (bg_area->y2 - bg_area->y1 + 1) / 2;

        // 5. คำนวณค่าความต่างพิกัด (Shift Offset) ระหว่างศูนย์กลางนอกและศูนย์กลางใน
        int32_t shift_cx = c.x - widget_cx;
        int32_t shift_cy = c.y - widget_cy;

        // ── track พื้นหลัง (GPART_MAIN) ───────────────────────
        {
          draw_arc_dsc_t bg_dsc;
          widget->init_draw_arc_dsc(GPART_MAIN, bg_dsc);
          bg_dsc.center      = c;
          bg_dsc.radius      = r;
          bg_dsc.rounded     = false;
          bg_dsc.start_angle = (int32_t)_p->_angle_offset;
          bg_dsc.end_angle   = (int32_t)(_p->_angle_offset + _p->_angle_total);
          widget->softdrawArc(bg_dsc);
        }

        // ── normalize ─────────────────────────────────────────
        uint8_t n = _p->_vals.size();
        if (n == 0) return;

        float total = 0;
        for (auto v : _p->_vals) total += max(v, 0.0f);
        if (total <= 0) return;

        float gap_total = _p->_gap * n;
        float arc_space = _p->_angle_total - gap_total;
        if (arc_space <= 0) return;

        // ── arc_dsc สำหรับ segment (GPART_INDICATOR) ──────────
        draw_arc_dsc_t arc_dsc;
        widget->init_draw_arc_dsc(GPART_INDICATOR, arc_dsc);
        arc_dsc.center  = c;
        arc_dsc.radius  = r;
        arc_dsc.rounded = false;

        // ── label_dsc สำหรับตัวเลขบน arc (GPART_INDICATOR) ────
        draw_label_dsc_t lbl_dsc;
        widget->init_draw_label_dsc(GPART_INDICATOR, lbl_dsc);

        // หารัศมีตรงกลางความหนาเส้นโค้ง
        float r_mid = (r > arc_dsc.width) ? r - (arc_dsc.width / 2.0f) : r / 2.0f;

        float cursor = _p->_angle_offset;

        for (uint8_t i = 0; i < n; i++) {
          float seg_deg = (max(_p->_vals[i], 0.0f) / total) * arc_space;

          if (seg_deg < 0.5f) {
            cursor += seg_deg + _p->_gap;
            continue;
          }

          color_t col = (i < _p->_colors.size())
                        ? _p->_colors[i]
                        : _p->PALETTE[i % _p->PALETTE_SIZE];

          // วาดแผ่นวงโค้งสอดรับพิกัดศูนย์กลางเนื้อใน
          arc_dsc.color       = TFT_COLOR(col);
          arc_dsc.start_angle = (int32_t)(cursor);
          arc_dsc.end_angle   = (int32_t)(cursor + seg_deg);
          widget->softdrawArc(arc_dsc);

          // ── วาดตัวเลขกลาง arc segment ─────────────────────────
          if (_p->_show_values) {
            float mid_deg = cursor + seg_deg / 2.0f;
            float mid_rad = mid_deg * DEG_TO_RAD;

            // 1. คำนวณตำแหน่งจุดรอบวงกลมสัมพัทธ์จากจุดศูนย์กลางแท้จริง (c.x, c.y)
            int32_t local_x = (int32_t)(r_mid * cosf(mid_rad));
            int32_t local_y = (int32_t)(r_mid * sinf(mid_rad));

            // 2. แปลงผลลัพธ์ให้อิงจุดศูนย์กลางหลักของตัว Widget โดยบวกค่า Shift Offset เข้าไป
            int32_t label_ofs_x = local_x + shift_cx;
            int32_t label_ofs_y = local_y + shift_cy;

            static String val_txt;
            val_txt = String((int)_p->_vals[i])+_p->_unit;
            
            widget->softdrawLabel(val_txt, *lbl_dsc.font,
                                  COLOR16(lbl_dsc.color), ALIGN_CENTER,
                                  label_ofs_x, label_ofs_y);
          }

          cursor += seg_deg + _p->_gap;
        }

        // ── title กลางวง (ขยับเข้าจุดศูนย์กลางพื้นที่ Content) ──
        if (_p->_title.length() > 0) {
          draw_label_dsc_t title_dsc;
          widget->init_draw_label_dsc(GPART_MAIN, title_dsc);

          static String title_txt;
          title_txt = _p->_title;
          
          // ส่งค่าเบี่ยงเบนแกนชดเชยเพื่อให้คำว่า FARM ขยับตามจุดศูนย์กลางที่บีบลงมาจาก Padding
          widget->softdrawLabel(title_txt, *widget->text_font(GPART_MAIN),
                                widget->text_color(GPART_MAIN), ALIGN_CENTER, shift_cx, shift_cy);
        }
      });
    }

    // ── Public API ─────────────────────────────────────────────
    void value(std::initializer_list<float> vals) {
      create(); _vals.assign(vals.begin(), vals.end()); this->invalidate();
    }
    void value(const std::vector<float>& vals) {
      create(); _vals = vals; this->invalidate();
    }
    void colors(std::initializer_list<color_t> cols) {
      create(); _colors.assign(cols.begin(), cols.end()); this->invalidate();
    }

    void colors(const std::vector<color_t>& cols) {
      create(); 
      _colors = cols;       // คัดลอกค่าจาก vector ตัวนอกเข้ามาเก็บใน _colors ได้โดยตรง
      this->invalidate();
    }

    void thick(int32_t t)        { create(); GObject::arc_width(t, GPART_INDICATOR);  }
    void gap(float g)            { create(); _gap         = g;  this->invalidate();   }
    void spacing(float g)        { create(); _gap         = g;  this->invalidate();   }
    void title(String t)         { create(); _title       = t;  this->invalidate();   }
    void showValues(bool en)     { create(); _show_values = en; this->invalidate();   }
    void unit(String unit)       { create(); _unit        = unit; this->invalidate(); }
};

const color_t DonutChart::PALETTE[] = {
  TFT_COLOR_HEX(0x4FC3F7), TFT_COLOR_HEX(0x81C784), TFT_COLOR_HEX(0xFFB74D),
  TFT_COLOR_HEX(0xE57373), TFT_COLOR_HEX(0xBA68C8), TFT_COLOR_HEX(0x4DB6AC),
  TFT_COLOR_HEX(0xF06292), TFT_COLOR_HEX(0xFFF176)
};
const uint8_t DonutChart::PALETTE_SIZE = 8;

#endif // __CWC_DONUTCHART_H__