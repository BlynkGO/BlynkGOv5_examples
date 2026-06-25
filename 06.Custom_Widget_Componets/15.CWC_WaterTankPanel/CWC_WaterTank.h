// ════════════════════════════════════════════════════════════════════
// CWC: WaterTank
// File: CWC_WaterTank.h
// Base: GImage
// Version: 3.1
// ────────────────────────────────────────────────────────────────────
// DESCRIPTION:
//   แสดงถังน้ำแบบกราฟิก (ภาพถัง + mask ตัดทรงถัง + แท่งสีน้ำที่ขยับตาม
//   ระดับ) — ตัว CWC เองคือภาพถัง (inherit จาก GImage ตรงๆ ไม่ห่อด้วย
//   GRect แล้ว) ตั้งค่าระดับน้ำผ่าน operator= และอ่านค่ากลับด้วย .toInt()
//
// INTERNAL STRUCTURE (absolute align — ไม่ใช้ GRID เพราะเป็นภาพกราฟิก):
//
//   this (GImage = ภาพถัง)
//    └─ m_tank (GMask)   ตัด mask ทรงถัง
//        └─ rect_tank (GRect) แท่งน้ำสีฟ้า  align(ALIGN_BOTTOM)
//
// DEPENDENCIES (run before build):
//   IMAGE: img_tank    // [imgconv] imgconv tank.png -t
//   IMAGE: mask_tank   // [imgconv] imgconv tank_mask.png -m -o mask_tank.c
//
// PUBLIC API:
//   void     range(int32_t min, int32_t max)  — กำหนดช่วงปริมาณน้ำ (ลิตร)
//   WaterTank& operator=(int32_t v)            — ตั้งปริมาณน้ำ  เช่น tank1 = 3000;
//   int32_t  toInt()                           — อ่านปริมาณน้ำปัจจุบัน (ลิตร)
//
// USAGE EXAMPLE:
//
//   // ── src/main.cpp ─────────────────────────────────
//   #include <BlynkGOv5.h>
//   #include "CWC_WaterTank.h"
//
//   // ── Global declarations ──────────────────────────
//   WaterTank tank1;
//   GSlider   slider;
//
//   // ── setup() ──────────────────────────────────────
//   void setup() {
//     Serial.begin(9600); Serial.println();
//     BlynkGO.begin();
//
//     tank1.range(0, 6000);
//
//     slider.width(500);
//     slider.align(ALIGN_BOTTOM, 0, -30);
//     slider.range(0, 6000);
//     slider.onValueChanged(GWIDGET_CB {
//       tank1 = slider.toInt();             // ตั้งค่าด้วย =
//       Serial.println(tank1.toInt());      // อ่านค่าด้วย .toInt()
//     });
//   }
//
//   void loop() { BlynkGO.update(); }
// ════════════════════════════════════════════════════════════════════

#pragma once
#include <BlynkGOv5.h>

class WaterTank : public GImage {
  public:
    WaterTank(GPARENT) : GImage(parent)   {}

    void create() {
      IMAGE_DECLARE(img_tank);            // 220x220px
      IMAGE_DECLARE(mask_tank);           // 220x220px

      if (GImage::isCreated()) return;   // ③ guard
      GImage::create();                  // ④ create ตัวฐาน (ภาพถัง) ก่อนเสมอ
      GImage::setImage(img_tank);

      // ⑤ bind parent ของลูกทุกตัว — ทำที่นี่เท่านั้น (หลัง BlynkGO.begin())
      m_tank.parent(this);
      m_tank = mask_tank;

      rect_tank.parent(m_tank);
      rect_tank.size(220, 180);
      rect_tank.align(ALIGN_BOTTOM);
      rect_tank.color(TFT_BLUE);
      rect_tank.bg_opa(255);
    }

    // ⑦ public method เรียก create() เป็นบรรทัดแรกเสมอ
    void range(int32_t min, int32_t max) {
      create();
      _min = min;
      _max = max;
      _refresh();
    }

    // ตั้งค่าระดับน้ำแบบ tank1 = 3000;
    WaterTank& operator=(int32_t v) {
      create();
      _value = v;
      _refresh();
      return *this;
    }

    // อ่านค่าระดับน้ำปัจจุบัน
    int32_t toInt() {
      create();
      return _value;
    }

  private:
    GMask m_tank;       // mask ตัดทรงถัง — ต้องใส่ source ตอน construct (ไม่มี setter)
    GRect rect_tank;    // แท่งน้ำสีฟ้าที่ขยับความสูงตามค่า _value

    int32_t _min = 0;
    int32_t _max = 6000;
    int32_t _value = 0;

    static constexpr int32_t FILL_MIN_PX = 28;    // ความสูงขั้นต่ำของแท่งน้ำ (px)
    static constexpr int32_t FILL_MAX_PX = 160;   // ความสูงที่เพิ่มได้สูงสุด (px)

    void _refresh() {
      int32_t h_px = map(_value, _min, _max, 0, FILL_MAX_PX);
      rect_tank.height(FILL_MIN_PX + h_px);
      rect_tank.align(ALIGN_BOTTOM);
    }
};