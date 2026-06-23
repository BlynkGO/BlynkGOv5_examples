#pragma once
#include <BlynkGOv5.h>

// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION  
// ════════════════════════════════════════════════════════════════
//  ** หา icon pack ทำ sprite sheet ได้จาก https://www.flaticon.com/packs เป็นต้น **
//
// ── ขั้นตอนเตรียม sprite sheet ก่อน build ──────────────────────
//
// 1. เตรียม sprite sheet ต้นฉบับ (PNG โปร่งใส)
//
// 2. [ถ้า sheet มี gap/padding] repack ให้ tight ก่อน:
//       python sprite_repack.py car_spritesheet.png \
//           --sprite 80 80 --stride 145 145 --grid 7 5
//    → ได้ car_spritesheet_repacked.png  (ประหยัด Flash ~67%)
//    → ได้ car_spritesheet_repacked.h    (descriptor C++ พร้อมใช้)
//
//    ถ้า origin sprite id=0 ไม่ได้อยู่ตรงกลาง stride ให้ระบุ --c0:
//       python sprite_repack.py car_spritesheet.png \
//           --c0 72 72 --sprite 80 80 --stride 145 145 --grid 7 5
//
// 3. แปลง PNG → BlynkGO image:
//       imgconv car_spritesheet_repacked.png -t
//    → ได้ img_car_spritesheet_repacked.c ใน src/
//
// 4. copy descriptor จาก .h ที่ได้ใส่ main.cpp แล้ว build ได้เลย
//
// ════════════════════════════════════════════════════════════════
// SpriteSheet — Descriptor
// ════════════════════════════════════════════════════════════════
//
//   sheet              : img_t ของ sprite sheet (PNG โปร่งใส)
//   c0x, c0y           : center point ของ sprite id=0 บน sheet (pixel)
//   sx,  sy            : ขนาด sprite 1 ช่อง — กว้าง × สูง (pixel)
//   stride_w, stride_h : ระยะ center-to-center แต่ละ sprite (pixel)
//   cols, rows         : จำนวน sprite ต่อแถว × จำนวนแถว
//
// sprite_id ไล่ 0 → (cols×rows − 1) แบบ row-major:
//
//   id= 0  id= 1  id= 2  ...  id=cols-1
//   id=cols  ...
//
// สูตร offset:
//   col   = sprite_id % cols
//   row   = sprite_id / cols
//   off_x = c0x + col * stride_w - sx/2
//   off_y = c0y + row * stride_h - sy/2
//   → GImage::offset(-off_x, -off_y)
// ────────────────────────────────────────────────────────────────

struct SpriteSheet {
  const img_t* sheet  = nullptr;
  int32_t c0x         = 0;
  int32_t c0y         = 0;
  int32_t sx          = 0;
  int32_t sy          = 0;
  int32_t stride_w    = 0;
  int32_t stride_h    = 0;
  int32_t cols        = 1;
  int32_t rows        = 1;

  SpriteSheet() = default;

  SpriteSheet(const img_t* _sheet,
              int32_t _c0x,      int32_t _c0y,
              int32_t _sx,       int32_t _sy,
              int32_t _stride_w, int32_t _stride_h,
              int32_t _cols,     int32_t _rows)
    : sheet(_sheet),
      c0x(_c0x),           c0y(_c0y),
      sx(_sx),             sy(_sy),
      stride_w(_stride_w), stride_h(_stride_h),
      cols(_cols),         rows(_rows) {}

  SpriteSheet(const img_t* _sheet,
              int32_t _sx,       int32_t _sy,
              int32_t _cols,     int32_t _rows)
    : sheet(_sheet),
      c0x(_sx/2),           c0y(_sy/2),
      sx(_sx),             sy(_sy),
      stride_w(_sx), stride_h(_sy),
      cols(_cols),         rows(_rows) {}

  void offsetOf(int32_t sprite_id, int32_t& out_ox, int32_t& out_oy) const {
    if (cols <= 0) { out_ox = 0; out_oy = 0; return; }
    int32_t col = sprite_id % cols;
    int32_t row = sprite_id / cols;
    out_ox = -(c0x + col * stride_w - sx / 2);
    out_oy = -(c0y + row * stride_h - sy / 2);
  }

  int32_t total() const { return cols * rows; }
};


// ════════════════════════════════════════════════════════════════
// SpriteButton : GImage  (CWC)
// ════════════════════════════════════════════════════════════════
//
// CWC ปุ่มรูปภาพที่แตก sprite จาก SpriteSheet เดียว
// ใช้ GImage เป็น base — inherits ทุก method ของ GImage ได้ทันที
// (recolor, opa, align, size, onClicked, onPressed, disabled ฯลฯ)
//
// LAZY CREATION PATTERN:
//   - constructor รับ parent เท่านั้น ไม่ตั้งค่าใดๆ
//   - spritesheet() เรียก create() บรรทัดแรกเสมอ
//   - sprite_id()   เรียก create() บรรทัดแรกเสมอ
//
// ── การใช้งาน ────────────────────────────────────────────────
//
//   // ① ประกาศ SpriteSheet (แชร์ได้ทุก instance)
//   SpriteSheet ss(&img_spritesheet,
//                  c0x, c0y,           // center ของ sprite id=0
//                  sx, sy,             // ขนาด sprite 1 ช่อง
//                  stride_w, stride_h, // ระยะ center-to-center
//                  cols, rows);
//
//   หรือ ถ้า sprite w,h == stride w,h  (ภาพ sprite sheet ไม่มี gap)
//   SpriteSheet ss(&img_spritesheet,
//                  sx, sy,             // ขนาด sprite 1 ช่อง
//                  cols, rows);
//
//   // ② ประกาศ global (default parent = GScreen)
//   SpriteButton btn_pump;
//   SpriteButton btn_fan;
//
//   // ③ setup ใน setup()
//   btn_pump.spritesheet(ss, 0);           // bind sheet + เลือก sprite id=0
//   btn_pump.align(ALIGN_CENTER);
//
//   // ④ เปลี่ยน sprite ภายหลัง
//   btn_pump.sprite_id(3);
//
//   // ⑤ ผูก event / style เพิ่มเติม (ทุก GImage method ใช้ได้)
//   btn_pump.onClicked(GWIDGET_CB { ... });
//   btn_fan.recolor(TFT_RED, 200);
//   btn_fan.recolor(TFT_COLOR_MIX(TFT_RED, TFT_BLACK, 80), 210, GSTATE_PRESSED);
//
// ── Built-in visual feedback ─────────────────────────────────
//   GSTATE_PRESSED  → recolor black opa=80  (tint อัตโนมัติ)
//   GSTATE_DISABLED → opa=120               (หรี่อัตโนมัติ)
// ────────────────────────────────────────────────────────────────

class SpriteButton : public GImage {
public:

  SpriteButton(GPARENT) : GImage(parent) {}
  SpriteButton(const SpriteSheet& ss, int32_t id = 0, GPARENT) : GImage(parent), _ss(&ss), _id(id)  {}

  void create(){
    if(GImage::isCreated()) return;
    GImage::create();
    if(_ss != NULL ) this->spritesheet(*this->_ss, this->_id);
  }

  // bind SpriteSheet และแสดง sprite id เริ่มต้น
  void spritesheet(const SpriteSheet& ss, int32_t id = 0) {
    create();
    _ss = &ss;
    _id = id;

    GImage::size(_ss->sx, _ss->sy);
    GImage::setImage(_ss->sheet);
    GImage::image_align(IMAGE_ALIGN_TILE);
    GImage::clickable(true);
    GImage::recolor(TFT_BLACK, 80, GSTATE_PRESSED);
    GImage::opa(120, GSTATE_DISABLED);

    _applySprite(_id);
  }

  // เปลี่ยน sprite ที่แสดง
  void sprite_id(int32_t sprite_id) {
    create();
    _applySprite(sprite_id);
  }

private:

  const SpriteSheet* _ss = nullptr;
  int32_t            _id = 0;

  void _applySprite(int32_t sprite_id) {
    if (!_ss || !GImage::isCreated()) return;
    int32_t ox, oy;
    sprite_id = constrain(sprite_id, 0, _ss->total()-1);
    _id = sprite_id;
    _ss->offsetOf(sprite_id, ox, oy);
    GImage::offset(ox, oy);
  }
};