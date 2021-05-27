#! /usr/local/bin/python3

import sys
import pathlib
import itertools

from PIL import Image
from tqdm import tqdm

from palette import closest_color

if len(sys.argv) < 2:
    print("A filename must be provided")
    sys.exit(1)

filename = sys.argv[1]

im = Image.open(filename)
print(im.format, im.size, im.mode)

im = im.convert("RGB")
w, h = im.size

path = pathlib.Path(filename)
print(path.name)


pixels = []

for x, y in tqdm(itertools.product(range(w), range(h))):
    rgb = im.getpixel((x, y))
    closest = closest_color(rgb)
    pixels.append(closest)

with open(f"bin/{path.name}.bin", "wb") as f:
    f.write(bytes(pixels))

print(f"Wrote bin/{path.name}.bin")
