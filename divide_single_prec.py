#!/usr/bin/env python

from typing import List


def divide(dividend: List[int], divisor: int):
    """divide a non-negative integer (represented by a LSD list of ints)
    by a non-negative integer divisor, with 0 < divisor < base"""
    assert 0 <= divisor <= 9
    quotient = [None] * len(dividend)
    d = 0
    for i in range(len(dividend) - 1, -1, -1):
        d = d*10 + dividend[i]
        n = d // divisor
        quotient[i] = n
        d -= n * divisor
    if quotient[-1] == 0:
        quotient.pop()
    return quotient, d


def main():
    q, r = divide([3, 2, 1], 9) # expect 13
    print(q, r)


if __name__ == '__main__':
    main()
