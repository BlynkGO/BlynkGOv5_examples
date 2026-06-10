// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION
// ════════════════════════════════════════════════════════════════
// ไม่มี asset เพิ่มเติม ใช้เฉพาะ built-in fonts
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_SensorCard.h"

#define CARD_COUNT 12  // ปรับเพิ่มจำนวนการ์ดเป็น 12 ช่อง (3 คอลัมน์ x 4 แถว)

// ─────────────────────────────────────────────────────────────────
//  Global widgets
// ─────────────────────────────────────────────────────────────────
GRect rect_bg;
  GLabel lb_header(rect_bg);
  GPage page_cards(rect_bg); // ใช้ GPage สำหรับเลื่อน Scroll 上下 อย่างลื่นไหล
  
  // ประกาศอาร์เรย์ของ SensorCard จำนวน 12 ช่อง (สไตล์ Lazy Code)
  SensorCard cards[CARD_COUNT] = {
    SensorCard(page_cards), SensorCard(page_cards), SensorCard(page_cards),
    SensorCard(page_cards), SensorCard(page_cards), SensorCard(page_cards),
    SensorCard(page_cards), SensorCard(page_cards), SensorCard(page_cards),
    SensorCard(page_cards), SensorCard(page_cards), SensorCard(page_cards)
  };

void setup() {
  BlynkGO.begin();

  // ── Background ──
  rect_bg.color(TFT_COLOR_HEX(0x111122));
  rect_bg.size(PCT(100), PCT(100));
  rect_bg.GRID_CELL(
    GRID_COL{ GRID_FR(1) },
    GRID_ROW{ GRID_CONTENT, GRID_FR(1) }
  );
  rect_bg.padding(10, 10, 10, 10, 0);

  // ── Header ──
  lb_header.font(prasanmit_35, TFT_WHITE);
  lb_header = "Sensor Dashboard (3x4)";
  lb_header.grid_cell(0, 0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

  // ── GPage Setup (สกรอลล์พื้นที่แสดงการ์ด) ──
  page_cards.grid_cell(0, 1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  page_cards.bg_opa(0);       // โปร่งแสงทะลุเห็นพื้นหลังหลัก
  page_cards.border(0);       // ลบขอบหน้าต่างออก
  
  // ออกแบบ Grid Layout ของ GPage เป็นแบบ 3 คอลัมน์ x 4 แถว (ใช้ GRID_CONTENT เพื่อให้แถวขยายดันสกรอลล์)
  page_cards.GRID_CELL(
    GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1) }, 
    GRID_ROW{ GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT }
  );
  page_cards.padding(10, 10, 10, 10, 10); // เว้นช่องไฟระหว่างขอบและการ์ด 10px
  page_cards.scrollbar(SCROLLBAR_ON, false, true);  // มีscrollbar แนวนอนไม่ต้อง มีแนวตั้ง
  page_cards.opa(40, GPART_SCROLLBAR);      // ให้เห็น scrollbar จางๆ

  // ── ข้อมูลเซนเซอร์ 12 ช่อง ──
  const char* titles[CARD_COUNT] = {
    "Temp 1",  "Humid 1", "CO2",
    "PM2.5",   "Volt In", "Current",
    "Power",   "Temp 2",  "Humid 2",
    "Press",   "Lux",     "Battery"   // เพิ่มเซนเซอร์ใหม่อีก 3 ตัวในแถวที่ 4
  };

  const char* units[CARD_COUNT] = {
    "°C", "%",   "ppm",
    "µg", "V",   "A",
    "W",  "°C",  "%",
    "hPa", "lx", "%"
  };

  color_t colors[CARD_COUNT] = {
    TFT_COLOR_HEX(0xFF6B6B), TFT_COLOR_HEX(0x4ECDC4), TFT_COLOR_HEX(0xFFD93D),
    TFT_COLOR_HEX(0xA8E6CF), TFT_COLOR_HEX(0x60A5FA), TFT_COLOR_HEX(0xF472B6),
    TFT_COLOR_HEX(0x34D399), TFT_COLOR_HEX(0xFF8B8B), TFT_COLOR_HEX(0x38BDF8),
    TFT_COLOR_HEX(0xFB923C), TFT_COLOR_HEX(0xFCD34D), TFT_COLOR_HEX(0xC084FC) // สีของเซนเซอร์ใหม่
  };

  // ── วนลูปจัดตำแหน่งและตั้งค่าสไตล์ ──
  for (int i = 0; i < CARD_COUNT; i++) {
    int col = i % 3; // คอลัมน์ 0, 1, 2
    int row = i / 3; // แถว 0, 1, 2, 3

    cards[i].grid_cell(col, row);
    cards[i].title(titles[i]);
    cards[i].unit(units[i]);
    cards[i].line_color(colors[i]);
    
    // ตั้งโครงช่วงกราฟเฉพาะกลุ่มตามประเภทความเหมาะสม
    if (i == 2)       cards[i].range(400, 2000); // CO2
    else if (i == 4)  cards[i].range(0, 250);    // Volt
    else if (i == 6)  cards[i].range(0, 1000);   // Power
    else if (i == 9)  cards[i].range(900, 1100); // Pressure (hPa)
    else if (i == 10) cards[i].range(0, 5000);   // Lux
    else              cards[i].range(0, 100);    // กลุ่มอุณหภูมิ, ความชื้น, แบตเตอรี่ ทั่วไป
  }

  // ── Simulate sensor data ──
  static SoftTimer timer;
  timer.setInterval(1000, []() { // ประมวลผลและอัปเดตข้อมูลกราฟทุกๆ 1 วินาที
    for (int i = 0; i < CARD_COUNT; i++) {
      float base_val = 50.0;
      int decimal = 1;

      // จัดการค่าฐานและจุดทศนิยมของข้อมูลจำลองแยกตามประเภท
      if (i == 2)       { base_val = 800.0;  decimal = 0; }
      else if (i == 4)  { base_val = 220.0;  decimal = 1; }
      else if (i == 5)  { base_val = 5.4;    decimal = 2; }
      else if (i == 6)  { base_val = 350.0;  decimal = 0; }
      else if (i == 9)  { base_val = 1013.0; decimal = 0; } // Pressure
      else if (i == 10) { base_val = 350.0;  decimal = 0; } // Lux
      else if (i == 11) { base_val = 98.0;   decimal = 0; } // Battery

      float rand_val = base_val + (random(-50, 50) / 10.0);
      
      // ปรับแต่งสเกลสุ่มเพิ่มเฉพาะบางตัวเพื่อให้เห็นแนวโน้มกราฟชัดเจนขึ้น
      if(i == 10) rand_val = base_val + random(-100, 300); 

      cards[i].addValue(rand_val, decimal);
    }
  }, true);
}

void loop() {
  BlynkGO.update();
}