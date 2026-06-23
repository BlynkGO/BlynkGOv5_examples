// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — ใช้เฉพาะ built-in fonts
// prasanmit_20, prasanmit_30, BebasNeueBold_num_45
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_GGaugeGRAFANA.h"

// ════════════════════════════════════════════════════════════════
//  CWC: SensorCard — card พื้นหลัง gauge พร้อม label หัวข้อ
// ════════════════════════════════════════════════════════════════
class SensorCard : public GRect {
  public:
    SensorCard(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();
      GRect::height(SIZE_CONTENT);
      GRect::color(TFT_COLOR_HEX(0x1E2230));
      GRect::radius(14);
      GRect::border(0);

      this->GRID_CELL(
        GRID_COL{ GRID_FR(1) },
        GRID_ROW{ GRID_CONTENT, GRID_CONTENT }
      );
      this->padding(8, 8, 8, 8, 6);

      lb_header.parent(this);
      lb_header.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
      lb_header.grid_cell(0, 0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

      gauge.parent(this);
      gauge.size(200, 200);
      gauge.grid_cell(0, 1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    }

    inline void setTitle(const char* t, const char* u = "") {
      create();
      lb_header = t;          // ← แสดงชื่อที่ card header เท่านั้น
      // gauge.title(t)       ← ไม่ใส่ เพื่อไม่ให้ซ้ำ
      gauge.unit(u);
    }

    inline void setValue(float v)               { create(); gauge.value(v);       }
    inline void setRange(int32_t mn, int32_t mx){ create(); gauge.range(mn, mx);  }

    void setSections(const std::vector<uint32_t>& rng,
                     const std::vector<GColor>&    clr) {
      create();
      gauge.sections(rng, clr);
    }

    GGaugeGRAFANA gauge;
    GLabel        lb_header;
};

// ════════════════════════════════════════════════════════════════
//  Widget declarations
//
//  GRID layout ของ cont_frame (12 col x 2 row):
//
//  col:  0   1   2   3   4   5   6   7   8   9  10  11
//        ┌─────────────────────┬─────────────────┬──────┐  row 0 (gauge zone)
//        │   card_temp (0..4)  │ card_humid(5..9)│ WiFi │
//        ├─────────────────────┴─────────────────┴──────┤  row 1 (chart zone)
//        │              rect_chart (0..11)               │
//        └───────────────────────────────────────────────┘
// ════════════════════════════════════════════════════════════════

GContainer cont_frame;

// ── Gauge zone ──────────────────────────────────────────────────
SensorCard  card_temp(cont_frame);
SensorCard  card_humid(cont_frame);

// ── Chart zone ──────────────────────────────────────────────────
GRect           rect_chart(cont_frame);
  GLabel          lb_chart_title(rect_chart);
  GChart          chart(rect_chart);
  chart_series_t* series_temp  = nullptr;
  chart_series_t* series_humid = nullptr;

// ── WiFi ────────────────────────────────────────────────────────
GWiFiManager wifi_manager(cont_frame);

// ════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ── Main container ──────────────────────────────────────────
  cont_frame.size(PCT(100), PCT(100));
  cont_frame.bg_color(TFT_COLOR_HEX(0x12141C));
  cont_frame.border(0);
  cont_frame.radius(0);
  cont_frame.GRID_CELL(
    GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1),
              GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1),
              GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1) },
    GRID_ROW{ GRID_CONTENT, GRID_FR(1) }
  );
  cont_frame.padding(12, 12, 12, 12, 10);

  // ── Gauge cards (row 0) ─────────────────────────────────────
  card_temp.grid_cell(0, 0, 5, 1);       // GRect → STRETCH อัตโนมัติ
  card_temp.setTitle("อุณหภูมิ", "°C");
  card_temp.setRange(0, 60);
  card_temp.setSections(
    {0, 20, 40, 50, 60},
    { TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED }
  );
  card_temp.setValue(28.5f);

  card_humid.grid_cell(5, 0, 5, 1);      // GRect → STRETCH อัตโนมัติ
  card_humid.setTitle("ความชื้น", "%RH");
  card_humid.setRange(0, 100);
  card_humid.setSections(
    {0, 30, 60, 80, 100},
    { TFT_RED, TFT_GREEN, TFT_YELLOW, TFT_RED }
  );
  card_humid.setValue(65.0f);

  wifi_manager.grid_cell(10, 0, 2, 1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

  // ── Chart (row 1) ───────────────────────────────────────────
  rect_chart.grid_cell(0, 1, 12, 1);     // GRect → STRETCH อัตโนมัติ
  rect_chart.color(TFT_COLOR_HEX(0x1E2230));
  rect_chart.radius(14);
  rect_chart.border(0);

  lb_chart_title.font(prasanmit_20, TFT_COLOR_HEX(0x8892AA));
  lb_chart_title = "อุณหภูมิ & ความชื้น";
  lb_chart_title.align(ALIGN_TOP_LEFT, 14, 10);

  chart.size(PCT(100), PCT(82));
  chart.align(ALIGN_BOTTOM_MID, 0, -6);
  chart.chart_type(CHART_TYPE_LINE);
  chart.div_line_count( 0, 0);
  chart.range(0, 100);
  chart.point_count(60);
  chart.bg_opa(0);
  chart.border(0);
  chart.line_width(2, GPART_ITEMS);
  chart.line_opa(200, GPART_ITEMS);
  chart.draw_grad(true);

  series_temp  = chart.createSerie(TFT_COLOR_HSV(10,  90, 100));
  series_humid = chart.createSerie(TFT_COLOR_HSV(200, 90, 100));

  for (int i = 0; i < 60; i++) {
    chart.addPoint(series_temp,  random(22, 38));
    chart.addPoint(series_humid, random(45, 80));
  }

  // ── Timer อัพเดตทุก 2 วินาที ────────────────────────────────
  static SoftTimer t_sensor;
  t_sensor.setInterval(2000, []() {
    float temp  = 25.0f + (float)random(-50, 80) / 10.0f;
    float humid = 55.0f + (float)random(-50, 50) / 5.0f;

    card_temp.setValue(temp);
    card_humid.setValue(humid);
    chart.addPoint(series_temp,  (int32_t)temp);
    chart.addPoint(series_humid, (int32_t)humid);
  }, true);
}

void loop() {
  BlynkGO.update();
}