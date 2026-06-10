"""
sprite_repack.py
────────────────────────────────────────────────────────────────
รับ sprite sheet ต้นฉบับที่มี gap/padding แล้ว crop แต่ละ sprite
ตาม descriptor (c0x, c0y, sx, sy, stride_w, stride_h, cols, rows)
จากนั้น repack ใหม่แบบ tight (ไม่มี gap) → ประหยัด Flash

Output:
  <input_name>_repacked.png   ← sprite sheet ใหม่ ชิดกันพอดี
  <input_name>_repacked.h     ← SpriteSheet descriptor สำหรับ C++ พร้อมใช้

ใช้งาน:
  python sprite_repack.py <sheet.png> [options]

ตัวอย่าง (ค่าจากโค้ดจริง):
  python sprite_repack.py car_spritesheet.png \\
      --c0 72 72 --sprite 80 80 --stride 145 145 --grid 7 5
"""

import argparse
import os
from PIL import Image


def repack(sheet_path, c0x, c0y, sx, sy, stride_w, stride_h, cols, rows):
    sheet = Image.open(sheet_path).convert("RGBA")

    sprites = []
    for row in range(rows):
        for col in range(cols):
            # center ของ sprite นี้บน sheet ต้นฉบับ
            cx = c0x + col * stride_w
            cy = c0y + row * stride_h

            # crop box จาก center ± half-size
            left   = cx - sx // 2
            top    = cy - sy // 2
            right  = left + sx
            bottom = top  + sy

            sprite = sheet.crop((left, top, right, bottom))
            sprites.append(sprite)

    total = cols * rows
    assert len(sprites) == total

    # repack tight — ไม่มี gap เลย
    out_w = sx * cols
    out_h = sy * rows
    out   = Image.new("RGBA", (out_w, out_h), (0, 0, 0, 0))

    for idx, sprite in enumerate(sprites):
        col = idx % cols
        row = idx // cols
        out.paste(sprite, (col * sx, row * sy))

    # save PNG
    base     = os.path.splitext(sheet_path)[0]
    out_png  = base + "_repacked.png"
    out.save(out_png)
    print(f"[OK] saved: {out_png}  ({out_w}×{out_h} px)")

    # ── คำนวณ descriptor ใหม่ ────────────────────────────────────
    # tight pack → c0x = sx/2, c0y = sy/2, stride = sx/sy
    new_c0x      = sx // 2
    new_c0y      = sy // 2
    new_stride_w = sx
    new_stride_h = sy

    # savings
    orig_w = c0x + (cols - 1) * stride_w + sx // 2  # approx original width used
    orig_h = c0y + (rows - 1) * stride_h + sy // 2
    orig_size = orig_w * orig_h
    new_size  = out_w  * out_h
    saving_pct = (1 - new_size / orig_size) * 100 if orig_size > 0 else 0

    print(f"[OK] original area : ~{orig_w}×{orig_h} = {orig_w*orig_h:,} px²")
    print(f"[OK] repacked area :  {out_w}×{out_h} = {out_w*out_h:,} px²")
    print(f"[OK] area saving   :  ~{saving_pct:.1f}%")

    # ── generate .h descriptor ───────────────────────────────────
    img_name = os.path.basename(base) + "_repacked"
    out_h_path = base + "_repacked.h"

    h_content = f"""\
#include <BlynkGOv5.h>

// ── [IMAGE] ──────────────────────────────────────────────────
// imgconv {img_name}.png -t
// ─────────────────────────────────────────────────────────────
IMAGE_DECLARE(img_{img_name});

// SpriteSheet descriptor (repacked — tight, no gap)
//   original : c0=({c0x},{c0y})  sprite=({sx}×{sy})  stride=({stride_w}×{stride_h})  grid={cols}×{rows}
//   repacked : c0=({new_c0x},{new_c0y})  sprite=({sx}×{sy})  stride=({new_stride_w}×{new_stride_h})  grid={cols}×{rows}
SpriteSheet ss_repacked(
  &img_{img_name},
  {new_c0x}, {new_c0y},       // c0x, c0y  — center of sprite id=0
  {sx}, {sy},                 // sx,  sy   — sprite size
  {new_stride_w}, {new_stride_h},   // stride_w, stride_h
  {cols}, {rows}              // cols, rows
);
"""
    with open(out_h_path, "w") as f:
        f.write(h_content)
    print(f"[OK] saved: {out_h_path}")
    print()
    print("── C++ descriptor ──────────────────────────────────────────")
    print(h_content)


def main():
    parser = argparse.ArgumentParser(description="Repack sprite sheet (crop + tight pack)")
    parser.add_argument("sheet",               help="input sprite sheet PNG")
    parser.add_argument("--c0",   nargs=2, type=int, metavar=("C0X","C0Y"),
                        default=None,          help="center of sprite id=0  e.g. --c0 72 72  (default: stride/2)")
    parser.add_argument("--sprite", nargs=2, type=int, metavar=("SX","SY"),
                        required=True,         help="sprite size  e.g. --sprite 80 80")
    parser.add_argument("--stride", nargs=2, type=int, metavar=("SW","SH"),
                        required=True,         help="stride center-to-center  e.g. --stride 145 145")
    parser.add_argument("--grid",   nargs=2, type=int, metavar=("COLS","ROWS"),
                        required=True,         help="grid size  e.g. --grid 7 5")
    args = parser.parse_args()

    c0x = args.c0[0] if args.c0 else args.stride[0] // 2
    c0y = args.c0[1] if args.c0 else args.stride[1] // 2

    repack(
        sheet_path = args.sheet,
        c0x        = c0x,
        c0y        = c0y,
        sx         = args.sprite[0],
        sy         = args.sprite[1],
        stride_w   = args.stride[0],
        stride_h   = args.stride[1],
        cols       = args.grid[0],
        rows       = args.grid[1],
    )


if __name__ == "__main__":
    main()
