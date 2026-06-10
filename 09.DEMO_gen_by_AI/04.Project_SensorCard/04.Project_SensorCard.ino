// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
// ไม่มี asset เพิ่มเติม ใช้เฉพาะ built-in fonts
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_SensorCard.h"

// ─────────────────────────────────────────────────────────────────
//  Global widgets
// ─────────────────────────────────────────────────────────────────
GRect rect_bg;
  GLabel lb_header(rect_bg);
  GRect  rect_cards(rect_bg);
    SensorCard card_temp(rect_cards);
    SensorCard card_humid(rect_cards);
    SensorCard card_co2(rect_cards);
    SensorCard card_pm25(rect_cards);

void setup() {
  BlynkGO.begin();

  // ── Background ──
  rect_bg.color(TFT_COLOR_HEX(0x111122));
  rect_bg.GRID_CELL(
    GRID_COL{ GRID_FR(1) },
    GRID_ROW{ GRID_CONTENT, GRID_FR(1) }
  );
  rect_bg.padding(20, 20, 20, 20, 0);

    // ── Header ──
    lb_header.font(prasanmit_35, TFT_WHITE);
    lb_header = "Sensor Dashboard";
    lb_header.grid_cell(0, 0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

    // ── Card container ──
    rect_cards.grid_cell(0, 1);
    rect_cards.bg_opa(0);
    rect_cards.GRID_CELL(
      GRID_COL{ GRID_FR(1), GRID_FR(1) },
      GRID_ROW{ GRID_FR(1), GRID_FR(1) }
    );
    rect_cards.padding(10, 10, 10, 10, 14);

      // ── Card setup ──
      card_temp.grid_cell(0, 0);
      card_temp.title("Temperature");
      card_temp.unit("°C");
      card_temp.range(0, 50);
      card_temp.line_color(TFT_COLOR_HEX(0xFF6B6B));

      card_humid.grid_cell(1, 0);
      card_humid.title("Humidity");
      card_humid.unit("%");
      card_humid.range(0, 100);
      card_humid.line_color(TFT_COLOR_HEX(0x4ECDC4));

      card_co2.grid_cell(0, 1);
      card_co2.title("CO2");
      card_co2.unit("ppm");
      card_co2.range(400, 2000);
      card_co2.line_color(TFT_COLOR_HEX(0xFFD93D));

      card_pm25.grid_cell(1, 1);
      card_pm25.title("PM2.5");
      card_pm25.unit("µg");
      card_pm25.range(0, 150);
      card_pm25.line_color(TFT_COLOR_HEX(0xA8E6CF));

  // ── Simulate sensor data ──
  static SoftTimer timer;
  timer.setInterval(800, []() {
    card_temp .addValue(25.0 + random(-30, 80) / 10.0, 1);
    card_humid.addValue(60.0 + random(-100, 100) / 10.0, 1);
    card_co2  .addValue(800  + random(-200, 500), 0);
    card_pm25 .addValue(35.0 + random(-100, 200) / 10.0, 1);
  }, true);
}

void loop() {
  BlynkGO.update();
}