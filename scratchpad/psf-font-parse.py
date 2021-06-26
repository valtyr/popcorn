import math
import glob

from struct import Struct
from collections import namedtuple
from itertools import tee
from typing import List, Iterable

from PIL import Image, ImageDraw


class FriendlyStruct(Struct):
    def __init__(self, name, fmt, labels):
        super(FriendlyStruct, self).__init__(fmt)
        self.returnTuple = namedtuple(name, labels)

    def unpack(self, __buffer):
        result = super(FriendlyStruct, self).unpack(__buffer)
        return self.returnTuple(*result)


header_struct = FriendlyStruct(
    "PSF2Header",
    "<LIIIIIII",
    (
        "magic",
        "version",
        "headerSize",
        "flags",
        "length",
        "charsize",
        "height",
        "width",
    ),
)


MAGIC = 0x864AB572


def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i : i + n]


def bitwise_idx(bytestring, idx):
    outer = idx // 8
    inner = idx % 8
    return bytestring[outer] & (0x1 << inner)


def parse_unicode_table(bytestring: bytes):
    tail = bytestring
    while tail:
        # Get next description
        desc, _, tail = tail.partition(b"\xFF")
        yield list(desc.decode())


def flip_unicode_table(table: Iterable[List[str]]) -> dict[str, int]:
    charmap = dict()
    for pos, chars in enumerate(table):
        for char in chars:
            charmap[char] = pos

    return charmap


def read_psf2(filename, header_only=False):
    with open(filename, "rb") as f:
        raw_header = f.read(header_struct.size)
        parsed = header_struct.unpack(raw_header)

        if parsed.magic != MAGIC:
            raise Exception("Invalid magic")

        if header_only:
            return parsed, None, None

        f.seek(parsed.headerSize)

        glyphs = [f.read(parsed.charsize) for _ in range(parsed.length)]

        charmap = None
        if header.flags / 0x1:  # if has unicode table
            rest = f.read()
            table = parse_unicode_table(rest)
            charmap = flip_unicode_table(table)

        return parsed, glyphs, charmap


def display_font(filename):
    header, glyphs, charmap = read_psf2(filename)
    bytes_per_row = math.ceil(header.width / 8)

    iwidth = 32 * header.width
    iheight = 16 * header.height

    img = Image.new("1", (iwidth, iheight))
    d = ImageDraw.Draw(img)

    if glyphs is None:
        raise Exception("Glyphs empty")

    for i, glyph in enumerate(glyphs):
        xx = header.width * i
        x = xx % iwidth
        y = xx // iwidth * header.height

        # for li, line in enumerate(chunks(glyph, bytes_per_row)):
        mask = Image.frombytes("1", (header.width, header.height), glyph)
        img.paste(mask, (x, y))

    img.show()


def display_string(filename, string):
    header, glyphs, charmap = read_psf2(filename)
    bytes_per_row = math.ceil(header.width / 8)

    iwidth = 32 * header.width
    iheight = 16 * header.height

    img = Image.new("1", (iwidth, iheight))
    d = ImageDraw.Draw(img)

    if glyphs is None:
        raise Exception("Glyphs empty")

    for i, char in enumerate(string):
        xx = header.width * i
        x = xx % iwidth
        y = xx // iwidth * header.height

        # for li, line in enumerate(chunks(glyph, bytes_per_row)):
        position = charmap.get(char, 63)
        glyph = glyphs[position]

        mask = Image.frombytes("1", (header.width, header.height), glyph)
        img.paste(mask, (x, y))

    img.show()


if __name__ == "__main__":
    # header, glyphs = read_psf2("./ter-u18n.psf")
    # bytes_per_row = math.ceil(header.width / 8)

    files = glob.glob("*.psf")
    files.sort()

    for filename in files:
        try:
            header, _, _ = read_psf2(filename, header_only=True)
            has_unicode_table = bool(header.flags & 0x01)
            print(
                f"{filename + ':':<15} {header.length} char   {'has table' if has_unicode_table else ' no table'}   ({header.width}×{header.height})"
            )
        except:
            print(f"{filename + ':':<15} Invalid magic")

    # display_font("ter-v20n.psf")
    # display_font("ter-v20b.psf")
    display_string(
        "ter-v20b.psf",
        "©2021 Valtýr Örn Kjartansson                                    Að minnsta kosti einn er látinn eftir að 12 hæða íbúðahús hrundi að hluta í strandbænum Surfside í Miami í Flórída í nótt. aáAÁðÐeéÉiIíÍoOóÓuUúÚyYýÝ",
    )
