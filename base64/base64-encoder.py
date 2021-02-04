def toBase64Char(num: int) -> str:
    assert 0 <= num <= 63
    if num <= 25:
        return chr(ord('A') + num)
    elif num <= 51:
        return chr(ord('a') + num - 26)
    elif num <= 61:
        return chr(ord('0') + num - 52)
    else:
        return '+' if num == 62 else '/'

def concatenate(tup: tuple):
    return '' if not tup else (tup[0] + concatenate(tup[1:]))

def convertTriple(a: int, b: int = -1, c: int = -1) -> str:
    d, e, f, g = (a & 0xFC) >> 2, ((a & 0x3) << 4) | ((b & 0xF0) >> 4),\
         ((b & 0xF) << 2) | ((c & 0xC0) >> 6), c & 0x3F
    if b < 0:
        return toBase64Char(d) + toBase64Char((a & 0x3) << 4) + '=='
    elif c < 0:
        return toBase64Char(d) + toBase64Char(e) + toBase64Char((b & 0xF) << 2) + '='
    else:
        return concatenate(tuple(map(toBase64Char, [d, e, f, g])))

instr = tuple(input().encode(encoding='utf-8'))
base64str = ''
while len(instr) > 2:
    base64str += convertTriple(*(instr[:3]))
    instr = instr[3:]
if instr:
    base64str += convertTriple(*instr)
print(base64str)

