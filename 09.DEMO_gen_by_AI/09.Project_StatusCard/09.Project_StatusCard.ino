// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
//
// [ICON]  สร้างไฟล์ iconlist.txt ใน src/ ด้วยเนื้อหาต่อไปนี้:
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

#include <BlynkGOv5.h>
#include "CWC_StatusCard.h"
#include "status_icons_List.h"    // ← ได้จาก iconconv

FONT_DECLARE(status_icons_40);    // ← custom icon font

// ── Layout ──────────────────────────────────────────────────────
//   800×480 — แบบเดียวกับรูป: คอลัมน์ซ้ายมือ 4 cards แนวตั้ง
//
//   ┌───────────────┬─────────────────────┐
//   │  card_solar   │                     │
//   │  card_usage   │   (ส่วนขวา)         │
//   │  card_battery │                     │
//   │  card_pool    │                     │
//   └───────────────┴─────────────────────┘

GRect  panel_left;
  StatusCard card_solar(panel_left);
  StatusCard card_usage(panel_left);
  StatusCard card_battery(panel_left);
  StatusCard card_pool(panel_left);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // ── panel ซ้าย ──────────────────────────────────────────────
  panel_left.bg_opa(0);
  panel_left.width(PCT(45));
  panel_left.align(ALIGN_LEFT_MID, 8, 0);
  panel_left.GRID_CELL( GRID_COL{GRID_FR(1)},
                        GRID_ROW{GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1)} );
  panel_left.gap(6);
  panel_left.padding(8);

  // ── StatusCard setup ─────────────────────────────────────────
  card_solar.grid_cell(0, 0);
  card_solar.setup(SYMBOL_WB_SUNNY, "SOLAR", "kW",
                   STATUS_COLOR_SOLAR, 0.0f, 15.0f);

  card_usage.grid_cell(0, 1);
  card_usage.setup(SYMBOL_BOLT, "USAGE", "kW",
                   STATUS_COLOR_USAGE, 0.0f, 10.0f);

  card_battery.grid_cell(0, 2);
  card_battery.setup(SYMBOL_BATTERY_CHARGE, "BATTERY", "%",
                     STATUS_COLOR_BATTERY, 0.0f, 100.0f);

  card_pool.grid_cell(0, 3);
  card_pool.setup(SYMBOL_WATER, "POOL WATER", "°C",
                  STATUS_COLOR_WATER, 0.0f, 40.0f);

  // ── ค่าเริ่มต้น ───────────────────────────────────────────────
  card_solar   = 0.0f;
  card_usage   = 3.8f;
  card_battery = 28.0f;
  card_pool    = 15.0f;

  // ── simulate data update ──────────────────────────────────────
  static SoftTimer t_update;
  t_update.setInterval(3000, []() {
    card_solar   = (float)random(0, 150) / 10.0f;    // 0.0–15.0 kW
    card_usage   = (float)random(10, 80) / 10.0f;    // 1.0–8.0 kW
    card_battery = (float)random(10, 100);            // 10–100 %
    card_pool    = (float)random(120, 350) / 10.0f;  // 12.0–35.0 °C
  });
}

void loop() {
  BlynkGO.update();
}