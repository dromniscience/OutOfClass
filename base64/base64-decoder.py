def checkValidity(obj: str) -> bool:
    obj += '='
    if (len(obj) & 0x3) != 1:
        return False
    tmp = obj.count('=')
    if tmp > 3:
        return False
    obj = obj[:-tmp]
    for _ in obj:
        if not (_.isdigit() or _.islower() or _.isupper() or _ in ('+', '/')):
            return False
    return True

def toBaseInt(a: str) -> int:
    if a.isupper():
        return ord(a) - ord('A')
    elif a.islower():
        return ord(a) - ord('a') + 26
    elif a.isdigit():
        return ord(a) - ord('0') + 52
    else:
        return 62 if a == '+' else 63

def convertQuater(a: str, b: str, c: str, d: str) -> bytes:
    e, f, g, h = tuple(map(toBaseInt, [a, b, c, d]))
    l, m, n = ((e & 0x3F) << 2) | ((f & 0x30) >> 4), ((f & 0xF) << 4) | ((g & 0x3C) >> 2),\
         ((g & 0x3) << 6) | (h & 0x3F)
    ret = bytes((l,m,n))
    return ret[:1] if c == '=' else (ret[:2] if d == '=' else ret)

instr = input()
if not checkValidity(instr):
    print('\033[0;31;40mError:\t\033[0m', end='')
    print('Invalid input.')
bytestr = b''
while instr:
    bytestr += convertQuater(*(instr[:4]))
    instr = instr[4:]
print(bytestr.decode('utf-8'))