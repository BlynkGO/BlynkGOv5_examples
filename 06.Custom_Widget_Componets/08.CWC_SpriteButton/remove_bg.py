#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
remove_bg.py
────────────────────────────────────────────────────────────

Auto remove background from image / sprite sheet

DEFAULT MODE:
  - remove เฉพาะ background ที่เชื่อมกับขอบภาพ
  - preserve สีเดียวกันภายใน icon

OPTION:
  -a / --all
      remove ทุก pixel ที่สีใกล้กับ bg
      (รวมสีภายใน icon ด้วย)

ติดตั้ง:
    pip install pillow

────────────────────────────────────────────────────────────
ตัวอย่าง:

# remove เฉพาะ bg รอบนอก
python remove_bg.py icons.png

# remove ทุกสีเดียวกับ bg
python remove_bg.py icons.png -a

# output custom
python remove_bg.py icons.png -o clean.png

# tolerance เพิ่ม
python remove_bg.py icons.png --tol 30

────────────────────────────────────────────────────────────
"""

import argparse
from collections import Counter, deque
from pathlib import Path

from PIL import Image


# ──────────────────────────────────────────────────────────
# helper
# ──────────────────────────────────────────────────────────

def similar(c1, c2, tol):
    return (
        abs(c1[0] - c2[0]) <= tol and
        abs(c1[1] - c2[1]) <= tol and
        abs(c1[2] - c2[2]) <= tol
    )


def detect_background_color(img):

    w, h = img.size
    pix = img.load()

    colors = []

    # top / bottom
    for x in range(w):
        colors.append(pix[x, 0][:3])
        colors.append(pix[x, h - 1][:3])

    # left / right
    for y in range(h):
        colors.append(pix[0, y][:3])
        colors.append(pix[w - 1, y][:3])

    bg = Counter(colors).most_common(1)[0][0]

    return bg


# ──────────────────────────────────────────────────────────
# MODE 1 : remove only connected background
# ──────────────────────────────────────────────────────────

def remove_connected_background(img, bg_color, tol):

    w, h = img.size
    pix = img.load()

    visited = set()
    q = deque()

    # push border pixels
    for x in range(w):
        q.append((x, 0))
        q.append((x, h - 1))

    for y in range(h):
        q.append((0, y))
        q.append((w - 1, y))

    while q:

        x, y = q.popleft()

        if (x, y) in visited:
            continue

        visited.add((x, y))

        r, g, b, a = pix[x, y]

        if similar((r, g, b), bg_color, tol):

            # transparent
            pix[x, y] = (255, 255, 255, 0)

            # neighbors
            neighbors = [
                (x + 1, y),
                (x - 1, y),
                (x, y + 1),
                (x, y - 1),
            ]

            for nx, ny in neighbors:
                if 0 <= nx < w and 0 <= ny < h:
                    q.append((nx, ny))

    return img


# ──────────────────────────────────────────────────────────
# MODE 2 : remove ALL matching colors
# ──────────────────────────────────────────────────────────

def remove_all_background(img, bg_color, tol):

    w, h = img.size
    pix = img.load()

    for y in range(h):
        for x in range(w):

            r, g, b, a = pix[x, y]

            if similar((r, g, b), bg_color, tol):
                pix[x, y] = (255, 255, 255, 0)

    return img


# ──────────────────────────────────────────────────────────
# main
# ──────────────────────────────────────────────────────────

def main():

    parser = argparse.ArgumentParser(
        description="Auto remove background from image"
    )

    parser.add_argument(
        "input",
        help="input image file"
    )

    parser.add_argument(
        "-o",
        "--output",
        help="output png filename"
    )

    parser.add_argument(
        "--tol",
        type=int,
        default=20,
        help="color tolerance (default: 20)"
    )

    parser.add_argument(
        "-a",
        "--all",
        action="store_true",
        help="remove ALL pixels matching bg color"
    )

    args = parser.parse_args()

    input_path = Path(args.input)

    if not input_path.exists():
        print(f"[ERROR] file not found: {input_path}")
        return

    # auto output
    if args.output:
        output_path = Path(args.output)
    else:
        suffix = "_alpha" if args.all else "_nobg"

        output_path = input_path.with_name(
            input_path.stem + suffix + ".png"
        )

    # load
    img = Image.open(input_path).convert("RGBA")

    # detect bg
    bg = detect_background_color(img)

    print("────────────────────────────────────")
    print(f"Input      : {input_path}")
    print(f"Output     : {output_path}")
    print(f"Detected BG: {bg}")
    print(f"Tolerance  : {args.tol}")

    if args.all:
        print("Mode       : REMOVE ALL MATCHING COLORS")
    else:
        print("Mode       : REMOVE CONNECTED BACKGROUND")

    print("────────────────────────────────────")

    # process
    if args.all:
        img = remove_all_background(
            img,
            bg,
            args.tol
        )
    else:
        img = remove_connected_background(
            img,
            bg,
            args.tol
        )

    # save
    img.save(output_path)

    print("[DONE]")


if __name__ == "__main__":
    main()