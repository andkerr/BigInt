def most_siginificant_dig(n: int, base: int) -> int:
    '''return the most significant digit of and integer n'''
    from math import log
    return int(n / 10**int(log(n, base)))


def main():
    pass


if __name__ == '__main__':
    main()
