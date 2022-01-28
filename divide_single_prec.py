#!/usr/bin/env python

from typing import List


def divide(dividend: List[int], divisor: int):
    assert 0 <= divisor <= 9
    quotient = 0
    for digit in reversed(dividend):
        print(digit)


def main():
    divide([3, 2, 1], 9) # expect 13


if __name__ == '__main__':
    main()
