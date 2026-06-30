#include <BlynkGOv5.h>
#include "CWC_DonutChart.h"
// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
//
// [FONT]  fontconv --font Roboto-Bold.ttf   --size 40 -eng_basic -o roboto_bold_40.c
//         fontconv --font Roboto-Medium.ttf --size 18 -eng        -o roboto_medium_18.c
//         fontconv --font Roboto-Medium.ttf --size 14 -eng        -o roboto_medium_14.c
//         Download: https://fonts.google.com/specimen/Roboto
//
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_DonutChart.h"

// ── [FONT] ──────────────────────────────────────────────────────
FONT_DECLARE(roboto_bold_40);
FONT_DECLARE(roboto_medium_18);
FONT_DECLARE(roboto_medium_14);

// ── Color Palette ────────────────────────────────────────────────
#define COLOR_BG        TFT_COLOR_HEX(0xF0F4F5)
#define COLOR_CARD_BG   TFT_COLOR_HEX(0xFFFFFF)
#define COLOR_TITLE     TFT_COLOR_HEX(0x1A1A2E)
#define COLOR_SUBTITLE  TFT_COLOR_HEX(0x6B7280)
#define COLOR_GREEN     TFT_COLOR_HEX(0x22C55E)
#define COLOR_BORDER    TFT_COLOR_HEX(0xE5E7EB)

// ── Donut segment data ───────────────────────────────────────────
struct DonutSeg { float pct; color_t color; const char* label; };
static const DonutSeg SEGS[] = {
  { 22, TFT_COLOR_HEX(0x6EE7B7), "New"         },
  { 18, TFT_COLOR_HEX(0x67E8F9), "In research" },
  { 15, TFT_COLOR_HEX(0xC4B5FD), "In progress" },
  { 14, TFT_COLOR_HEX(0x93C5FD), "In review"   },
  { 21, TFT_COLOR_HEX(0xFDE68A), "Completed"   },
  { 10, TFT_COLOR_HEX(0xFCA5A5), "Backlog"     },
};
static const int SEG_COUNT = 6;

// ════════════════════════════════════════════════════════════════
//  CWC: DonutLegend
//  - แสดง legend list (dot + label) ของทุก segment
//  - inherit GRect → จัด layout ด้วย GRID_CELL ภายในตัวเอง
//
//  วิธีใช้:
//    DonutLegend legend;
//    legend.grid_cell(1, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);
//    legend.create();
// ════════════════════════════════════════════════════════════════
class DonutLegend : public GRect {
  public:
    DonutLegend(GPARENT) : GRect(parent) {}

    // แต่ละ item = GCircle dot + GLabel
    struct Item {
      GCircle dot;
      GLabel  lb;
    } items[SEG_COUNT];

    void create() {
      if (GRect::isCreated()) return;

      GRect::create();
      GRect::color(COLOR_CARD_BG);
      GRect::border(0);
      GRect::height(SIZE_CONTENT);
      GRect::width(SIZE_CONTENT);

      // GRID: 1 col, SEG_COUNT rows — แต่ละ row = 1 item
      GRID_ROW rows;
      for (int i = 0; i < SEG_COUNT; i++) rows.push_back(GRID_CONTENT);

      GRect::GRID_CELL( GRID_COL{ GRID_CONTENT }, rows );
      GRect::padding(4, 4, 0, 0, 6);

      for (int i = 0; i < SEG_COUNT; i++) {
        // container แต่ละ item
        // วาง dot + label เคียงกันใน row i
        items[i].dot.parent(this);
        items[i].dot.size(10, 10);
        items[i].dot.border(0);
        items[i].dot.color(SEGS[i].color);
        items[i].dot.grid_cell(0, i, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        items[i].lb.parent(this);
        items[i].lb.font(roboto_medium_14, COLOR_SUBTITLE);
        items[i].lb.grid_cell(0, i, GRID_ALIGN_START, GRID_ALIGN_CENTER);
        items[i].lb = String("   ") + SEGS[i].label;  // indent ให้ label เยื้องหลัง dot
      }
    }
};

// ════════════════════════════════════════════════════════════════
//  CWC: StatCard
// ════════════════════════════════════════════════════════════════
class StatCard : public GRect {
  public:
    StatCard(GPARENT) : GRect(parent) {}

    GLabel lb_title;
    GLabel lb_value;

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      GRect::height(SIZE_CONTENT);
      GRect::color(COLOR_CARD_BG);
      GRect::radius(12);
      GRect::border(1, COLOR_BORDER);
      GRect::shadow(6, 2, TFT_BLACK);
      GRect::shadow_opa(25);
      GRect::transform_size(-6,-6);
      GRect::GRID_CELL( GRID_COL{ GRID_FR(1) },
                        GRID_ROW{ GRID_CONTENT, GRID_CONTENT } );
      GRect::padding(14, 14, 16, 16, 8);
      GRect::min_size(150, 120);

        lb_title.parent(this);
        lb_title.font(roboto_medium_14, COLOR_SUBTITLE);
        lb_title.grid_cell(0, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
        lb_title.border(1, COLOR_BORDER);
        lb_title.border_side(BORDER_SIDE_BOTTOM);
        lb_title.padding(0, 8, 0, 0);

        lb_value.parent(this);
        lb_value.font(roboto_bold_40, COLOR_TITLE);
        lb_value.grid_cell(0, 1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    }

    void title(String t) { create(); lb_title = t; }
    void value(int v)    { create(); lb_value = String(v); }
};

// ════════════════════════════════════════════════════════════════
//  Global Declarations
// ════════════════════════════════════════════════════════════════
GRect rect_root;

  GRect    rect_header;
    GLabel   lb_page_title;
    GLabel   lb_author;

  GRect    rect_cards;
    StatCard card_active;
    StatCard card_new;
    StatCard card_completed;
    StatCard card_all;

  GRect    rect_bottom;

    // ── LEFT panel ──────────────────────────────────────────
    GRect       rect_donut_panel;
      GLabel      lb_donut_title;
      GRect       rect_donut_body;   // GRID 2 col: [donut | legend]
        DonutChart  donut;
        DonutLegend donut_legend;

    // ── RIGHT panel ─────────────────────────────────────────
    GRect    rect_bar_panel;
      GLabel   lb_bar_title;
      GChart   chart_bar;

chart_series_t* series_bar = nullptr;


// ════════════════════════════════════════════════════════════════
//  setup()
// ════════════════════════════════════════════════════════════════
void setup() {
  BlynkGO.begin();


  // ── Root ────────────────────────────────────────────────────
  rect_root.color(COLOR_BG);
  rect_root.GRID_CELL( GRID_COL{ GRID_FR(1) },
                       GRID_ROW{ 55, 120, GRID_FR(1) } );
  rect_root.padding(16, 16, 20, 20, 10);

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  //  ROW 0 — Header
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  rect_header.parent(rect_root);
  rect_header.grid_cell(0, 0);
  rect_header.color(COLOR_BG);
  rect_header.border(0);
  rect_header.GRID_CELL( GRID_COL{ GRID_CONTENT, GRID_CONTENT, GRID_FR(1) },
                         GRID_ROW{ GRID_FR(1) } );
  rect_header.padding(0, 0, 0, 0, 12);

    lb_page_title.parent(rect_header);
    lb_page_title.font(prasanmit_35, COLOR_TITLE);
    lb_page_title.grid_cell(0, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);
    lb_page_title = "Projects";

    lb_author.parent(rect_header);
    lb_author.font(roboto_medium_14, COLOR_SUBTITLE);
    lb_author.grid_cell(1, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 0, (lb_page_title.font()->base_line - lb_author.font()->base_line)/2);
    lb_author = "By BlynkGO";

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  //  ROW 1 — Stat Cards
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  rect_cards.parent(rect_root);
  rect_cards.grid_cell(0, 1);
  rect_cards.color(COLOR_BG);
  rect_cards.border(0);
  rect_cards.GRID_CELL( GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1) },
                        GRID_ROW{ GRID_FR(1) } );
  rect_cards.padding(0, 0, 0, 0, 10);
  rect_cards.overflow_visible(true);

    card_active.parent(rect_cards);
    card_active.grid_cell(0, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    card_active.title("Projects: Active");
    card_active.value(54);

    card_new.parent(rect_cards);
    card_new.grid_cell(1, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    card_new.title("Projects: New");
    card_new.value(14);

    card_completed.parent(rect_cards);
    card_completed.grid_cell(2, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    card_completed.title("Projects: Completed");
    card_completed.value(25);

    card_all.parent(rect_cards);
    card_all.grid_cell(3, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    card_all.title("Projects: All");
    card_all.value(103);

  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  //  ROW 2 — Bottom panels
  // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  rect_bottom.parent(rect_root);
  rect_bottom.grid_cell(0, 2);
  rect_bottom.color(COLOR_BG);
  rect_bottom.border(0);
  rect_bottom.GRID_CELL( GRID_COL{ GRID_FR(5), GRID_FR(8) },
                         GRID_ROW{ GRID_FR(1) } );
  rect_bottom.padding(0, 0, 0, 0, 10);

  // ──────────────────────────────────────────────────────────
  //  LEFT — Projects by Status
  // ──────────────────────────────────────────────────────────
  rect_donut_panel.parent(rect_bottom);
  rect_donut_panel.grid_cell(0, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  rect_donut_panel.color(COLOR_CARD_BG);
  rect_donut_panel.radius(12);
  rect_donut_panel.border(1, COLOR_BORDER);
  rect_donut_panel.shadow(6, 2, TFT_BLACK);
  rect_donut_panel.shadow_opa(25);
  rect_donut_panel.transform_width(-6);
  rect_donut_panel.transform_height(-6);
  rect_donut_panel.GRID_CELL( GRID_COL{ GRID_FR(1) },
                              GRID_ROW{ GRID_CONTENT, GRID_FR(1) } );
  rect_donut_panel.padding(14, 14, 16, 16, 8);

    lb_donut_title.parent(rect_donut_panel);
    lb_donut_title.grid_cell(0, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    lb_donut_title.font(roboto_medium_18, COLOR_TITLE);
    lb_donut_title.border(1, COLOR_BORDER);
    lb_donut_title.border_side(BORDER_SIDE_BOTTOM);
    lb_donut_title.padding(0, 8, 0, 0);
    lb_donut_title = "Projects by Status";

    // rect_donut_body: GRID 2 col
    // col 0 = DonutChart (FR1) | col 1 = DonutLegend (CONTENT)
    rect_donut_body.parent(rect_donut_panel);
    rect_donut_body.grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    rect_donut_body.color(COLOR_CARD_BG);
    rect_donut_body.border(0);
    rect_donut_body.GRID_CELL( GRID_COL{ GRID_FR(1), GRID_CONTENT },
                               GRID_ROW{ GRID_FR(1) } );
    rect_donut_body.padding(8, 8, 8, 8, 0);

      // col 0 — DonutChart CWC
      donut.parent(rect_donut_body);
      donut.grid_cell(0, 0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      donut.size(150, 150);   // สร้าง arc ทั้งหมดภายใน CWC

      // ──> วิธีใช้ Loop ดึงข้อมูลจากโครงสร้างอาร์เรย์มาใส่ให้ donut <──
      std::vector<float>   chart_vals;
      std::vector<color_t> chart_colors;

      for (int i = 0; i < SEG_COUNT; i++) {
        chart_vals.push_back(SEGS[i].pct);
        chart_colors.push_back(SEGS[i].color);
      }

      donut.unit("%");
      donut.value(chart_vals);     // ส่งชุดข้อมูลตัวเลขทั้งหมด
      donut.colors(chart_colors);  // ส่งชุดคู่สีทั้งหมดเข้าไป


      // col 1 — DonutLegend CWC
      donut_legend.parent(rect_donut_body);
      donut_legend.grid_cell(1, 0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

  // ──────────────────────────────────────────────────────────
  //  RIGHT — Projects by Types (Bar Chart)
  // ──────────────────────────────────────────────────────────
  rect_bar_panel.parent(rect_bottom);
  rect_bar_panel.grid_cell(1, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  rect_bar_panel.color(COLOR_CARD_BG);
  rect_bar_panel.radius(12);
  rect_bar_panel.border(1, COLOR_BORDER);
  rect_bar_panel.shadow(6, 2, TFT_BLACK);
  rect_bar_panel.shadow_opa(25);
  rect_bar_panel.transform_size(-6,-6);
  rect_bar_panel.GRID_CELL( GRID_COL{ GRID_FR(1) },
                            GRID_ROW{ GRID_CONTENT, GRID_FR(1) } );
  rect_bar_panel.padding(14, 14, 16, 16, 8);

    lb_bar_title.parent(rect_bar_panel);
    lb_bar_title.grid_cell(0, 0, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    lb_bar_title.font(roboto_medium_18, COLOR_TITLE);
    lb_bar_title.border(1, COLOR_BORDER);
    lb_bar_title.border_side(BORDER_SIDE_BOTTOM);
    lb_bar_title.padding(0, 8, 0, 0);
    lb_bar_title = "Projects by Types";

    chart_bar.parent(rect_bar_panel);
    chart_bar.grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    chart_bar.type(CHART_TYPE_BAR);
    chart_bar.range(0, 320);
    chart_bar.point_count(7);
    chart_bar.bg_color(COLOR_CARD_BG);
    chart_bar.border(0);
    // เส้น บอกระดับอ้างอิงให้ chart
    chart_bar.div_line_count(5,0);
    chart_bar.line_dash_width(3);
    chart_bar.line_dash_gap(3);

    series_bar = chart_bar.createSerie(COLOR_GREEN);
    // Feb  Mar  Apr  May  Jun  Jul  Aug
    chart_bar.addPoint(series_bar,  65);
    chart_bar.addPoint(series_bar, 260);
    chart_bar.addPoint(series_bar, 170);
    chart_bar.addPoint(series_bar,  80);
    chart_bar.addPoint(series_bar,  50);
    chart_bar.addPoint(series_bar, 110);
    chart_bar.addPoint(series_bar, 305);
}

void loop() {
  BlynkGO.update();
}