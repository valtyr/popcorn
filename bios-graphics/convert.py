import argparse
import itertools
import struct

from PIL import Image

parser = argparse.ArgumentParser(
    description="Convert .png to .pbm to include in kernel binary"
)
parser.add_argument(
    "input_file", metavar="input", type=str, nargs=1, help="the input .png file"
)
parser.add_argument(
    "output_path",
    metavar="output",
    type=str,
    nargs=1,
    help="the path to the output .fbi file",
)

args = parser.parse_args()

input_filename = args.input_file[0]
output_filename = args.output_path[0]


pixels = []
width = 0
height = 0

with open(input_filename, "rb") as f:
    img = Image.open(f)
    width, height = img.size

    for x in range(width):
        for y in range(height):
            r, g, b, a = img.getpixel((y, x))
            pixels.append(struct.pack("<BBBB", b, g, r, a))

pixel_bytes = b"".join(pixels)
pixel_byte_len = len(pixel_bytes)

print(len(pixel_bytes))

fbi_image = struct.pack("<LL{}s".format(pixel_byte_len), height, width, pixel_bytes)


with open(output_filename, "wb") as f:
    f.write(fbi_image)
    # f.write(struct.pack("<LL", 0, 0))
