#ifndef __CWC_SENSORCARD_H__
#define __CWC_SENSORCARD_H__

#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
//  CWC: SensorCard — การ์ดแสดงผล Sensor พร้อมกราฟ Mini Trend
// ════════════════════════════════════════════════════════════════
//  QUICK START / วิธีใช้งาน:
//  ─────────────────────────────────────────────────────────────
//  1. ประกาศตัวแปรตัววัตถุ (สไตล์ Lazy Code ไม่ต้องใช้ pointer/new)
//     SensorCard temp_card;
//
//  2. ตั้งค่าในฟังก์ชัน setup()
//     temp_card.parent(rect_panel);  // กำหนด parent คอนเทนเนอร์หลัก
//     temp_card.grid_cell(0, 0);     // จัดวางตำแหน่งในระบบตาราง
//     temp_card.title("อุณหภูมิห้อง");
//     temp_card.unit("°C");
//     temp_card.range(0, 50);        // กำหนดช่วงต่ำสุด-สูงสุดของกราฟ
//     temp_card.line_color(TFT_CYAN); // เปลี่ยนสีเส้นกราฟเทรนด์ (เลือกใส่หรือไม่ใส่ก็ได้)
//
//  3. การอัปเดตข้อมูล (เช่น ทุกๆ 1 วินาที หรือเมื่อได้รับค่าจาก Sensor)
//     temp_card.addValue(28.4);       // อัปเดตตัวเลขพร้อมดันเข้ากราฟ (ค่าเริ่มต้นทศนิยม 1 ตำแหน่ง)
//     temp_card.addValue(1013.25, 2); // สามารถระบุจำนวนตำแหน่งทศนิยมได้ตามต้องการ
//
//  PUBLIC MEMBERS (คอมโพเนนต์ภายในที่สามารถเข้าถึงเพื่อตกแต่งเพิ่มได้):
//  ─────────────────────────────────────────────────────────────
//  temp_card.lb_title  → GLabel ตัวอักษรหัวข้อ
//  temp_card.lb_value  → GLabel ตัวอักษรค่าปัจจุบัน (BebasNeueBold_num_45)
//  temp_card.lb_unit   → GLabel ตัวอักษรหน่วย
//  temp_card.chart     → GChart ตัวกราฟเทรนด์ขนาดเล็ก
// ════════════════════════════════════════════════════════════════

class SensorCard : public GRect {
  public:
    SensorCard(GPARENT) : GRect(parent) {}

    GLabel lb_title;
    GLabel lb_value;
    GLabel lb_unit;
    GChart chart;
    chart_series_t* series = nullptr;

    void create() {
      if (GRect::isCreated()) return;
      GRect::create();

      // กำหนดขนาดและลักษณะของการ์ดพื้นหลัง
      GRect::size(180, 130);
      GRect::color(TFT_COLOR_HEX(0x1E1E2E));
      GRect::radius(14);
      GRect::border(0);
      GRect::shadow(12, 3, TFT_BLACK);
      GRect::shadow_opa(100);
      GRect::shadow_offset(2, 4);

      // จัดระเบียบตาราง Layout ภายใน (3 แถว)
      // แถว 0: ข้อความหัวข้อ (Content)
      // แถว 1: พื้นที่แสดงกราฟเส้นเทรนด์สไตล์มินิมอล (ยืดหยุ่น FR1)
      // แถว 2: ตัวเลขค่าปัจจุบันขนานคู่กับหน่วยวัด (Content)
      GRect::GRID_CELL(
        GRID_COL{ GRID_FR(1), GRID_CONTENT },
        GRID_ROW{ GRID_CONTENT, GRID_FR(1), GRID_CONTENT }
      );
      GRect::padding(12, 10, 14, 14, 0, 10);

      // ── แถวที่ 0 — ข้อความหัวข้อ (Title) ──
      lb_title.parent(this);
      lb_title.font(prasanmit_20, TFT_COLOR_HEX(0x8888AA));
      lb_title = "Sensor";
      lb_title.grid_cell(0, 0, 2, 1, GRID_ALIGN_START, GRID_ALIGN_CENTER);

      // ── แถวที่ 1 — กราฟ Mini Trend (ตรงกลาง) ──
      chart.parent(this);
      chart.grid_cell(0, 1, 2, 1);
      chart.type(CHART_TYPE_LINE);
      chart.point_count(30);
      chart.range(0, 100);
      chart.bg_opa(0);
      chart.border(0);
      chart.line_opa(0);            // ซ่อนสไตล์ขอบทึบดั้งเดิม
      chart.div_line_count(0, 0);   // ไม่แสดงเส้นตารางใดๆ
      chart.clickable(false);
      chart.draw_grad(true);        // เปิดใช้งานฟังก์ชันไล่เฉดสีพื้นที่ใต้กราฟ
      
      series = chart.createSerie(TFT_COLOR_HEX(0x5E81F4));
      chart.line_width(2, GPART_ITEMS);
      chart.line_opa(220, GPART_ITEMS);

      // ── แถวที่ 2 — ตัวเลขและหน่วยวัดด้านล่าง ──
      lb_value.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value = "--";
      lb_value.grid_cell(0, 2, GRID_ALIGN_END, GRID_ALIGN_END);

      lb_unit.parent(this);
      lb_unit.font(prasanmit_20, TFT_COLOR_HEX(0x8888AA));
      lb_unit = "";
      lb_unit.grid_cell(1, 2, GRID_ALIGN_END, GRID_ALIGN_END);
    }

    // ── Public API สำหรับเรียกสั่งการจากภายนอก ──

    // กำหนดชื่อหัวข้อของเซนเซอร์
    void title(const char* t) {
      create();
      lb_title = t;
    }

    // กำหนดหน่วยของเซนเซอร์
    void unit(const char* u) {
      create();
      lb_unit = u;
    }

    // กำหนดขอบเขตต่ำสุด-สูงสุดของสเกลข้อมูลกราฟภายใน
    void range(int32_t vmin, int32_t vmax) {
      create();
      chart.range(vmin, vmax);
    }

    // กำหนดสีของเส้นกราฟเทรนด์
    void line_color(color_t c) {
      create();
      if (series != nullptr) {
        chart.series_color(series, c);
      }
    }

    // เพิ่มจุดข้อมูลใหม่เข้าสู่กราฟพร้อมแปลงค่าเป็นตัวอักษรแสดงผลที่หน้าจอหลัก
    void addValue(float v, int decimal = 1) {
      create();
      lb_value = String(v, decimal);
      if (series != nullptr) {
        chart.addPoint(series, (int32_t)v);
      }
    }
};

#endif // __CWC_SENSORCARD_H__