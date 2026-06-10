#include <BlynkGOv5.h>

// ── [IMAGE] ──────────────────────────────────────────────────
// imgconv icons_education_repacked.png -t
// ─────────────────────────────────────────────────────────────
IMAGE_DECLARE(img_icons_education_repacked);

// SpriteSheet descriptor (repacked — tight, no gap)
//   original : c0=(72,72)  sprite=(75×75)  stride=(145×145)  grid=7×7
//   repacked : c0=(37,37)  sprite=(75×75)  stride=(75×75)  grid=7×7
SpriteSheet ss_repacked(
  &img_icons_education_repacked,
  37, 37,       // c0x, c0y  — center of sprite id=0
  75, 75,                 // sx,  sy   — sprite size
  75, 75,   // stride_w, stride_h
  7, 7              // cols, rows
);
