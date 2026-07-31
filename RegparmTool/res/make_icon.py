# -*- coding: utf-8 -*-
# Генератор иконки RegparmTool: res/regparmtool_icon.ico
# Дизайн соответствует res/regparmtool_icon.svg (стиль иконок DBFileTool/DiffToScript)
# Для окна приложения используется SVG напрямую (setWindowIcon), ICO нужен только для exe
import os
from PIL import Image, ImageDraw

BG = (168, 50, 50, 255)      # #a83232
FG = (255, 255, 255, 255)    # #ffffff

SUPERSAMPLE = 8
ICON_SIZES = [16, 24, 32, 48, 64, 256]

# Геометрия в координатах 16x16 (как в SVG)
CORNER_RADIUS = 3.0
LINE_Y = [5.0, 8.0, 11.0]
LINE_X0, LINE_X1 = 3.5, 12.5
LINE_W = 1.3
KNOBS = [(6.0, 5.0), (10.0, 8.0), (5.5, 11.0)]
KNOB_R = 1.8
HOLE_R = 0.8


def render(size):
    s = size * SUPERSAMPLE
    k = s / 16.0
    img = Image.new("RGBA", (s, s), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)

    d.rounded_rectangle([0, 0, s - 1, s - 1], radius=CORNER_RADIUS * k, fill=BG)

    for y in LINE_Y:
        d.line([LINE_X0 * k, y * k, LINE_X1 * k, y * k], fill=FG, width=max(1, round(LINE_W * k)))

    for cx, cy in KNOBS:
        r = KNOB_R * k
        d.ellipse([cx * k - r, cy * k - r, cx * k + r, cy * k + r], fill=FG)
        r = HOLE_R * k
        d.ellipse([cx * k - r, cy * k - r, cx * k + r, cy * k + r], fill=BG)

    return img.resize((size, size), Image.LANCZOS)


def main():
    here = os.path.dirname(os.path.abspath(__file__))

    ico_path = os.path.join(here, "regparmtool_icon.ico")
    # ICO сохраняем из самого большого рендера: Pillow сам даунскейлит до sizes
    render(max(ICON_SIZES)).save(ico_path, format="ICO",
                                 sizes=[(s, s) for s in ICON_SIZES])
    print("written:", ico_path)


if __name__ == "__main__":
    main()
