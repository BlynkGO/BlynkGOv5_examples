// ════════════════════════════════════════════════════════════════
// ASSET PREPARATION — รันคำสั่งเหล่านี้ใน folder src/ ก่อน build
// ════════════════════════════════════════════════════════════════
//
// [IMAGE]  1. รัน sprite_repack.py เพื่อ crop + repack ให้ tight:
//              python sprite_repack.py icons_education.png \
//                  --sprite 75 75 --stride 145 145 --grid 7 7
//          2. แปลงผลลัพธ์:
//              imgconv icons_education_repacked.png -t
//          → img_icons_education_repacked.c
//
// ════════════════════════════════════════════════════════════════

#include <BlynkGOv5.h>
#include "CWC_SpriteButton.h"

IMAGE_DECLARE(img_icons_education_repacked);

// SpriteSheet descriptor (repacked — tight, no gap)
//   original : c0=(72,72)  sprite=(75×75)  stride=(145×145)  grid=7×7
//   repacked : c0=(37,37)  sprite=(75×75)  stride=(75×75)  grid=7×7
SpriteSheet ss( 
  &img_icons_education_repacked, 
  75, 75,  // sprite size กว้าง, สูง  
  7, 7    // จน. cols, จน. rows
);

SpriteButton btn_dna(ss, 0);      // ใช้ sprite sheet id = 0
SpriteButton btn_calc(ss, 1);     // ใช้ sprite sheet id = 1
SpriteButton btn_pen(ss, 2);      // ใช้ sprite sheet id = 2

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_CYAN);

  // ─ btn_dna ──────────────────────────────────────────────────
  btn_dna.align(ALIGN_TOP_LEFT, 20, 60);
  btn_dna.onClicked(GWIDGET_CB {
    Serial.println("dna clicked");
  });

  // ─ btn_calc ───────────────────────────────────────────────────
  btn_calc.align(btn_dna, ALIGN_OUT_RIGHT_MID, 16, 0);
  btn_calc.onClicked(GWIDGET_CB {
    Serial.println("calc clicked");
  });

  // ─ btn_pen ─────────────────────────────────────────────────
  btn_pen.align(btn_calc, ALIGN_OUT_RIGHT_MID, 16, 0);
  btn_pen.disabled(true);
  btn_pen.onClicked(GWIDGET_CB {
    Serial.println("pen clicked");
  });
}

void loop() {
  BlynkGO.update();
}
