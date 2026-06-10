// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
//
// [IMAGE]  วางไฟล์ภาพพื้นหลังใน folder เดียวกับ src/ แล้วรัน:
//            imgconv bg.png
//          จะได้ไฟล์ img_bg.c ใน src/
//          (resize ภาพให้เป็น 800×480 ก่อน imgconv)
//          หาภาพฟรี: https://unsplash.com
//
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_ValueCard.h"

// ─── Color Palette ──────────────────────────────────────────────
#define COLOR_BG_MAIN       TFT_COLOR_HEX(0x0A2A3A)   // พื้นหลังหลัก (น้ำเงินเข้ม)
#define COLOR_BG_CARD       TFT_COLOR_HEX(0x01466e)   // การ์ด sensor
#define COLOR_BG_HEADER     TFT_COLOR_HEX(0x167484)   // header bar
#define COLOR_BG_FOOTER     TFT_COLOR_HEX(0x0F3D2E)   // footer bar (เขียวเข้ม)
#define COLOR_ACCENT_CYAN   TFT_COLOR_HEX(0x00E5CC)   // ข้อความ Smart Farm / AUTO MODE
#define COLOR_VALUE         TFT_COLOR_HEX(0x4DD9FF)   // ตัวเลข value
#define COLOR_PUMP_STATUS   TFT_COLOR_HEX(0x66FF99)   // pump status text
#define COLOR_TITLE_WHITE   TFT_WHITE
#define COLOR_SUBTEXT       TFT_COLOR_HEX(0xAABBCC)   // ข้อความรอง

// ── [IMAGE] imgconv bg.png ───────────────────────────────────────
IMAGE_DECLARE(img_bg);

// ══════════════════════════════════════════════════════════════════
//  Widget Declarations (global)
// ══════════════════════════════════════════════════════════════════

// — Background
GRect rect_background;

// — Header
GRect  rect_header(rect_background); // parent เป็น rect_background เพื่อให้ header มีระยะห่างจากขอบหน้าจอมากขึ้น
GLabel lb_title_main;
GLabel lb_auto_mode;

// — Card area container
GRect       rect_cards(rect_background); // parent เป็น rect_background เพื่อให้การ์ดมีระยะห่างจากขอบหน้าจอมากขึ้น
ValueCard  card_temp;
ValueCard  card_humidity;
ValueCard  card_soil;

// — Footer
GRect  rect_footer(rect_background); // parent เป็น rect_background เพื่อให้ footer มีระยะห่างจากขอบหน้าจอมากขึ้น
GLabel lb_pump_label;
GLabel lb_pump_status;
GLabel lb_zone_info;

// ══════════════════════════════════════════════════════════════════
//  setup()
// ══════════════════════════════════════════════════════════════════
void setup() {
  BlynkGO.begin();

  // ── Background Image by GRect ใช้ xxx.image(...) ─────────────────────────────────────────
  rect_background.image(img_bg);
  rect_background.GRID_CELL( GRID_COL{ GRID_FR(1) }, GRID_ROW{ GRID_CONTENT, GRID_FR(1), GRID_CONTENT } ); // ขยายเต็มพื้นที่
  rect_background.padding(30); // top, bottom, left, right, gap_row, gap_col

  // ── Header Bar ───────────────────────────────────────────────
  rect_header.height(70);
  rect_header.color(COLOR_BG_HEADER);
  rect_header.bg_opa(220);
  rect_header.border_side(BORDER_SIDE_BOTTOM);
  rect_header.border(2);
  rect_header.border_color(COLOR_ACCENT_CYAN);
  rect_header.border_opa(80);
  rect_header.radius(0);
  rect_header.grid_cell(0, 0);
  rect_header.padding(0, 0, 24, 24);

    lb_title_main.parent(rect_header);
    lb_title_main.font(prasanmit_35, COLOR_ACCENT_CYAN);
    lb_title_main = "Smart Farm";
    lb_title_main.align(ALIGN_LEFT_MID);

    lb_auto_mode.parent(rect_header);
    lb_auto_mode.font(prasanmit_20, COLOR_ACCENT_CYAN);
    lb_auto_mode = "AUTO MODE";
    lb_auto_mode.align(ALIGN_RIGHT_MID);

  // ── Card Container — 3 ValueCards เรียงแนวนอน ──────────────
  rect_cards.bg_opa(0);           // โปร่งใส — เห็น background image
  rect_cards.border(0);
  rect_cards.radius(0);
  rect_cards.GRID_CELL( GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1) },
                         GRID_ROW{ GRID_CONTENT } );
  rect_cards.padding(16, 16, 0, 0, 0, 14);  // top,bottom,left,right,gap_row,gap_col
  rect_cards.grid_cell(0, 1); // วางในแถวที่ 1 (นับจาก 0) เพื่อให้ header กับ footer มีระยะห่างจากการ์ดมากขึ้น

    // Card 1 — Temperature
    card_temp.parent(rect_cards);
    card_temp.title("Temperature");
    card_temp.value("28 C");
    card_temp.grid_cell(0, 0);

    // Card 2 — Air Humidity
    card_humidity.parent(rect_cards);
    card_humidity.title("Air Humidity");
    card_humidity.value("64%");
    card_humidity.grid_cell(1, 0);

    // Card 3 — Soil Moisture
    card_soil.parent(rect_cards);
    card_soil.title("Soil Moisture");
    card_soil.value("72%");
    card_soil.grid_cell(2, 0);

  // ── Footer Bar ───────────────────────────────────────────────
  rect_footer.height(80);
  rect_footer.color(COLOR_BG_FOOTER);
  rect_footer.bg_opa(230);
  rect_footer.border(0);
  rect_footer.radius(0);
  rect_footer.grid_cell(0, 2); // วางในแถวที่ 2 (นับจาก 0)
  rect_footer.padding(0, 0, 24, 24);

    lb_pump_label.parent(rect_footer);
    lb_pump_label.font(prasanmit_25, COLOR_TITLE_WHITE);
    lb_pump_label = "Irrigation Pump";
    lb_pump_label.align(ALIGN_TOP_LEFT, 0, 8);

    lb_pump_status.parent(rect_footer);
    lb_pump_status.font(prasanmit_20, COLOR_PUMP_STATUS);
    lb_pump_status = "Running - 12 min left";
    lb_pump_status.align(lb_pump_label, ALIGN_OUT_BOTTOM_LEFT, 0, 2);

    lb_zone_info.parent(rect_footer);
    lb_zone_info.font(prasanmit_20, COLOR_TITLE_WHITE);
    lb_zone_info = "Zone A  |  Greenhouse 01";
    lb_zone_info.align(ALIGN_RIGHT_MID);
}

// ══════════════════════════════════════════════════════════════════
//  loop()
// ══════════════════════════════════════════════════════════════════
void loop() {
  BlynkGO.update();
}