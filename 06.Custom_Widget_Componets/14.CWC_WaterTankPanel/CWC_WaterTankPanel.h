// ════════════════════════════════════════════════════════════════════
// CWC: WaterTankPanel
// File: CWC_WaterTankPanel.h
// Base: GRect
// Version: 1.2
// ────────────────────────────────────────────────────────────────────
// DESCRIPTION:
//   ห่อ WaterTank (GImage) + GScale (เส้นขีดบอกความสูง 0-300cm) ไว้ใน
//   container เดียว เพื่อให้จัด align/size ของ "ถัง+สเกล" ทั้งคู่พร้อมกัน
//   ได้ในคำสั่งเดียว — ใช้ API ตั้งค่า/อ่านค่าน้ำแบบเดิมทุกอย่าง
//   (range / operator= / toInt()) เพราะ forward ไปยัง WaterTank ภายใน
//
// INTERNAL STRUCTURE (absolute align):
//
//   this (GRect, container)         size(SIZE_CONTENT, SIZE_CONTENT)
//    ├─ lb_title (GLabel)            align(ALIGN_TOP, 0, 5)
//    ├─ tank  (WaterTank : GImage)   align(ALIGN_TOP_LEFT, 0, title_h + padding_row)
//    └─ scale (GScale)               align(tank, ALIGN_RIGHT, padding_column, 0)
//        range 0-300cm, 31 ขีด, ขีดหลักทุกๆ 5 ขีด
//
//   NOTE — unit แยกกัน:
//   scale  ใช้หน่วย cm  (0-300)  — บอกระดับน้ำในถัง
//   tank / range() / operator= ใช้หน่วย ลิตร — ปรับผ่าน range()
//
// DEPENDENCIES (run before build):
//   IMAGE: img_tank    // [imgconv] imgconv tank.png -t
//   IMAGE: mask_tank   // [imgconv] imgconv tank_mask.png -m -o mask_tank.c
//
// PUBLIC API:
//   void     title(String t)                     — กำหนด Title
//   void     range(int32_t min, int32_t max)     — กำหนดช่วงปริมาณน้ำ (ลิตร)
//   WaterTankPanel& operator=(int32_t v)          — ตั้งปริมาณน้ำ  เช่น tank1 = 3000;
//   int32_t  toInt()                              — อ่านปริมาณน้ำปัจจุบัน (ลิตร)
//   tank, scale                                   — public, เข้าถึง/ปรับแต่งเพิ่มได้ตรงๆ
//
// USAGE EXAMPLE:
//
//   // ── src/main.cpp ─────────────────────────────────
//   #include <BlynkGOv5.h>
//   #include "CWC_WaterTankPanel.h"
//
//   // ── Global declarations ──────────────────────────
//   WaterTankPanel tank1;
//   GSlider        slider;
//
//   // ── setup() ──────────────────────────────────────
//   void setup() {
//     Serial.begin(9600); Serial.println();
//     BlynkGO.begin();
//
//     tank1.title("ถังน้ำ 1");
//     tank1.range(0, 6000);            // เหมือน API เดิมทุกอย่าง
//     tank1 = 3000;
//
//     slider.width(500);
//     slider.align(ALIGN_BOTTOM, 0, -30);
//     slider.range(0, 6000);
//     slider.onValueChanged(GWIDGET_CB {
//       tank1 = slider.toInt();        // ตั้งค่าด้วย =
//       Serial.println(tank1.toInt()); // อ่านค่าด้วย .toInt()
//     });
//   }
//
//   void loop() { BlynkGO.update(); }
// ════════════════════════════════════════════════════════════════════

#pragma once
#include <BlynkGOv5.h>
#include "CWC_WaterTank.h"

class WaterTankPanel : public GRect {
  public:
    GLabel    lb_title;
    WaterTank tank;    // ① ภาพถัง+แท่งน้ำ (CWC ลูก)
    GScale    scale;   // ① เส้นขีดบอกความสูง (หน่วย cm, range 0-300 fixed)

    WaterTankPanel(GPARENT) : GRect(parent) {}

    void create() {
      if (GRect::isCreated()) return;     // ③ guard
      GRect::create();                    // ④ create container ก่อนเสมอ
      GRect::size(SIZE_CONTENT, SIZE_CONTENT);
      GRect::padding(5, 20, 30, 30, 5, 20);
      GRect::radius(10);
      GRect::color(TFT_COLOR_HEX(0x1E2230));
      GRect::font_color(TFT_COLOR_HEX(0x8A92A6));

      // ⑤ bind parent ของลูกทุกตัว — ทำที่นี่เท่านั้น (หลัง BlynkGO.begin())
      lb_title.parent(this);
      lb_title = "ถังน้ำ";

      tank.parent(this);

      scale.parent(this);
      scale.size(30, 160);
      scale.range(0, 300);                        // หน่วย cm — fixed ตาม visual ถัง
      scale.mode(SCALE_MODE_VERTICAL_RIGHT, 31, 5); // 31 ขีด, ขีดหลักทุกๆ 5 ขีด
      scale.tick_length(10, 5);
      scale.showLabel(true);
      scale.translate_x(5, GPART_INDICATOR);      // ช่องไฟระหว่าง scale กับ label
      scale.color(TFT_WHITE);
      scale.font_color(TFT_COLOR_HEX(0x8A92A6));

      this->refresh_alignment();

      GRect::onSizeChanged(GWIDGET_CB{
        if(widget->event_old_size() == widget->size()) return;
        WaterTankPanel* _wpanel = (WaterTankPanel*) widget;
        _wpanel->refresh_alignment();
      });
    }

    void title(String t) {
      create();
      lb_title = t;
    }

    // ⑦ public method เรียก create() เป็นบรรทัดแรกเสมอ — forward ไปยัง tank
    void range(int32_t min, int32_t max) {
      create();
      tank.range(min, max);
    }

    WaterTankPanel& operator=(int32_t v) {
      create();
      tank = v;
      return *this;
    }

    int32_t toInt() {
      create();
      return tank.toInt();
    }

  private:
    void refresh_alignment() {
      lb_title.align(ALIGN_TOP, 0, 5);
      tank.align(ALIGN_TOP_LEFT, 0, lb_title.font_height() + GRect::padding_row());
      scale.align(tank, ALIGN_RIGHT, GRect::padding_column(), 0);
    }
};