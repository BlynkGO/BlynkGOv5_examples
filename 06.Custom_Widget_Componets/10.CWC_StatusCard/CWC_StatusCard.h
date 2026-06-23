#pragma once
#include <BlynkGOv5.h>

// ── Font forward declaration (ต้องรัน iconconv ก่อน build) ───────
FONT_DECLARE(status_icons_40);

// ════════════════════════════════════════════════════════════════
// CWC_StatusCard.h — StatusCard widget สำหรับแสดงค่า sensor
// ════════════════════════════════════════════════════════════════
//
// ── ASSET PREPARATION ────────────────────────────────────────────
//
// [ICON]  สร้างไฟล์ iconlist.txt ใน src/:
// ┌──────────────────────────────────────────────┐
// │  wb_sunny            E430                    │
// │  bolt                EA0B                    │
// │  battery_charging    E1A3                    │
// │  water_drop          E798                    │
// │  thermostat          F076                    │
// │  water               F084                    │
// │  wind_power          EC0C                    │
// │  ev_station          E56D                    │
// │  solar_power         EC0F                    │
// │  home                E88A                    │
// │  settings            E8B8                    │
// └──────────────────────────────────────────────┘
//   รันคำสั่ง:
//     iconconv iconlist.txt --size 40 --name status_icons
//   → ได้ status_icons_40.c ใน src/
//   → #include "status_icons_List.h" ใน main.cpp → build ได้เลย
//
// ════════════════════════════════════════════════════════════════
// FONT_DECLARE ต้องอยู่ใน main.cpp:
//   FONT_DECLARE(status_icons_40);
// ════════════════════════════════════════════════════════════════

// ── Icon Symbols ─────────────────────────────────────────────────
#define SYMBOL_WB_SUNNY         "\uE430"   // ดวงอาทิตย์ / Solar
#define SYMBOL_BOLT             "\uEA0B"   // ฟ้าผ่า / Usage / Power
#define SYMBOL_BATTERY_CHARGE   "\uE1A3"   // แบตเตอรี่ชาร์จ / Battery
#define SYMBOL_WATER_DROP       "\uE798"   // หยดน้ำ / Humidity / Water
#define SYMBOL_THERMOSTAT       "\uF076"   // เทอร์โมสตัท / Temperature
#define SYMBOL_WATER            "\uF084"   // คลื่นน้ำ / Pool / Water level
#define SYMBOL_WIND_POWER       "\uEC0C"   // ลม / Wind / Turbine
#define SYMBOL_EV_STATION       "\uE56D"   // สถานีชาร์จ EV
#define SYMBOL_SOLAR_POWER      "\uEC0F"   // แผงโซลาร์
#define SYMBOL_HOME             "\uE88A"   // บ้าน / Home
#define SYMBOL_SETTINGS         "\uE8B8"   // ตั้งค่า / Settings

// ── Color presets (ตรงกับ card ในรูป) ───────────────────────────
#define STATUS_COLOR_SOLAR      TFT_COLOR_HEX(0xF5C842)   // เหลือง-ทอง
#define STATUS_COLOR_USAGE      TFT_COLOR_HEX(0xE04040)   // แดง
#define STATUS_COLOR_BATTERY    TFT_COLOR_HEX(0x4CAF50)   // เขียว
#define STATUS_COLOR_WATER      TFT_COLOR_HEX(0x29B6F6)   // ฟ้า
#define STATUS_COLOR_TEMP       TFT_COLOR_HEX(0xFF7043)   // ส้ม
#define STATUS_COLOR_WIND       TFT_COLOR_HEX(0x80DEEA)   // ฟ้าอ่อน
#define STATUS_COLOR_DEFAULT    TFT_COLOR_HEX(0x90CAF9)   // ฟ้าอ่อน

// ════════════════════════════════════════════════════════════════
// StatusCard : GRect  (CWC)
// ════════════════════════════════════════════════════════════════
//
// Layout — GRID 4col × 2row:
//   ┌──────┬────────────────┬────────┬──────┐
//   │ ICON │ LABEL NAME     │  VALUE │ unit │  ← row0 FR(1)
//   ├──────┴────────────────┴────────┴──────┤
//   │ ████████████░░░░░░░░░░░░░░░░░░░░░░░  │  ← row1 12px (bar)
//   └───────────────────────────────────────┘
//     56px   FR(1)           CONTENT  CONTENT
//
// ── การใช้งาน ──────────────────────────────────────────────────
//
//   // ① ประกาศ global
//   StatusCard card_solar;
//   StatusCard card_usage;
//   StatusCard card_battery;
//
//   // ② setup ใน setup() — แบบ operator= เหมือน widget ปกติ
//   card_solar.setup(SYMBOL_WB_SUNNY,   "SOLAR",   "kW",  STATUS_COLOR_SOLAR,   0, 10.0f);
//   card_usage.setup(SYMBOL_BOLT,       "USAGE",   "kW",  STATUS_COLOR_USAGE,   0, 10.0f);
//   card_battery.setup(SYMBOL_BATTERY_CHARGE, "BATTERY", "%", STATUS_COLOR_BATTERY, 0, 100.0f);
//
//   // ③ อัปเดตค่า — แบบ widget = ค่า
//   card_solar   = 3.7f;
//   card_usage   = 5.2f;
//   card_battery = 28.0f;
//
//   หรือระบุ unit ใหม่พร้อมค่า:
//   card_solar.value(3.7f, "kW");
//
// ── ตัวอย่าง reuse ใน GRID ─────────────────────────────────────
//   GRect panel;
//   StatusCard card_solar(panel);
//   StatusCard card_usage(panel);
//   StatusCard card_battery(panel);
//   StatusCard card_pool(panel);
//
//   panel.GRID_CELL( GRID_COL{GRID_FR(1)}, GRID_ROW{80,80,80,80} );
//   panel.gap(4);
//
//   card_solar.grid_cell(0,0);
//   card_usage.grid_cell(0,1);
//   card_battery.grid_cell(0,2);
//   card_pool.grid_cell(0,3);
//
// ═══════════════════════════════════════════════════════════════

class StatusCard : public GRect {
public:
  // ── constructor ──────────────────────────────────────────────
  StatusCard(GPARENT) : GRect(parent) {}

  // ── create (LAZY) ────────────────────────────────────────────
  void create() {
    if (GRect::isCreated()) return;
    GRect::create();

    // card style
    GRect::bg_color(TFT_COLOR_HEX(0x111827));
    GRect::radius(10);
    GRect::border(0);
    GRect::height(72);
    // GRID หลัก: 4 col × 2 row
    //   col0=56px(icon)  col1=FR(1)(label)  col2=CONTENT(value)  col3=CONTENT(unit)
    //   row0=FR(1)(content)  row1=8px(bar)
    GRect::GRID_CELL( GRID_COL{56, GRID_FR(1), GRID_CONTENT, GRID_CONTENT},
                      GRID_ROW{GRID_FR(1), 12} );
    GRect::padding(0, 8, 8, 12);   // top, bottom, left, right

    // ── icon ──────────────────────────────────────────────────
    lb_icon.parent(this);
    lb_icon.font(status_icons_40, STATUS_COLOR_DEFAULT);
    lb_icon.size(SIZE_CONTENT, SIZE_CONTENT);
    lb_icon.grid_cell(0, 0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

    // ── label ชื่อ ─────────────────────────────────────────────
    lb_label.parent(this);
    lb_label.font(prasanmit_20, TFT_COLOR_HEX(0x9CA3AF));
    lb_label.height(SIZE_CONTENT);
    lb_label.grid_cell(1, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

    // ── value (ตัวเลข BebasNeue) ───────────────────────────────
    lb_value.parent(this);
    lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
    lb_value.height(SIZE_CONTENT);
    lb_value.grid_cell(2, 0, GRID_ALIGN_END, GRID_ALIGN_CENTER);

    // ── unit (prasanmit) ───────────────────────────────────────
    // Baseline alignment:
    // ทั้งคู่ใช้ GRID_ALIGN_CENTER → center ของ font box อยู่กึ่งกลาง cell
    // แต่ baseline ของแต่ละ font ห่างจาก center ต่างกัน:
    //   dist_from_center = font_base_line - font_height/2
    // offset_y = dist_value - dist_unit
    //          → ดัน lb_unit ให้ baseline ตรงกับ lb_value พอดี
    lb_unit.parent(this);
    lb_unit.font(prasanmit_25, TFT_COLOR_HEX(0xD1D5DB));
    lb_unit.height(SIZE_CONTENT);
    {
      // GRID_ALIGN_CENTER วาง center ของ font box ตรงกลาง cell
      // baseline อยู่ต่ำกว่า center = (line_height - base_line) - line_height/2
      // offset_y = dist_below_center_value - dist_below_center_unit
      //          = [(39-23)=16] - [(23-16)=7] = +9  → ดัน lb_unit ลง 9px
      const font_t* fv = lb_value.font();
      const font_t* fu = lb_unit.font();
      int32_t dist_v  = (fv->line_height - fv->base_line) - fv->line_height / 2;
      int32_t dist_u  = (fu->line_height - fu->base_line) - fu->line_height / 2;
      int32_t bl_offset = dist_v - dist_u;
      lb_unit.grid_cell(3, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 4, bl_offset);
    }

    // ── progress bar (row 1, span 4 col) ──────────────────────
    bar.parent(this);
    bar.height(5);
    bar.grid_cell(0, 1, 4, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    bar.radius(3);
    bar.padding(0);
    bar.bg_color(TFT_COLOR_HEX(0x374151));
    bar.color(STATUS_COLOR_DEFAULT, GPART_INDICATOR);
    bar.range(0, 10000);
    bar.value(0, false);
  }

  // ── setup — กำหนด icon / label / unit / color / range ────────
  void setup(const char* icon_symbol,
             const char* label_text,
             const char* unit_text,
             color_t     accent_color,
             float       range_min = 0.0f,
             float       range_max = 100.0f) {
    create();
    _unit     = unit_text;
    _rmin     = range_min;
    _rmax     = range_max;
    _accent   = accent_color;

    lb_icon  = icon_symbol;
    lb_icon.color(accent_color);

    lb_label = label_text;
    lb_unit  = unit_text;

    bar.color(accent_color, GPART_INDICATOR);
  }

  // ── value(float) — อัปเดตค่า ──────────────────────────────────
  void value(float v) {
    create();
    _val = v;
    _refreshValue();
  }

  // ── value(float, unit) — อัปเดตค่า + เปลี่ยน unit ────────────
  void value(float v, const char* unit) {
    create();
    _unit    = unit;
    lb_unit  = unit;
    _val     = v;
    _refreshValue();
  }

  // ── operator= — card_solar = 3.7f ─────────────────────────────
  void operator=(float v)        { value(v); }
  void operator=(int v)          { value((float)v); }

  // ── accent_color — เปลี่ยนสีหลังตั้งค่าแล้ว ──────────────────
  void accent_color(color_t c) {
    create();
    _accent = c;
    lb_icon.color(c);
    bar.color(c, GPART_INDICATOR);
  }

  // ── public child widgets (เข้าถึงได้จากภายนอกถ้าต้องการ) ─────
  GLabel lb_icon;
  GLabel lb_label;
  GLabel lb_value;
  GLabel lb_unit;
  GBar   bar;

private:
  String  _unit   = "";
  float   _rmin   = 0.0f;
  float   _rmax   = 100.0f;
  float   _val    = 0.0f;
  color_t _accent = STATUS_COLOR_DEFAULT;

  void _refreshValue() {
    // lb_value แสดงแค่ตัวเลข — lb_unit แสดง unit แยก
    if (_val == (int32_t)_val) {
      lb_value = StringX::printf("%d", (int32_t)_val);
    } else {
      lb_value = StringX::printf("%.1f", _val);
    }

    // bar: map ค่าเป็น % แบบ 0-10000 (ละเอียด)
    float clamped = constrain(_val, _rmin, _rmax);
    float pct     = (_rmax > _rmin) ? (clamped - _rmin) / (_rmax - _rmin) : 0.0f;
    bar.value((int32_t)(pct * 10000), true);
  }
};