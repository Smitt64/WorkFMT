# -*- coding: utf-8 -*-
# Генерация BuildTrig/res/buildtrig.ico из геометрии res/buildtrig.svg
# Размеры: 16/24/32/48/64/128/256
import os
from PIL import Image, ImageDraw

BASE = 16.0
COLOR = (0x61, 0x2D, 0x53, 255)   # #612D53
WHITE = (255, 255, 255, 255)

# lightning polygon from SVG
BOLT = [(8.6, 4.5), (5.8, 8.6), (7.7, 8.6), (6.9, 11.5), (10.2, 7.2), (8.2, 7.2)]

SIZES = [16, 24, 32, 48, 64, 128, 256]
SS = 8  # supersampling factor


def draw_icon(size):
    scale = size / BASE
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    # outer rounded rect 16x16 r=3
    draw.rounded_rectangle([0, 0, size - 1, size - 1], radius=int(3 * scale), fill=COLOR)

    # inner white rect x=3 y=3 10x10 r=1
    draw.rounded_rectangle([round(3 * scale), round(3 * scale),
                            round(13 * scale) - 1, round(13 * scale) - 1],
                           radius=max(1, int(1 * scale)), fill=WHITE)

    # lightning bolt
    pts = [(round(x * scale), round(y * scale)) for (x, y) in BOLT]
    draw.polygon(pts, fill=COLOR)

    return img


def draw_icon_ss(size):
    # draw at larger canvas then downscale for smooth edges
    big = size * SS
    img = Image.new("RGBA", (big, big), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    s = big / BASE

    draw.rounded_rectangle([0, 0, big - 1, big - 1], radius=int(3 * s), fill=COLOR)
    draw.rounded_rectangle([round(3 * s), round(3 * s),
                            round(13 * s) - 1, round(13 * s) - 1],
                           radius=max(1, int(1 * s)), fill=WHITE)
    pts = [(x * s, y * s) for (x, y) in BOLT]
    draw.polygon(pts, fill=COLOR)

    return img.resize((size, size), Image.LANCZOS)


def main():
    out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "res")
    out_path = os.path.join(out_dir, "buildtrig.ico")

    images = []
    for size in SIZES:
        if size <= 48:
            img = draw_icon(size)      # pixel-perfect for small sizes
        else:
            img = draw_icon_ss(size)   # smooth for large sizes
        images.append(img)

    # save multi-size ico (largest first as base)
    images[-1].save(out_path, format="ICO",
                    sizes=[(s, s) for s in SIZES],
                    append_images=images[:-1])
    print("Saved:", out_path)


if __name__ == "__main__":
    main()