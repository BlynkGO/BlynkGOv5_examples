#ifndef __CWC_WATT_POWER_METER_H__
#define __CWC_WATT_POWER_METER_H__

// ════════════════════════════════════════════════════════════════════
// ASSET PREPARATION — ต้องเตรียมรูปก่อน build
// ════════════════════════════════════════════════════════════════════
//
//   ต้องมีรูป watt-hour meter (เช่น มิเตอร์ไฟฟ้าแบบกลม/สี่เหลี่ยม)
//   ชื่อไฟล์: watt_hour_meter.png
//
//   วางไว้ใน src/ แล้วรันคำสั่งนี้ก่อน build:
//
//     imgconv watt_hour_meter.png
//
//   จะได้ไฟล์ img_watt_hour_meter.c ออกมาใน src/ อัตโนมัติ
//   (ถ้ารูปมีพื้นโปร่งใส ให้ใช้ imgconv watt_hour_meter.png -t แทน)
//
//   ⚠️ ถ้ายังไม่มีรูป หรือยังไม่ได้รัน imgconv — โค้ดนี้จะ build ไม่ผ่าน
//      เพราะ IMAGE_DECLARE(img_watt_hour_meter) หาไฟล์ img_watt_hour_meter.c ไม่เจอ
//
// ════════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>

IMAGE_DECLARE(img_watt_hour_meter);

// ════════════════════════════════════════════════════════════════
//  CWC: GWattHourMeter — power meter 7-digit display
//
//  QUICK START:
//  ─────────────────────────────────────────────────────────────
//  GWattHourMeter watt_meter;
//
//  watt_meter.align(ALIGN_CENTER);
//  watt_meter.font(prasanmit_20, TFT_WHITE);   // เปลี่ยน font ได้ (optional)
//  watt_meter = 1234.5f;
//  float v = watt_meter.toFloat();
// ════════════════════════════════════════════════════════════════
class GWattHourMeter : public GImage {
  public:
    GWattHourMeter(GPARENT) : GImage(parent) {}

    void create() {
      if (GImage::isCreated()) return;
      GImage::create();
      GImage::setImage(img_watt_hour_meter);

      GImage::font(prasanmit_25, TFT_WHITE, GPART_MAIN);

      GImage::onDrawMain(GWIDGET_CB {
        GWattHourMeter* _p = (GWattHourMeter*) widget;
        _p->_draw_display();
      });
    }

    inline void value(float v)         { create(); _value = v; this->invalidate(); }
    inline void decimal_font(const font_t& f) { create(); _dec_font = &f; this->invalidate(); }
    inline void  operator=(float f)    { value(f); }
    inline float toFloat()             { return _value; }

    using GImage::font;

  private:
    float        _value    = 0;
    const font_t* _dec_font = &prasanmit_20;   // default เล็กกว่า integer

    void _draw_display() {
      area_t bg = this->softdrawRect(164, 56, TFT_BLACK, 8, 0, TFT_BLACK,
                                      255, ALIGN_TOP, 0, 95);

      const int total_digits = 7;
      const int int_digits   = 6;
      int32_t bg_w = 164;
      int32_t bg_h = 56;
      int32_t cell_w = bg_w / total_digits;

      float v = _value;
      if (v > 999999.9f)  v = 999999.9f;
      if (v < -99999.9f)  v = -99999.9f;
      char buf[16];
      snprintf(buf, sizeof(buf), "%07.1f", v);

      String s(buf);
      int dot_pos = s.indexOf('.');
      String digits = s.substring(0, dot_pos) + s.substring(dot_pos + 1);
      if (digits.length() > total_digits) digits = digits.substring(digits.length() - total_digits);
      while (digits.length() < total_digits) digits = "0" + digits;

      const font_t* int_fnt = this->text_font(GPART_MAIN);   // prasanmit_25
      color_t       txt_col = this->text_color(GPART_MAIN);

      int32_t bg_left_x = -bg_w / 2;
      int32_t cell_cy    = 95 + bg_h / 2;   // กึ่งกลางแนวตั้งของ bg (offset จาก widget center)

      for (int i = 0; i < total_digits; i++) {
        bool is_decimal = (i == int_digits);

        int32_t cell_cx = bg_left_x + i * cell_w + cell_w / 2;

        if (i > 0 && !is_decimal) {
          this->softdrawRect(1, bg_h - 4, TFT_COLOR_HEX(0x444444),
                              ALIGN_TOP, bg_left_x + i * cell_w, 97);
        }

        if (is_decimal) {
          this->softdrawRect(cell_w - 2, bg_h - 4, TFT_WHITE, 4,
                              ALIGN_TOP, cell_cx - (cell_w - 2) / 2 + 10, 97);
        }

        // ── จุดทศนิยม — วงกลมเล็กมุมล่างขวาของช่อง int_digits-1 ──
        if (i == int_digits - 1) {
          int32_t dot_x = bg_left_x + (i + 1) * cell_w - 2;
          int32_t dot_y = 97 + bg_h - 23; //- 10;
          this->softdrawRect(4, 4, txt_col, 2, ALIGN_TOP, dot_x, dot_y);
        }

        char digit_str[2] = { digits[i], '\0' };
        const font_t* fnt = is_decimal ? _dec_font : int_fnt;

        int32_t fnt_h   = fnt->line_height;
        int32_t label_y = cell_cy - (fnt_h / 2);

        this->softdrawLabel(String(digit_str), *fnt,
                            is_decimal ? TFT_BLACK : txt_col,
                            ALIGN_TOP, cell_cx, label_y);
      }
    }
};

#endif // __CWC_WATT_POWER_METER_H__