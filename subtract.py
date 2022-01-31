#!/usr/bin/env python3
from math import floor
import sys


def mod(x, b):
    return x - b * floor(x / b)


def normalize(a: str, b: str) -> str:
    while len(a) > len(b):
        b = '0' + b
    return a, b


def unpad(a: str) -> str:
    while a[0] == '0':
        a = a[1:]
    return a


def subtract(a: str, b: str) -> str:
    BASE = 10
    # assert len(a) == len(b), "work the simple case for now"
    res = []
    i = 1
    borrow = 0
    while i <= len(a) or i <= len(b):
        if i <= len(a) and i <= len(b):
            part_sub = int(a[-i]) - int(b[-i]) + borrow
            res.append(str(mod(part_sub, BASE)))
            borrow = floor(part_sub / BASE)
        elif i <= len(a):
            part_sub = int(a[-i]) + borrow
            res.append(str(mod(part_sub, BASE)))
            borrow = floor(part_sub / BASE)
        else:
            part_sub = -int(b[-i]) + borrow
            res.append(str(mod(part_sub, BASE)))
            borrow = floor(part_sub / BASE)
        i += 1

    return "".join(reversed(res))


def main():
    a, b = normalize('123', '23')
    a = unpad(a)
    b = unpad(b)
    print(a, b)

    ans = subtract(sys.argv[1], sys.argv[2])
    print(ans)


if __name__ == "__main__":
    main()
