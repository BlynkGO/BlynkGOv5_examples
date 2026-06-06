// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — run these commands in src/ before build
// ════════════════════════════════════════════════════════════════
//
// [FONT] ใช้ built-in fonts ทั้งหมด — ไม่ต้องรัน fontconv
//        prasanmit_25  → label แกน Y, unit
//        prasanmit_30  → ชื่อ series, title
//        prasanmit_35  → ค่าตัวเลขปัจจุบัน
//
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>

// ── ค่าคงที่ ──────────────────────────────────────────────────
#define CHART_POINTS      200         // จำนวนจุดข้อมูลต่อ series (ความละเอียดสูง)
#define CHART_W           680
#define CHART_H           320
#define Y_MIN             0
#define Y_MAX             100
#define GRID_H            10          // เส้นแบ่งแนวนอน
#define GRID_V            20          // เส้นแบ่งแนวตั้ง
#define TIMER_INTERVAL_MS 50          // อัปเดตทุก 50ms = 20 fps

// ── Series ────────────────────────────────────────────────────
chart_series_t* series_a;            // Series A — อุณหภูมิ
chart_series_t* series_b;            // Series B — ความชื้น

// ── Layout: container หลัก ────────────────────────────────────
GContainer cont_main;
  GLabel    lb_title;                // หัวข้อ
  GLabel    lb_val_a;                // ค่าปัจจุบัน series A
  GLabel    lb_val_b;                // ค่าปัจจุบัน series B
  GLabel    lb_unit_a;
  GLabel    lb_unit_b;
  GScale    scale_y;                 // แกน Y ด้านซ้าย
  GChart    chart;                   // กราฟหลัก
  GLabel    lb_legend_a;             // legend A
  GLabel    lb_legend_b;             // legend B

// ── ตัวแปร rolling index ──────────────────────────────────────
static int32_t cur_idx = -1;

// ── ฟังก์ชัน simulate sensor data ────────────────────────────
float simulate_a(float t) {          // คลื่น sine เลียนแบบอุณหภูมิ
  return 50.0f + 40.0f * sin(t * 0.08f) + (random(-5, 5));
}
float simulate_b(float t) {          // คลื่น cosine เลียนแบบความชื้น
  return 50.0f + 30.0f * cos(t * 0.06f) + (random(-4, 4));
}

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  BlynkGO.begin();

  // ── Container หลัก ──────────────────────────────────────────
  cont_main.size(PCT(100), PCT(100));
  cont_main.GRID_CELL(
    GRID_COL{ 50, GRID_FR(1) },
    GRID_ROW{ 55, GRID_FR(1), 40 }
  );
  cont_main.padding(10, 10, 15, 15, 8, 8);
  cont_main.color(TFT_COLOR_HEX(0x0D1117), TFT_COLOR_HEX(0x161B22));
  cont_main.grad_dir(GRAD_DIR_VER);

  // ── Title ────────────────────────────────────────────────────
  lb_title.parent(cont_main);
  lb_title.grid_cell(0, 0, 2, 1);
  lb_title.font(prasanmit_35, TFT_COLOR_HEX(0xE6EDF3));
  lb_title = "High-Resolution Chart";
  lb_title.text_align(TEXT_ALIGN_CENTER);

  // ── Scale แกน Y ─────────────────────────────────────────────
  scale_y.parent(cont_main);
  scale_y.grid_cell(0, 1, 1, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  scale_y.range(Y_MIN, Y_MAX);
  scale_y.mode(SCALE_MODE_VER_LEFT, 6, 1);
  scale_y.tick_length(4, 0);
  scale_y.axis_thickness(1);
  scale_y.label_show(true);
  scale_y.color(TFT_COLOR_HEX(0x8B949E));
  scale_y.font(prasanmit_25, TFT_COLOR_HEX(0x8B949E));

  // ── GChart ───────────────────────────────────────────────────
  chart.parent(cont_main);
  chart.grid_cell(1, 1, 1, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);

  chart.type(CHART_TYPE_LINE);
  chart.point_count(CHART_POINTS);               // ความละเอียดสูง 200 จุด
  chart.range(CHART_AXIS_PRIMARY_Y, Y_MIN, Y_MAX);
  chart.div_line_count(GRID_H + 1, GRID_V + 1);  // เส้นกริดละเอียด

  // สไตล์พื้นหลัง
  chart.bg_color(TFT_COLOR_HEX(0x0D1117));
  chart.bg_opa(255);
  chart.border(1);
  chart.border_opa(60);

  // สไตล์เส้นกริด
  chart.line_color(TFT_COLOR_HEX(0x30363D));
  chart.line_width(1);
  chart.line_opa(80);

  // Series A — อุณหภูมิ (สีส้ม)
  series_a = chart.createSerie(TFT_COLOR_HEX(0xFF7B54));
  chart.line_width(2, GPART_ITEMS);
  chart.line_rounded(true, GPART_ITEMS);
  chart.line_opa(230, GPART_ITEMS);
  chart.draw_grad(true);

  // Series B — ความชื้น (สีฟ้า)
  series_b = chart.createSerie(TFT_COLOR_HEX(0x58A6FF));

  // ── Legend & ค่าปัจจุบัน ─────────────────────────────────────
  lb_legend_a.parent(cont_main);
  lb_legend_a.grid_cell(0, 2, 1, 1, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
  lb_legend_a.font(prasanmit_25, TFT_COLOR_HEX(0xFF7B54));
  lb_legend_a = SYMBOL_THERMO  " Temp";

  lb_legend_b.parent(cont_main);
  lb_legend_b.grid_cell(1, 2, 1, 1, GRID_ALIGN_START, GRID_ALIGN_CENTER);
  lb_legend_b.font(prasanmit_25, TFT_COLOR_HEX(0x58A6FF));
  lb_legend_b = SYMBOL_HUMID " Humid";

  // ค่าปัจจุบัน (วางซ้อนบน chart มุมซ้ายบน)
  lb_val_a.parent(chart);
  lb_val_a.ignore_layout(true);
  lb_val_a.font(prasanmit_35, TFT_COLOR_HEX(0xFF7B54));
  lb_val_a = "--";
  lb_val_a.position(12, 8);

  lb_unit_a.parent(chart);
  lb_unit_a.ignore_layout(true);
  lb_unit_a.font(prasanmit_25, TFT_COLOR_HEX(0xFF7B54));
  lb_unit_a = "°C";
  lb_unit_a.align(lb_val_a, ALIGN_RIGHT, 2, 0);

  lb_val_b.parent(chart);
  lb_val_b.ignore_layout(true);
  lb_val_b.font(prasanmit_35, TFT_COLOR_HEX(0x58A6FF));
  lb_val_b = "--";
  lb_val_b.position(12, 48);

  lb_unit_b.parent(chart);
  lb_unit_b.ignore_layout(true);
  lb_unit_b.font(prasanmit_25, TFT_COLOR_HEX(0x58A6FF));
  lb_unit_b = "%";
  lb_unit_b.align(lb_val_b, ALIGN_RIGHT, 2, 0);

  // ── Timer: อัปเดตข้อมูลแบบ real-time ──────────────────────
  static SoftTimer timer;
  static float t = 0;
  timer.setInterval(TIMER_INTERVAL_MS, []() {
    t += 1.0f;
    cur_idx = (cur_idx + 1) % CHART_POINTS;

    float va = constrain(simulate_a(t), Y_MIN, Y_MAX);
    float vb = constrain(simulate_b(t), Y_MIN, Y_MAX);

    // ใส่ข้อมูลแบบ direct เพื่อ performance สูงสุด
    series_a->y_points[cur_idx] = (int32_t)va;
    series_b->y_points[cur_idx] = (int32_t)vb;

    // เว้นช่วง 8 จุดหน้า cursor ให้ดูเหมือนกราฟ scrolling
    for (int j = cur_idx + 1; j <= cur_idx + 8; j++) {
      series_a->y_points[j % CHART_POINTS] = CHART_POINT_NONE;
      series_b->y_points[j % CHART_POINTS] = CHART_POINT_NONE;
    }

    chart.invalidate();   // redraw chart

    // อัปเดต label ค่าปัจจุบัน
    lb_val_a = String((int)va);
    lb_val_b = String((int)vb);

    lb_unit_b.align(lb_val_b, ALIGN_RIGHT, 2, 0);
    lb_unit_a.align(lb_val_a, ALIGN_RIGHT, 2, 0);
  });
}

// ─────────────────────────────────────────────────────────────
void loop() {
  BlynkGO.update();
}
