// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
//
// ขั้นตอน: แปลง .otf → .ttf ก่อน (ใช้ otf2ttf.py)
//   python otf2ttf.py "SF-Pro-Display-Light.otf"
//   python otf2ttf.py "SF-Pro-Display-Regular.otf"
//
// [FONT]  fontconv --font SF-Pro-Display-Light.ttf   --size 80 -num -o CalcNum_80.c
//         fontconv --font SF-Pro-Display-Light.ttf   --size 55 -num -o CalcNum_55.c
//         fontconv --font SF-Pro-Display-Light.ttf   --size 40 -num -o CalcNum_40.c
//         fontconv --font SF-Pro-Display-Regular.ttf --size 32 -eng -o CalcBtn_32.c
//         fontconv --font SF-Pro-Display-Regular.ttf --size 24 -eng -o CalcBtn_24.c
//         fontconv --font SF-Pro-Display-Regular.ttf --size 20 -eng -o CalcBtn_20.c
//
//  ── fallback: ถ้าต้องการทดสอบก่อนแปลง font ──
//     เปลี่ยน CalcNum_xx → BebasNeueBold_num_45
//     เปลี่ยน CalcBtn_xx → prasanmit_30
//     แล้วลบ FONT_DECLARE ทั้งหมดออก
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>

// ── [FONT] ──────────────────────────────────────────────────────
FONT_DECLARE(CalcNum_80);
FONT_DECLARE(CalcNum_55);
FONT_DECLARE(CalcNum_40);
FONT_DECLARE(CalcBtn_32);
FONT_DECLARE(CalcBtn_24);
FONT_DECLARE(CalcBtn_20);
// ────────────────────────────────────────────────────────────────

// ════════════════════════════════════════════════════════════════
//  iPhoneCalc — CWC เครื่องคิดเลข iOS style
//
//  Layout: GRID_CELL-based, portrait เสมอ
//  - คำนวณ btn_size จาก min(sw,sh) → ปุ่มกลม gap เท่ากันทุกทิศ
//  - onSizeChanged → recalc GRID ใหม่อัตโนมัติ (รองรับ screen rotate)
//  - GRect::size(SIZE_CONTENT) → ขนาด CWC ขยายตาม GRID อัตโนมัติ
//
//  ใช้งาน:
//    iPhoneCalc calc;
//    void setup() {
//      BlynkGO.begin();
//      calc.align_center();
//    }
//
//  หรือกำหนดตำแหน่งเองก็ได้:
//    calc.align(ALIGN_CENTER, 0, 0);
// ════════════════════════════════════════════════════════════════

class iPhoneCalc : public GRect {
  public:
    iPhoneCalc(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      GRect::size(PCT(100), PCT(100));
      GRect::min_size(SIZE_CONTENT, 240);
      GRect::color(TFT_COLOR_HEX(0x000000));
      GRect::border(0);
      GRect::radius(0);
      GRect::padding(0);

      // ── สี (คงที่ ไม่ขึ้นกับขนาด) ───────────────────────────
      _C_DARK   = TFT_COLOR_HEX(0x333333);
      _C_GRAY   = TFT_COLOR_HEX(0xA5A5A5);
      _C_ORANGE = TFT_COLOR_HEX(0xFF9500);
      _C_BLACK  = TFT_COLOR_HEX(0x000000);
      _C_WHITE  = TFT_COLOR_HEX(0xFFFFFF);

      // ── Display area ─────────────────────────────────────────
      rect_disp.parent(this);
      rect_disp.color(TFT_COLOR_HEX(0x000000));
      rect_disp.border(0);
      rect_disp.radius(0);
      rect_disp.padding(0);

        lb_result.parent(rect_disp);
        lb_result.long_mode(LABEL_LONG_CLIP);
        lb_result.text_align(TEXT_ALIGN_RIGHT);
        lb_result = "0";

      // ── ปุ่มทั้งหมด (สร้างก่อน ตั้ง callback ─────────────────
      // Row 0: C  +/-  %  ÷
      _setupBtnStyle(btn_clear,   "C",  _C_GRAY,   _C_BLACK);
      _setupBtnStyle(btn_sign,    "+/-",_C_GRAY,   _C_BLACK);
      _setupBtnStyle(btn_percent, "%",  _C_GRAY,   _C_BLACK);
      _setupBtnStyle(btn_div,     "/",  _C_ORANGE, _C_WHITE);
      // Row 1: 7  8  9  ×
      _setupBtnStyle(btn_7,   "7", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_8,   "8", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_9,   "9", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_mul, "x", _C_ORANGE, _C_WHITE);
      // Row 2: 4  5  6  −
      _setupBtnStyle(btn_4,   "4", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_5,   "5", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_6,   "6", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_sub, "-", _C_ORANGE, _C_WHITE);
      // Row 3: 1  2  3  +
      _setupBtnStyle(btn_1,   "1", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_2,   "2", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_3,   "3", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_add, "+", _C_ORANGE, _C_WHITE);
      // Row 4: 0(wide)  .  =
      _setupBtnStyle(btn_0,   "0", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_dot, ".", _C_DARK,   _C_WHITE);
      _setupBtnStyle(btn_eq,  "=", _C_ORANGE, _C_WHITE);

      // ── grid_cell ของปุ่มแต่ละตัว ────────────────────────────
      // GRID_COL: col0=btn, col1=btn, col2=btn, col3=btn
      // GRID_ROW: row0=disp, row1=btn, row2=btn, row3=btn, row4=btn, row5=btn
      // gap มาจาก padding(gap) ไม่ใช่ col/row แยก
      btn_clear.grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);   btn_sign.grid_cell(1, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_percent.grid_cell(2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH); btn_div.grid_cell(3, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      btn_7.grid_cell(0, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_8.grid_cell(1, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_9.grid_cell(2, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_mul.grid_cell(3, 2, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      btn_4.grid_cell(0, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_5.grid_cell(1, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_6.grid_cell(2, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_sub.grid_cell(3, 3, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      btn_1.grid_cell(0, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_2.grid_cell(1, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_3.grid_cell(2, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);  btn_add.grid_cell(3, 4, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      // ปุ่ม 0: colspan=2 → STRETCH เต็ม 2 col → pill shape
      btn_0.grid_cell(0, 5, 2, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_dot.grid_cell(2, 5, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
      btn_eq.grid_cell(3, 5, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      // ── ผูก callbacks ─────────────────────────────────────────
      btn_clear.onClicked(GWIDGET_CB {
        iPhoneCalc* c = (iPhoneCalc*) widget->user_data();
        c->_dispStr = "0";
        c->_opA = 0; c->_opB = 0;
        c->_pendOp = '\0';
        c->_newInput = true; c->_dotUsed = false;
        c->_updateDisplay();
      }, this);

      btn_sign.onClicked(GWIDGET_CB {
        iPhoneCalc* c = (iPhoneCalc*) widget->user_data();
        double v = c->_dispStr.toDouble();
        c->_dispStr = c->_fmtResult(-v);
        c->_dotUsed = c->_dispStr.indexOf('.') >= 0;
        c->_updateDisplay();
      }, this);

      btn_percent.onClicked(GWIDGET_CB {
        iPhoneCalc* c = (iPhoneCalc*) widget->user_data();
        double v = c->_dispStr.toDouble() / 100.0;
        c->_dispStr = c->_fmtResult(v);
        c->_dotUsed = c->_dispStr.indexOf('.') >= 0;
        c->_updateDisplay();
      }, this);

      btn_div.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressOp('/'); }, this);

      btn_7.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('7'); }, this);
      btn_8.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('8'); }, this);
      btn_9.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('9'); }, this);
      btn_mul.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressOp('*'); }, this);

      btn_4.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('4'); }, this);
      btn_5.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('5'); }, this);
      btn_6.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('6'); }, this);
      btn_sub.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressOp('-'); }, this);

      btn_1.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('1'); }, this);
      btn_2.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('2'); }, this);
      btn_3.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDigit('3'); }, this);
      btn_add.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressOp('+'); }, this);

      btn_0.onClicked(GWIDGET_CB   { ((iPhoneCalc*)widget->user_data())->_pressDigit('0'); }, this);
      btn_dot.onClicked(GWIDGET_CB { ((iPhoneCalc*)widget->user_data())->_pressDot(); }, this);
      btn_eq.onClicked(GWIDGET_CB  { ((iPhoneCalc*)widget->user_data())->_pressEqual(); }, this);

      // ── layout ครั้งแรก ───────────────────────────────────────
      refresh_layout();

      GRect::onSizeChanged(GWIDGET_CB{
        auto old_size = widget->event_old_size();
        auto new_size = widget->size();
        if( old_size.y  != new_size.y ){
          iPhoneCalc* calc = (iPhoneCalc*) widget;
          calc->_targetH = new_size.y;
          calc->refresh_layout();
        }
      });
    }

    // ── public: recalc GRID และ font ─────────────────────────────
    // Logic:
    //   1. ใช้ height ของ CWC เป็นหลัก (จาก GRect::height() หรือ GScreen.height())
    //   2. gap = h / 66  (proportional)
    //   3. btn_size = (h - gap*6 - disp_h) / 5   โดย disp_h = btn_size*2 → แก้สมการ:
    //      h = btn_size*7 + gap*6  →  btn_size = (h - gap*6) / 7
    //   4. GRID_COL{ btn_size×4 }  + padding(gap) → gap ออกมาจาก padding
    //      btn cell = btn_size×btn_size พอดี → radius(btn_size/2) → กลม
    //   5. width = SIZE_CONTENT เสมอ (ขยายตาม 4 col + padding)
    void refresh_layout() {
      if (!GRect::isCreated())  return;

      // ── ใช้ _targetH ที่เก็บไว้ ถ้าไม่มีใช้ GScreen.height() ──
      int32_t h = (_targetH > 0) ? _targetH : GScreen.height();

      // ── gap proportional ─────────────────────────────────────────
      int32_t gap = max(4, h / 66);

      // ── btn_size: แก้จาก h = btn_size*7 + gap*6 ─────────────────
      // (display area = btn_size*2, ปุ่ม 5 แถว, gap 6 ช่อง)
      int32_t btn_size = (h - gap * 6) / 7;
      if (btn_size < 20) btn_size = 20;  // ขั้นต่ำ

      // ── disp_h: ส่วนที่เหลือหลังหัก 5 แถวปุ่ม + 7 gap ────────────
      int32_t disp_h = h - (btn_size * 5 + gap * 7);
      if (disp_h < 20) disp_h = 20;

      // Serial.printf("[iPhoneCalc] gap=%d  btn_size=%d  disp_h=%d\n",
      //               gap, btn_size, disp_h);

      // ── GRID: 4 col เท่ากัน ─────────────────────────────────────
      // outer padding = gap (ขอบนอก CWC)
      // pad_row / pad_column = gap (ระหว่าง cell) → ไม่กิน cell size
      // btn cell = btn_size×btn_size พอดี → radius(btn_size/2) → กลม
      // GRect::size(SIZE_CONTENT, SIZE_CONTENT);
      // GRect::size(SIZE_CONTENT, h);
      GRect::padding(gap);
      GRect::padding_bottom(2*gap);
      GRect::GRID_CELL(
        GRID_COL{ btn_size, btn_size, btn_size, btn_size },
        GRID_ROW{ disp_h, btn_size, btn_size, btn_size, btn_size, btn_size }
      , GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

      // ── rect_disp span เต็ม 4 col ─────────────────────────────────
      rect_disp.grid_cell(0, 0, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

      // ── radius ปุ่ม = btn_size/2 → กลมพอดี ───────────────────────
      int32_t r = btn_size / 2;
      btn_clear.radius(r);   btn_sign.radius(r);
      btn_percent.radius(r); btn_div.radius(r);
      btn_7.radius(r);  btn_8.radius(r);  btn_9.radius(r);  btn_mul.radius(r);
      btn_4.radius(r);  btn_5.radius(r);  btn_6.radius(r);  btn_sub.radius(r);
      btn_1.radius(r);  btn_2.radius(r);  btn_3.radius(r);  btn_add.radius(r);
      btn_dot.radius(r); btn_eq.radius(r);
      btn_0.radius(r);  // pill: STRETCH กว้าง 2 col → radius ครึ่งสูง = กลมพอดี

      // ── เลือก font ตาม btn_size ───────────────────────────────────
      const font_t* f_num_large;
      const font_t* f_num_mid;
      const font_t* f_num_small;
      const font_t* f_btn;

      if (btn_size >= 100) {
        f_num_large = &CalcNum_80; f_num_mid = &CalcNum_55;
        f_num_small = &CalcNum_40; f_btn     = &CalcBtn_32;
      } else if (btn_size >= 70) {
        f_num_large = &CalcNum_55; f_num_mid = &CalcNum_40;
        f_num_small = &CalcNum_40; f_btn     = &CalcBtn_24;
      } else {
        f_num_large = &CalcNum_40; f_num_mid = &CalcNum_40;
        f_num_small = &CalcNum_40; f_btn     = &CalcBtn_20;
      }

      _f_large = f_num_large;
      _f_mid   = f_num_mid;
      _f_small = f_num_small;
      _gap     = gap;

      // ── font ปุ่ม ─────────────────────────────────────────────────
      btn_clear.font(*f_btn, _C_BLACK);   btn_sign.font(*f_btn,    _C_BLACK);
      btn_percent.font(*f_btn, _C_BLACK); btn_div.font(*f_btn,     _C_WHITE);
      btn_7.font(*f_btn, _C_WHITE);  btn_8.font(*f_btn, _C_WHITE);
      btn_9.font(*f_btn, _C_WHITE);  btn_mul.font(*f_btn, _C_WHITE);
      btn_4.font(*f_btn, _C_WHITE);  btn_5.font(*f_btn, _C_WHITE);
      btn_6.font(*f_btn, _C_WHITE);  btn_sub.font(*f_btn, _C_WHITE);
      btn_1.font(*f_btn, _C_WHITE);  btn_2.font(*f_btn, _C_WHITE);
      btn_3.font(*f_btn, _C_WHITE);  btn_add.font(*f_btn, _C_WHITE);
      btn_0.font(*f_btn, _C_WHITE);  btn_dot.font(*f_btn, _C_WHITE);
      btn_eq.font(*f_btn, _C_WHITE);

      // ── font display ──────────────────────────────────────────────
      _updateDisplay();
    }

    using GRect::operator=;

  private:
    // ── state ────────────────────────────────────────────────────
    double  _opA      = 0;
    double  _opB      = 0;
    char    _pendOp   = '\0';
    bool    _newInput = true;
    bool    _dotUsed  = false;
    String  _dispStr  = "0";
    int32_t _gap      = 12;
    int32_t _targetH  = 0;   // h ที่ user set ไว้ (0 = ใช้ GScreen.height())

    const font_t* _f_large = nullptr;
    const font_t* _f_mid   = nullptr;
    const font_t* _f_small = nullptr;

    // ── สี ───────────────────────────────────────────────────────
    color_t _C_DARK;
    color_t _C_GRAY;
    color_t _C_ORANGE;
    color_t _C_BLACK;
    color_t _C_WHITE;

    // ── child widgets ─────────────────────────────────────────────
    GRect   rect_disp;
    GLabel  lb_result;
    GButton btn_clear, btn_sign, btn_percent, btn_div;
    GButton btn_7, btn_8, btn_9, btn_mul;
    GButton btn_4, btn_5, btn_6, btn_sub;
    GButton btn_1, btn_2, btn_3, btn_add;
    GButton btn_0, btn_dot, btn_eq;

    // ── helper: style ปุ่ม (สร้าง parent + สี) ───────────────────
    void _setupBtnStyle(GButton& b, const char* label, color_t bg, color_t fg) {
      b.parent(this);
      b.color(bg);
      b.border(0);
      b = label;
      // font และ radius จะถูกตั้งใน refresh_layout()
    }

    // ── helper: format ────────────────────────────────────────────
    String _fmtResult(double v) {
      if (v == (long long)v) return String((long long)v);
      String s = String(v, 8);
      while (s.endsWith("0")) s = s.substring(0, s.length() - 1);
      if (s.endsWith(".")) s = s.substring(0, s.length() - 1);
      return s;
    }

    double _calcOp(double a, double b, char op) {
      switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
      }
      return b;
    }

    void _updateDisplay() {
      if (!_f_large || !_f_mid || !_f_small) return;
      int32_t len = _dispStr.length();
      if      (len <= 7)  { lb_result.font(*_f_large, _C_WHITE); }
      else if (len <= 10) { lb_result.font(*_f_mid,   _C_WHITE); }
      else                { lb_result.font(*_f_small, _C_WHITE); }
      lb_result.align(ALIGN_BOTTOM_RIGHT, -_gap, -_gap);
      lb_result = _dispStr;
    }

    void _pressDigit(char d) {
      if (_newInput) {
        _dispStr = String(d); _newInput = false; _dotUsed = false;
      } else {
        if (_dispStr == "0") _dispStr = String(d);
        else if (_dispStr.length() < 12) _dispStr += d;
      }
      _updateDisplay();
    }

    void _pressDot() {
      if (_newInput) { _dispStr = "0."; _newInput = false; _dotUsed = true; }
      else if (!_dotUsed) { _dispStr += "."; _dotUsed = true; }
      _updateDisplay();
    }

    void _pressOp(char op) {
      _opA = _dispStr.toDouble();
      if (_pendOp != '\0' && !_newInput) {
        _opA = _calcOp(_opA, _dispStr.toDouble(), _pendOp);
        _dispStr = _fmtResult(_opA);
        _updateDisplay();
      }
      _pendOp = op;
      _newInput = true;
    }

    void _pressEqual() {
      if (_pendOp == '\0') return;
      _opB = _dispStr.toDouble();
      double result = _calcOp(_opA, _opB, _pendOp);
      _dispStr = _fmtResult(result);
      _pendOp  = '\0';
      _newInput = true;
      _dotUsed  = _dispStr.indexOf('.') >= 0;
      _updateDisplay();
    }
};

// ════════════════════════════════════════════════════════════════
//  GLOBAL
// ════════════════════════════════════════════════════════════════
iPhoneCalc calc;

void setup() {
  BlynkGO.begin();
  BlynkGO.fillScreen("#ffbbbb");
  calc.align_center();
}

void loop() {
  BlynkGO.update();
}