import argparse
import itertools
import struct

from PIL import Image

parser = argparse.ArgumentParser(description="View an .fbi file")
parser.add_argument("file", metavar="input", type=str, help="the input .fbi file")

args = parser.parse_args()

input_filename = args.file


pixels = []
width = 0
height = 0

with open(input_filename, "rb") as f:
    first_8 = f.read(8)
    width, height = struct.unpack('<LL', first_8)

    print(width, height)


    # width, height = img.size

    # for x, y in itertools.product(range(width), range(height)):
    #     r, g, b, a = img.getpixel((x, y))
    #     pixels.append(struct.pack("<BBBB", b, g, r, a))

# pixel_bytes = b"".join(pixels)
# pixel_byte_len = len(pixel_bytes)

# fbi_image = struct.pack("<LL{}s".format(pixel_byte_len), width, height, pixel_bytes)


# with open(output_filename, "wb") as f:
    # f.write(pixel_bytes)
